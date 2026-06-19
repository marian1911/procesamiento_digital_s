/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dac.h"
#include "dma.h"
#include "gpio.h"
#include "tim.h"

/* Includes privadas --------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "arm_math.h"
/* USER CODE END Includes */

/* Tipos privados -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Definiciones privadas -----------------------------------------------------*/
/* USER CODE BEGIN PD */
/* Buffer DMA ping-pong: cada mitad contiene un bloque completo de procesamiento DSP. */
#define BLOCK_SIZE 32U
#define LMS_ORDER 32U
#define MWI_SIZE 30U
#define BUFFER_SIZE (2U * BLOCK_SIZE)
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Variables privadas --------------------------------------------------------*/
/* USER CODE BEGIN PV */
/* Buffers DMA circulares independientes para las entradas ADC y las salidas DAC. */
static uint16_t adc_buffer_ch1[BUFFER_SIZE];
static uint16_t adc_buffer_ch2[BUFFER_SIZE];
static uint16_t dac_buffer_ch1[BUFFER_SIZE];
static uint16_t dac_buffer_ch2[BUFFER_SIZE];

/* Señales LMS: ECG deseado, ruido de referencia, ruido estimado y ECG de error. */
static float32_t d_block[BLOCK_SIZE];
static float32_t x_block[BLOCK_SIZE];
static float32_t y_block[BLOCK_SIZE];
static float32_t e_block[BLOCK_SIZE];

/* Señales intermedias generadas por la tubería parcial de Pan-Tompkins. */
static float32_t lpf_block[BLOCK_SIZE];
static float32_t hpf_block[BLOCK_SIZE];
static float32_t der_block[BLOCK_SIZE];
static float32_t sq_block[BLOCK_SIZE];
static float32_t mwi_block[BLOCK_SIZE];

/* Instancia LMS de CMSIS-DSP y estado persistente de adaptación. */
static float32_t lms_coeffs[LMS_ORDER];
static float32_t lms_state[LMS_ORDER + BLOCK_SIZE - 1U];
static arm_lms_instance_f32 lms_instance;

/* Estado compartido con la ISR usado para sincronización ADC y selección DAC2. */
static volatile uint8_t dac2_select = 0U;
static volatile uint8_t adc1_half_ready = 0U;
static volatile uint8_t adc1_full_ready = 0U;
static volatile uint8_t adc2_half_ready = 0U;
static volatile uint8_t adc2_full_ready = 0U;

/* Historias de muestras persistentes requeridas por las etapas recursivas de Pan-Tompkins. */
static float32_t lpf_x_hist[13];
static uint32_t lpf_x_idx = 0U;
static float32_t lpf_y1 = 0.0f;
static float32_t lpf_y2 = 0.0f;

static float32_t hpf_x_hist[33];
static uint32_t hpf_x_idx = 0U;
static float32_t hpf_y1 = 0.0f;

static float32_t der_x_hist[5];
static uint32_t der_x_idx = 0U;

static float32_t mwi_hist[MWI_SIZE];
static uint32_t mwi_idx = 0U;
static uint32_t mwi_count = 0U;
static float32_t mwi_acc = 0.0f;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
static float32_t adc_to_float(uint16_t adc_value);
static uint16_t float_to_dac(float32_t value);
static void process_block(uint16_t *adc1_ptr, uint16_t *adc2_ptr,
                          uint16_t *dac1_ptr, uint16_t *dac2_ptr);
static void pan_tompkins_process_block(float32_t *input, float32_t *lpf,
                                       float32_t *hpf, float32_t *der,
                                       float32_t *sq, float32_t *mwi,
                                       uint32_t block_size);
static float32_t lpf_process_sample(float32_t x);
static float32_t hpf_process_sample(float32_t x_lpf);
static float32_t derivative_process_sample(float32_t x_hpf);
static float32_t moving_window_process_sample(float32_t x_sq);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/**
 * @brief Convierte una muestra ADC de 12 bits sin signo en un voltaje centrado en cero.
 * @param adc_value Muestra ADC cruda en el rango [0, 4095].
 * @return Voltaje de entrada representado en el rango aproximado [-1.65, 1.65] V.
 */
static float32_t adc_to_float(uint16_t adc_value) {
  return ((float32_t)adc_value * 3.3f / 4095.0f) - 1.65f;
}

/**
 * @brief Convierte un voltaje centrado en cero en una muestra DAC de 12 bits saturada.
 * @param value Valor de la señal expresado en voltios.
 * @return Código DAC saturado en el rango [0, 4095].
 */
static uint16_t float_to_dac(float32_t value) {
  float32_t dac = ((value + 1.65f) * 4095.0f) / 3.3f;

  if (dac < 0.0f) {
    dac = 0.0f;
  }
  if (dac > 4095.0f) {
    dac = 4095.0f;
  }

  return (uint16_t)dac;
}

/**
 * @brief Procesa una muestra mediante la ecuación de diferencias del filtro pasa-bajos Pan-Tompkins.
 * @note Las historias de entrada y salida persisten entre bloques DMA.
 */
static float32_t lpf_process_sample(float32_t x) {
  const uint32_t x6_idx = (lpf_x_idx + 13U - 6U) % 13U;
  const uint32_t x12_idx = (lpf_x_idx + 13U - 12U) % 13U;
  float32_t y;

  lpf_x_hist[lpf_x_idx] = x;
  y = (2.0f * lpf_y1) - lpf_y2 + x - (2.0f * lpf_x_hist[x6_idx]) +
      lpf_x_hist[x12_idx];

  lpf_y2 = lpf_y1;
  lpf_y1 = y;
  lpf_x_idx = (lpf_x_idx + 1U) % 13U;

  return y;
}

/**
 * @brief Procesa una muestra de LPF mediante la etapa de filtro pasa-altos Pan-Tompkins.
 * @note La historia circular almacena los términos LPF retrasados requeridos por la
 * ecuación de diferencias.
 */
static float32_t hpf_process_sample(float32_t x_lpf) {
  const uint32_t x16_idx = (hpf_x_idx + 33U - 16U) % 33U;
  const uint32_t x17_idx = (hpf_x_idx + 33U - 17U) % 33U;
  const uint32_t x32_idx = (hpf_x_idx + 33U - 32U) % 33U;
  float32_t y;

  hpf_x_hist[hpf_x_idx] = x_lpf;
  y = hpf_y1 - (x_lpf / 32.0f) + hpf_x_hist[x16_idx] - hpf_x_hist[x17_idx] +
      (hpf_x_hist[x32_idx] / 32.0f);

  hpf_y1 = y;
  hpf_x_idx = (hpf_x_idx + 1U) % 33U;

  return y;
}

/**
 * @brief Aplica la aproximación derivativa de cinco muestras de Pan-Tompkins.
 */
static float32_t derivative_process_sample(float32_t x_hpf) {
  const uint32_t x1_idx = (der_x_idx + 5U - 1U) % 5U;
  const uint32_t x3_idx = (der_x_idx + 5U - 3U) % 5U;
  const uint32_t x4_idx = (der_x_idx + 5U - 4U) % 5U;
  float32_t y;

  der_x_hist[der_x_idx] = x_hpf;
  y = (1.0f / 8.0f) * ((2.0f * x_hpf) + der_x_hist[x1_idx] -
                       der_x_hist[x3_idx] - (2.0f * der_x_hist[x4_idx]));

  der_x_idx = (der_x_idx + 1U) % 5U;

  return y;
}

/**
 * @brief Actualiza el integrador de ventana móvil usando un buffer circular y una
 * suma acumulada.
 * @return Cero hasta que la primera ventana completa esté disponible; de lo
 * contrario, el promedio de las últimas MWI_SIZE muestras al cuadrado.
 */
static float32_t moving_window_process_sample(float32_t x_sq) {
  if (mwi_count < MWI_SIZE) {
    mwi_hist[mwi_idx] = x_sq;
    mwi_acc += x_sq;
    mwi_idx = (mwi_idx + 1U) % MWI_SIZE;
    mwi_count++;

    if (mwi_count < MWI_SIZE) {
      return 0.0f;
    }
  } else {
    mwi_acc -= mwi_hist[mwi_idx];
    mwi_hist[mwi_idx] = x_sq;
    mwi_acc += x_sq;
    mwi_idx = (mwi_idx + 1U) % MWI_SIZE;
  }

  return mwi_acc / (float32_t)MWI_SIZE;
}

/**
 * @brief Ejecuta la tubería parcial de Pan-Tompkins muestra por muestra.
 * @note La ejecución muestra a muestra preserva la continuidad del filtro entre bloques DMA.
 */
static void pan_tompkins_process_block(float32_t *input, float32_t *lpf,
                                       float32_t *hpf, float32_t *der,
                                       float32_t *sq, float32_t *mwi,
                                       uint32_t block_size) {
  for (uint32_t i = 0U; i < block_size; i++) {
    lpf[i] = lpf_process_sample(input[i]);
    hpf[i] = hpf_process_sample(lpf[i]);
    der[i] = derivative_process_sample(hpf[i]);
    sq[i] = der[i] * der[i];
    mwi[i] = moving_window_process_sample(sq[i]);
  }
}

/**
 * @brief Procesa un bloque ADC sincronizado y prepara ambos bloques DAC.
 *
 * ADC1 provee el ECG contaminado d(n), mientras ADC2 proporciona el ruido de
 * referencia x(n). El error de salida LMS e(n) alimenta la tubería Pan-Tompkins.
 * DAC1 siempre genera el resultado del MWI; DAC2 genera la señal diagnóstica seleccionada.
 */
static void process_block(uint16_t *adc1_ptr, uint16_t *adc2_ptr,
                          uint16_t *dac1_ptr, uint16_t *dac2_ptr) {
  for (uint32_t i = 0U; i < BLOCK_SIZE; i++) {
    d_block[i] = adc_to_float(adc1_ptr[i]);
    x_block[i] = adc_to_float(adc2_ptr[i]);
  }

  /* Estimate the correlated noise and obtain the cleaned ECG error signal. */
  arm_lms_f32(&lms_instance, x_block, d_block, y_block, e_block, BLOCK_SIZE);

  pan_tompkins_process_block(e_block, lpf_block, hpf_block, der_block, sq_block,
                             mwi_block, BLOCK_SIZE);

  for (uint32_t i = 0U; i < BLOCK_SIZE; i++) {
    float32_t selected_value;

    dac1_ptr[i] = float_to_dac(mwi_block[i]);

    switch (dac2_select) {
    case 0U:
      selected_value = d_block[i];
      break;
    case 1U:
      selected_value = x_block[i];
      break;
    case 2U:
      selected_value = y_block[i];
      break;
    case 3U:
      selected_value = e_block[i];
      break;
    case 4U:
      selected_value = hpf_block[i];
      break;
    case 5U:
      selected_value = der_block[i];
      break;
    case 6U:
      selected_value = sq_block[i];
      break;
    case 7U:
      selected_value = mwi_block[i];
      break;
    default:
      selected_value = 0.0f;
      break;
    }

    dac2_ptr[i] = float_to_dac(selected_value);
  }
}

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
  /* USER CODE BEGIN 1 */
  const float32_t mu = 0.001f;
  /* USER CODE END 1 */

  /* Reset of all peripherals, Initializes the Flash interface and the Systick.
   */
  HAL_Init();

  /* USER CODE BEGIN Init */
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_ADC2_Init();
  MX_TIM2_Init();
  MX_DAC_Init();

  /* USER CODE BEGIN 2 */
  /* Inicializa el filtro adaptativo una vez; los coeficientes y el estado persisten. */
  arm_lms_init_f32(&lms_instance, LMS_ORDER, lms_coeffs, lms_state, mu,
                   BLOCK_SIZE);

  /* TIM2 proporciona el disparo común que sincroniza las transferencias ADC y DAC. */
  HAL_TIM_Base_Start(&htim2);

  /* Inicia ADC1 (Canal 0 en PA0) en modo DMA */
  HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adc_buffer_ch1, BUFFER_SIZE);

  /* Inicia ADC2 (Canal 1 en PA1) en modo DMA */
  HAL_ADC_Start_DMA(&hadc2, (uint32_t *)adc_buffer_ch2, BUFFER_SIZE);

  /* Inicia DAC Canal 1 en modo DMA (PA4) */
  HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, (uint32_t *)dac_buffer_ch1,
                    BUFFER_SIZE, DAC_ALIGN_12B_R);

  /* Inicia DAC Canal 2 en modo DMA (PA5) */
  HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_2, (uint32_t *)dac_buffer_ch2,
                    BUFFER_SIZE, DAC_ALIGN_12B_R);
  /* USER CODE END 2 */

  while (1) {
    /*
     * Procesa solo mitades coincidentes después de que ambas transferencias DMA ADC
     * independientes reporten finalización. Esto evita combinar muestras de bloques distintos.
     */
    if ((adc1_half_ready != 0U) && (adc2_half_ready != 0U)) {
      adc1_half_ready = 0U;
      adc2_half_ready = 0U;

      process_block(&adc_buffer_ch1[0], &adc_buffer_ch2[0], &dac_buffer_ch1[0],
                    &dac_buffer_ch2[0]);
    }

    if ((adc1_full_ready != 0U) && (adc2_full_ready != 0U)) {
      adc1_full_ready = 0U;
      adc2_full_ready = 0U;

      process_block(&adc_buffer_ch1[BLOCK_SIZE], &adc_buffer_ch2[BLOCK_SIZE],
                    &dac_buffer_ch1[BLOCK_SIZE], &dac_buffer_ch2[BLOCK_SIZE]);
    }

    __WFI();
  }
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
   */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 180;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
    Error_Handler();
  }

  /** Activate the Over-Drive mode
   */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK) {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK) {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
/**
 * @brief Registra la finalización de la primera mitad de cada buffer ADC independiente.
 * @note El procesamiento DSP se delega al bucle principal para mantener la ISR acotada.
 */
void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef *hadc) {
  if (hadc->Instance == ADC1) {
    adc1_half_ready = 1U;
  } else if (hadc->Instance == ADC2) {
    adc2_half_ready = 1U;
  }
}

/**
 * @brief Registra la finalización de la segunda mitad de cada buffer ADC independiente.
 * @note El procesamiento DSP se delega al bucle principal para mantener la ISR acotada.
 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
  if (hadc->Instance == ADC1) {
    adc1_full_ready = 1U;
  } else if (hadc->Instance == ADC2) {
    adc2_full_ready = 1U;
  }
}

/**
 * @brief Selecciona la siguiente señal interna enviada al canal 2 del DAC.
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
  if (GPIO_Pin == SW_FS_Pin) {
    dac2_select = (dac2_select + 1U) % 8U;
  }
}

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
  /* USER CODE BEGIN Error_Handler_Debug */
  __disable_irq();
  while (1) {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line) {
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line
     number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
