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
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "arm_math.h" // CMSIS-DSP library for fixed-point processing
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define BUFFER_SIZE 512     // Size of ADC/DAC/Q15 buffers
#define FS_8K   11249        // Auto-reload value for TIM2 to achieve ~8kHz sampling with 90 MHz clock
#define FS_16K  5624
#define FS_22K  4090
#define FS_44K  2041
#define FS_48K  1874
#define DEBOUNCE_DELAY 200	// Time anti-bounce
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
extern TIM_HandleTypeDef htim2;      // Timer used for triggering ADC/DAC
extern ADC_HandleTypeDef hadc1;      // ADC handle
extern DAC_HandleTypeDef hdac;       // DAC handle

uint16_t adc_buffer[BUFFER_SIZE];    // Raw ADC samples
uint16_t dac_buffer[BUFFER_SIZE];    // Output buffer for DAC
q15_t q15_buffer[BUFFER_SIZE];       // Intermediate Q15 format buffer
uint8_t fs_mode = 0;                 // Sampling frequency mode selector (0–5)
uint32_t time = 0;                   // Variable to store CPU cycles for processing time profiling
uint32_t last_interrupt_time = 0;    // Time of the last interrupt anti-bouncing
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/**
 * @brief Converts a Q15 array to a 12-bit DAC buffer
 * @param in: Input Q15 data array
 * @param out: Output DAC buffer
 * @param len: Number of elements to convert
 */
void convert_q15_to_dac_buffer(q15_t *in, uint16_t *out, uint16_t len) {
	for (uint16_t i = 0; i < len; i++) {
		int32_t val = ((int32_t) in[i] * 4095) / 32767;
		if (val < 0)
			val = 0;
		else if (val > 4095)
			val = 4095;
		out[i] = (uint16_t) (val);
	}
}

/**
 * @brief Processes half of the ADC buffer: converts to Q15, applies processing, and prepares DAC output
 * @param adc_in: Input ADC buffer
 * @param q15_out: Intermediate Q15 buffer
 * @param dac_out: Output DAC buffer
 * @param offset: Offset to select first or second half of buffer
 */
void process_buffer_half(uint16_t *adc_in, q15_t *q15_out, uint16_t *dac_out, uint16_t offset) {
	uint32_t start = DWT->CYCCNT; // Start cycle counter for performance profiling

	// Convert ADC 12-bit values to Q15 format
	for (uint16_t i = 0; i < BUFFER_SIZE / 2; i++) {
		q15_out[i + offset] = (q15_t)(((int32_t) adc_in[i + offset] * 32767) / 4095);
	}


	// Convert processed Q15 data to DAC output format
	convert_q15_to_dac_buffer(&q15_out[offset], &dac_out[offset], BUFFER_SIZE / 2);
	time = DWT->CYCCNT - start; // Measure processing time in CPU cycles
}

/**
 * @brief DMA Half Transfer Complete Callback for ADC
 */
void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef *hadc) {
	if (hadc->Instance == ADC1) {
		process_buffer_half(adc_buffer, q15_buffer, dac_buffer, 0);
	}
}

/**
 * @brief DMA Transfer Complete Callback for ADC
 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
	if (hadc->Instance == ADC1) {
		process_buffer_half(adc_buffer, q15_buffer, dac_buffer, BUFFER_SIZE / 2);

	}
}

/**
 * @brief GPIO interrupt callback for switching sampling frequencies or stopping acquisition
 *        Cycles through 6 modes on button press (pin: SW_FS)
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {

	// Anti-bounce
	 uint32_t current_time = HAL_GetTick();
	 if ((current_time - last_interrupt_time) < DEBOUNCE_DELAY) {
	         return;
	     }
	 last_interrupt_time = current_time;

	if (GPIO_Pin == SW_FS_Pin) {
		fs_mode = (fs_mode + 1) % 6;

		// Stop timer before reconfiguring
		__HAL_TIM_DISABLE(&htim2);

		switch (fs_mode) {
		case 0:
			__HAL_TIM_SET_AUTORELOAD(&htim2, FS_8K);        // 8 kHz
			HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, GPIO_PIN_SET);    // Blue
			HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, GPIO_PIN_RESET);
			break;
		case 1:
			__HAL_TIM_SET_AUTORELOAD(&htim2, FS_16K);    // ~16 kHz
			HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, GPIO_PIN_SET);    // Red
			HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, GPIO_PIN_RESET);
			break;
		case 2:
			__HAL_TIM_SET_AUTORELOAD(&htim2, FS_22K);    // ~22 kHz
			HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, GPIO_PIN_SET);    // Green
			break;
		case 3:
			__HAL_TIM_SET_AUTORELOAD(&htim2, FS_44K);    // ~44 kHz
			HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, GPIO_PIN_SET);    // Red + Green = Yellow
			HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, GPIO_PIN_SET);
			break;
		case 4:
			__HAL_TIM_SET_AUTORELOAD(&htim2, FS_48K);    // ~48 kHz
			HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, GPIO_PIN_SET);    // Blue + Red = Purple
			HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, GPIO_PIN_RESET);
			break;
		case 5:
			// Stop all activity
			HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, GPIO_PIN_RESET);    // All off
			HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, GPIO_PIN_RESET);
			__HAL_TIM_DISABLE(&htim2);
			return;
		}

		// Reset and enable timer
		__HAL_TIM_SET_COUNTER(&htim2, 0);
		TIM2->EGR = TIM_EGR_UG;
		__HAL_TIM_ENABLE(&htim2);
	}
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
	// Enable DWT cycle counter for profiling purposes
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
	DWT->CYCCNT = 0;
	DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
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
  MX_ADC1_Init();
  MX_TIM2_Init();
  MX_DAC_Init();
  /* USER CODE BEGIN 2 */

	HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, GPIO_PIN_SET);    // Blue
	HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, GPIO_PIN_RESET);

	// Start base timer
	HAL_TIM_Base_Start(&htim2);

	// Start DAC in DMA mode
	HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, (uint32_t*) dac_buffer, BUFFER_SIZE, DAC_ALIGN_12B_R);

	// Start ADC in DMA mode
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*) adc_buffer, BUFFER_SIZE);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1) {
    /* USER CODE END WHILE */
		__WFI();
    /* USER CODE BEGIN 3 */
		// Main loop intentionally left empty. All processing handled via DMA callbacks and interrupts.
	}
  /* USER CODE END 3 */
}

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
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
	// Stay in infinite loop on error
	__disable_irq();
	while (1) {
	}
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
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
  // Custom assert handler (optional user-defined implementation)
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
