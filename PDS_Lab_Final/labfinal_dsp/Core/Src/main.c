/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "arm_math.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define BLOCK_SIZE      32U
#define BUFFER_SIZE     (2U * BLOCK_SIZE)
#define DAC_MID_SCALE   2048U
#define LMS_ORDER       32U
#define LMS_MU          0.001f
#define MWI_SIZE        30U

#define ADC1_READY_FLAG (1UL << 0)
#define ADC2_READY_FLAG (1UL << 1)
#define ADC_BOTH_READY  (ADC1_READY_FLAG | ADC2_READY_FLAG)

#define ADC_DAC_MAX_CODE   4095.0f
#define ANALOG_VDDA        3.3f
#define SIGNAL_BIAS        (ANALOG_VDDA / 2.0f)
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc2;
DMA_HandleTypeDef hdma_adc1;
DMA_HandleTypeDef hdma_adc2;

DAC_HandleTypeDef hdac;
DMA_HandleTypeDef hdma_dac1;
DMA_HandleTypeDef hdma_dac2;

TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
/* Buffers DMA ping-pong: dos mitades de BLOCK_SIZE muestras. */
static uint16_t adc_buffer_ch1[BUFFER_SIZE];
static uint16_t adc_buffer_ch2[BUFFER_SIZE];

static uint16_t dac_buffer_ch1[BUFFER_SIZE];
static uint16_t dac_buffer_ch2[BUFFER_SIZE];

/* Bloques de entrada y salida del filtro adaptativo LMS. */
static float32_t d_block[BLOCK_SIZE];
static float32_t x_block[BLOCK_SIZE];
static float32_t y_block[BLOCK_SIZE];
static float32_t e_block[BLOCK_SIZE];

/* Coeficientes, estado persistente e instancia CMSIS-DSP del LMS. */
static float32_t lms_coeffs[LMS_ORDER];
static float32_t lms_state[LMS_ORDER + BLOCK_SIZE - 1U];
static arm_lms_instance_f32 lms_instance;

/* Bloques intermedios de la cadena de preprocesamiento Pan-Tompkins. */
static float32_t lpf_block[BLOCK_SIZE];
static float32_t hpf_block[BLOCK_SIZE];
static float32_t der_block[BLOCK_SIZE];
static float32_t sq_block[BLOCK_SIZE];
static float32_t mwi_block[BLOCK_SIZE];

/*
 * Estado persistente del filtro pasa-bajos. Las historias deben conservarse
 * entre bloques DMA para procesar la señal como una secuencia continua.
 */
static float32_t lpf_x_hist[13];
static uint32_t lpf_x_idx = 0U;
static float32_t lpf_y1 = 0.0f;
static float32_t lpf_y2 = 0.0f;

/* Estado persistente del filtro pasa-altos. */
static float32_t hpf_x_hist[33];
static uint32_t hpf_x_idx = 0U;
static float32_t hpf_y1 = 0.0f;

/* Historia circular utilizada por la etapa derivativa. */
static float32_t der_x_hist[5];
static uint32_t der_x_idx = 0U;

/* Estado del integrador de ventana móvil de 30 muestras (150 ms a 200 Hz). */
static float32_t mwi_hist[MWI_SIZE];
static uint32_t mwi_idx = 0U;
static uint32_t mwi_count = 0U;
static float32_t mwi_acc = 0.0f;

/* Estado compartido entre las interrupciones DMA y el bucle principal. */
static volatile uint32_t adc_half_ready_mask = 0U;
static volatile uint32_t adc_full_ready_mask = 0U;
static volatile uint32_t adc_error_mask = 0U;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_ADC1_Init(void);
static void MX_ADC2_Init(void);
static void MX_DAC_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/**
 * @brief Convierte una muestra ADC de 12 bits en voltios centrados en cero.
 *
 * @param adc_value Código ADC dentro del rango [0, 4095].
 * @return Tensión aproximada dentro del rango [-VDDA/2, +VDDA/2].
 */
static float32_t adc_to_float(uint16_t adc_value)
{
    float32_t voltage;

    voltage = ((float32_t)adc_value * ANALOG_VDDA) / ADC_DAC_MAX_CODE;

    return voltage - SIGNAL_BIAS;
}

/**
 * @brief Convierte una tensión centrada en cero en un código DAC de 12 bits.
 *
 * @param value Tensión dentro del rango aproximado [-VDDA/2, +VDDA/2].
 * @return Código DAC saturado dentro del rango [0, 4095].
 */
static uint16_t float_to_dac(float32_t value)
{
    float32_t dac_code;

    dac_code = ((value + SIGNAL_BIAS) * ADC_DAC_MAX_CODE) / ANALOG_VDDA;

    if (dac_code <= 0.0f)
    {
        return 0U;
    }

    if (dac_code >= ADC_DAC_MAX_CODE)
    {
        return 4095U;
    }

    /* Suma 0.5 para redondear en lugar de truncar. */
    return (uint16_t)(dac_code + 0.5f);
}

/**
 * @brief Aplica una etapa pasa-bajos de Pan-Tompkins a una muestra.
 *
 * La ecuación utiliza las muestras de entrada x(n), x(n-6) y x(n-12),
 * junto con las dos salidas anteriores. El estado se conserva entre bloques.
 */
static float32_t lpf_process_sample(float32_t x)
{
  uint32_t x6_idx;
  uint32_t x12_idx;
  float32_t y;

  x6_idx = (lpf_x_idx + 13U - 6U) % 13U;
  x12_idx = (lpf_x_idx + 13U - 12U) % 13U;

  lpf_x_hist[lpf_x_idx] = x;

  y = (2.0f * lpf_y1)
      - lpf_y2
      + x
      - (2.0f * lpf_x_hist[x6_idx])
      + lpf_x_hist[x12_idx];

  lpf_y2 = lpf_y1;
  lpf_y1 = y;
  lpf_x_idx = (lpf_x_idx + 1U) % 13U;

  return y;
}

/**
 * @brief Aplica una etapa pasa-altos de Pan-Tompkins a una muestra.
 *
 * Esta etapa elimina la componente de continua y las variaciones lentas que
 * permanecen a la salida del filtro pasa-bajos.
 */
static float32_t hpf_process_sample(float32_t x)
{
  uint32_t x16_idx;
  uint32_t x17_idx;
  uint32_t x32_idx;
  float32_t y;

  x16_idx = (hpf_x_idx + 33U - 16U) % 33U;
  x17_idx = (hpf_x_idx + 33U - 17U) % 33U;
  x32_idx = (hpf_x_idx + 33U - 32U) % 33U;

  hpf_x_hist[hpf_x_idx] = x;

  y = hpf_y1
      - (x / 32.0f)
      + hpf_x_hist[x16_idx]
      - hpf_x_hist[x17_idx]
      + (hpf_x_hist[x32_idx] / 32.0f);

  hpf_y1 = y;
  hpf_x_idx = (hpf_x_idx + 1U) % 33U;

  return y;
}

/**
 * @brief Calcula una aproximación de la derivada mediante cinco muestras.
 *
 * La derivada resalta las pendientes pronunciadas asociadas al complejo QRS.
 */
static float32_t derivative_process_sample(float32_t x)
{
  uint32_t x1_idx;
  uint32_t x3_idx;
  uint32_t x4_idx;
  float32_t y;

  x1_idx = (der_x_idx + 5U - 1U) % 5U;
  x3_idx = (der_x_idx + 5U - 3U) % 5U;
  x4_idx = (der_x_idx + 5U - 4U) % 5U;

  der_x_hist[der_x_idx] = x;

  y = ((2.0f * x)
       + der_x_hist[x1_idx]
       - der_x_hist[x3_idx]
       - (2.0f * der_x_hist[x4_idx])) / 8.0f;

  der_x_idx = (der_x_idx + 1U) % 5U;

  return y;
}

/**
 * @brief Integra una muestra al cuadrado dentro de una ventana móvil.
 *
 * La suma acumulada evita recorrer las 30 muestras en cada actualización.
 * Durante el llenado inicial devuelve cero hasta disponer de una ventana
 * completa de 150 ms.
 */
static float32_t moving_window_process_sample(float32_t x_squared)
{
  if (mwi_count < MWI_SIZE)
  {
    mwi_hist[mwi_idx] = x_squared;
    mwi_acc += x_squared;
    mwi_idx = (mwi_idx + 1U) % MWI_SIZE;
    mwi_count++;

    if (mwi_count < MWI_SIZE)
    {
      return 0.0f;
    }
  }
  else
  {
    mwi_acc -= mwi_hist[mwi_idx];
    mwi_hist[mwi_idx] = x_squared;
    mwi_acc += x_squared;
    mwi_idx = (mwi_idx + 1U) % MWI_SIZE;
  }

  return mwi_acc / (float32_t)MWI_SIZE;
}

/**
 * @brief Ejecuta el preprocesamiento Pan-Tompkins sobre un bloque completo.
 *
 * Las etapas aplicadas son: pasa-bajos, pasa-altos, derivada, elevación al
 * cuadrado e integración de ventana móvil. No incluye todavía detección de
 * picos ni umbrales adaptativos.
 */
static void pan_tompkins_process_block(const float32_t *input,
                                       uint32_t block_size)
{
  for (uint32_t i = 0U; i < block_size; i++)
  {
    lpf_block[i] = lpf_process_sample(input[i]);
    hpf_block[i] = hpf_process_sample(lpf_block[i]);
    der_block[i] = derivative_process_sample(hpf_block[i]);
    sq_block[i] = der_block[i] * der_block[i];
    mwi_block[i] = moving_window_process_sample(sq_block[i]);
  }
}

/**
 * @brief Ejecuta LMS y el preprocesamiento Pan-Tompkins sobre un bloque.
 *
 * ADC1 contiene la señal deseada d(n), mientras ADC2 proporciona la
 * referencia de ruido x(n). El error e(n) del LMS alimenta Pan-Tompkins.
 * DAC1 entrega el integrador de ventana móvil y DAC2 el ECG filtrado por LMS.
 */
static void process_block(const uint16_t *adc1_ptr,
                          const uint16_t *adc2_ptr,
                          uint16_t *dac1_ptr,
                          uint16_t *dac2_ptr)
{
  for (uint32_t i = 0U; i < BLOCK_SIZE; i++)
  {
    d_block[i] = adc_to_float(adc1_ptr[i]);
    x_block[i] = adc_to_float(adc2_ptr[i]);
  }

  arm_lms_f32(&lms_instance,
              x_block,
              d_block,
              y_block,
              e_block,
              BLOCK_SIZE);

  /* El ECG filtrado por LMS alimenta todas las etapas de Pan-Tompkins. */
  pan_tompkins_process_block(e_block, BLOCK_SIZE);

  for (uint32_t i = 0U; i < BLOCK_SIZE; i++)
  {
    /* DAC1 permite observar la energía integrada asociada al complejo QRS. */
    dac1_ptr[i] = float_to_dac(mwi_block[i]);

    /* DAC2 conserva como referencia el ECG obtenido a la salida del LMS. */
    dac2_ptr[i] = float_to_dac(e_block[i]);
  }
}

/**
 * @brief Extrae un evento cuando ambos ADC completaron la misma mitad.
 * @param ready_mask Máscara de sincronización de la mitad consultada.
 * @retval 1 si el bloque está listo; 0 en caso contrario.
 */
static uint8_t adc_take_ready_block(volatile uint32_t *ready_mask)
{
  uint8_t block_ready = 0U;
  uint32_t primask = __get_PRIMASK();

  __disable_irq();

  if ((*ready_mask & ADC_BOTH_READY) == ADC_BOTH_READY)
  {
    *ready_mask &= ~ADC_BOTH_READY;
    block_ready = 1U;
  }

  if (primask == 0U)
  {
    __enable_irq();
  }

  return block_ready;
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
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
  MX_USART2_UART_Init();
  MX_ADC1_Init();
  MX_ADC2_Init();
  MX_DAC_Init();
  MX_TIM2_Init();

  /* USER CODE BEGIN 2 */

/* Inicializa una única vez el estado y los coeficientes del filtro LMS. */
arm_lms_init_f32(&lms_instance,
                 LMS_ORDER,
                 lms_coeffs,
                 lms_state,
                 LMS_MU,
                 BLOCK_SIZE);

/* Inicializa las salidas DAC en media escala: aproximadamente VDDA / 2. */
for (uint32_t i = 0U; i < BUFFER_SIZE; i++)
{
    dac_buffer_ch1[i] = DAC_MID_SCALE;
    dac_buffer_ch2[i] = DAC_MID_SCALE;
}

/* Arma primero las transferencias DMA del DAC. */
/* DAC 1 */
if (HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, (uint32_t *)dac_buffer_ch1, BUFFER_SIZE, DAC_ALIGN_12B_R) != HAL_OK)
{
    Error_Handler();
}

/* DAC 2 */
if (HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_2, (uint32_t *)dac_buffer_ch2, BUFFER_SIZE, DAC_ALIGN_12B_R) != HAL_OK)
{
    Error_Handler();
}

/* Arma las adquisiciones ADC mediante DMA circular. */
/* ADC 1 */
if (HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adc_buffer_ch1, BUFFER_SIZE) != HAL_OK)
{
    Error_Handler();
}

/* ADC 2 */
if (HAL_ADC_Start_DMA(&hadc2, (uint32_t *)adc_buffer_ch2, BUFFER_SIZE) != HAL_OK)
{
   Error_Handler();
}

/*
 * Inicia TIM2 al final. Desde este momento, cada TRGO de 200 Hz
 * dispara simultáneamente los ADC y los DAC.
 */
if (HAL_TIM_Base_Start(&htim2) != HAL_OK)
{
    Error_Handler();
}

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    /* Primera mitad: índices 0 a BLOCK_SIZE - 1. */
    if (adc_take_ready_block(&adc_half_ready_mask) != 0U)
    {

       process_block(&adc_buffer_ch1[0],
                     &adc_buffer_ch2[0],
                     &dac_buffer_ch1[0],
                     &dac_buffer_ch2[0]);

    }

    /* Segunda mitad: índices BLOCK_SIZE a BUFFER_SIZE - 1. */
    if (adc_take_ready_block(&adc_full_ready_mask) != 0U)
    {

       process_block(&adc_buffer_ch1[BLOCK_SIZE],
                     &adc_buffer_ch2[BLOCK_SIZE],
                     &dac_buffer_ch1[BLOCK_SIZE],
                     &dac_buffer_ch2[BLOCK_SIZE]);

    }

    if (adc_error_mask != 0U)
    {
      /* Pendiente: agregar diagnóstico o recuperación del ADC. */
    }

  }
  /* USER CODE END 3 */
}





/* ==================================================================================================================================*/
/* =============================================== CONFIGURACIONES ==================================================================*/
/* ==================================================================================================================================*/

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 360;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV6;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
  hadc1.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T2_TRGO;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = ENABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief ADC2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC2_Init(void)
{

  /* USER CODE BEGIN ADC2_Init 0 */

  /* USER CODE END ADC2_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC2_Init 1 */

  /* USER CODE END ADC2_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc2.Instance = ADC2;
  hadc2.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV6;
  hadc2.Init.Resolution = ADC_RESOLUTION_12B;
  hadc2.Init.ScanConvMode = DISABLE;
  hadc2.Init.ContinuousConvMode = DISABLE;
  hadc2.Init.DiscontinuousConvMode = DISABLE;
  hadc2.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
  hadc2.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T2_TRGO;
  hadc2.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc2.Init.NbrOfConversion = 1;
  hadc2.Init.DMAContinuousRequests = ENABLE;
  hadc2.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc2) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC2_Init 2 */

  /* USER CODE END ADC2_Init 2 */

}

/**
  * @brief DAC Initialization Function
  * @param None
  * @retval None
  */
static void MX_DAC_Init(void)
{

  /* USER CODE BEGIN DAC_Init 0 */

  /* USER CODE END DAC_Init 0 */

  DAC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN DAC_Init 1 */

  /* USER CODE END DAC_Init 1 */

  /** DAC Initialization
  */
  hdac.Instance = DAC;
  if (HAL_DAC_Init(&hdac) != HAL_OK)
  {
    Error_Handler();
  }

  /** DAC channel OUT1 config
  */
  sConfig.DAC_Trigger = DAC_TRIGGER_T2_TRGO;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
  if (HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }

  /** DAC channel OUT2 config
  */
  if (HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DAC_Init 2 */

  /* USER CODE END DAC_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 89;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 4999;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);
  /* DMA1_Stream6_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream6_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream6_IRQn);
  /* DMA2_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);
  /* DMA2_Stream2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/**
 * @brief Notifica la finalización de la primera mitad de un buffer ADC.
 */
void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef *hadc)
{
  if (hadc->Instance == ADC1)
  {
    adc_half_ready_mask |= ADC1_READY_FLAG;
  }
  else if (hadc->Instance == ADC2)
  {
    adc_half_ready_mask |= ADC2_READY_FLAG;
  }
}

/**
 * @brief Notifica la finalización de la segunda mitad de un buffer ADC.
 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
  if (hadc->Instance == ADC1)
  {
    adc_full_ready_mask |= ADC1_READY_FLAG;
  }
  else if (hadc->Instance == ADC2)
  {
    adc_full_ready_mask |= ADC2_READY_FLAG;
  }
}

/**
 * @brief Registra errores producidos durante una adquisición ADC por DMA.
 */
void HAL_ADC_ErrorCallback(ADC_HandleTypeDef *hadc)
{
  if (hadc->Instance == ADC1)
  {
    adc_error_mask |= ADC1_READY_FLAG;
  }
  else if (hadc->Instance == ADC2)
  {
    adc_error_mask |= ADC2_READY_FLAG;
  }
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
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
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
