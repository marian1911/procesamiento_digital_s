/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : ADC sampling -> Q15 processing -> using DMA
 *
 * This program implements real-time signal acquisition and analysis using
 * the STM32F446RE . It samples an analog signal via ADC,
 * converts it to Q15 format, optionally computes the FFT using CMSIS-DSP,
 * and transmits the result via UART. User input through GPIO allows for
 * runtime configuration of sampling rate, FFT size, and processing mode.
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "arm_math.h" // CMSIS-DSP library for fixed-point signal processing
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define BUFFER_SIZE 2048                    // DMA buffer size (circular)
#define MAX_FFT_SIZE 2048                   // Maximum FFT length
#define MAX_COMPLEX_SIZE (2 * MAX_FFT_SIZE) // Real + Imag interleaved
#define MAX_MAG_SIZE (MAX_FFT_SIZE / 2 + 1) // Magnitude spectrum size
#define FS_8K   11249        // Auto-reload value for TIM2 to achieve ~8kHz sampling with 90 MHz clock
#define FS_16K  5624
#define FS_22K  4090
#define FS_44K  2041
#define FS_48K  1874
#define DEBOUNCE_DELAY 50	// Time anti-bounce
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
extern TIM_HandleTypeDef htim2;      // Timer used to trigger ADC conversions
extern ADC_HandleTypeDef hadc1;      // ADC handle for sampling input
extern UART_HandleTypeDef huart4;    // UART handle for data transmission

// CMSIS-DSP FFT structures for different transform lengths
extern arm_cfft_instance_q15 arm_cfft_sR_q15_len512;
extern arm_cfft_instance_q15 arm_cfft_sR_q15_len1024;
extern arm_cfft_instance_q15 arm_cfft_sR_q15_len2048;

uint16_t adc_buffer[BUFFER_SIZE];     // DMA circular buffer for raw ADC samples
q15_t window_buffer[MAX_FFT_SIZE];           // Accumulator for windowed samples
uint16_t raw_buffer[MAX_FFT_SIZE];          // Raw ADC samples (for bypass mode)
q15_t q15_complex_buffer[MAX_COMPLEX_SIZE]; // Complex buffer for FFT (real/imag interleaved)
q15_t magnitudes_buffer[MAX_MAG_SIZE]; // Buffer for magnitude spectrum (static, no VLA)

volatile uint8_t fs_mode = 0;          // Sampling frequency mode selector (0–5)
volatile uint32_t time_fft = 0;                     // Time to compute FFT
volatile uint32_t time_mag = 0;                     // Time to compute magnitude
volatile uint32_t time_uart = 0;              // Time spent in UART transmission
volatile uint32_t time_total = 0;                // Total time from start to end
volatile uint8_t window_size = 0; // FFT window size index (0: 512, 1: 1024, 2: 2048)
volatile uint16_t data_size = 512;                  // Current FFT input size
volatile uint8_t bypass = 0;            // Bypass FFT (send raw Q15 data if set)
volatile uint16_t window_idx = 0;      // Current position in window accumulator
volatile uint8_t window_ready = 0;    // Flag: window complete, ready to process
uint32_t last_interrupt_time = 0;    // Time of the last interrupt anti-bouncing

arm_cfft_instance_q15 *transform = NULL; // Pointer to the current FFT configuration
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

// Accumulate ADC samples into window buffer. Called from DMA callbacks.
// Signals when a complete FFT window is ready (window_ready = 1).
void accumulate_adc_to_window(uint16_t *adc_half, uint16_t half_len) {
	uint16_t to_copy = half_len;
	uint16_t src = 0;

	while (to_copy) {
		uint16_t need = data_size - window_idx;
		uint16_t cnt = (to_copy < need) ? to_copy : need;

		// Convert ADC to Q15 and accumulate
		for (uint16_t i = 0; i < cnt; ++i) {
			uint16_t adc_val = adc_half[src++];
			raw_buffer[window_idx] = adc_val;      // keep raw sample for bypass
			window_buffer[window_idx++] = (q15_t) (((int32_t) adc_val * 32767)
					/ 4095);
		}
		to_copy -= cnt;

		// If window is complete, signal main loop
		if (window_idx >= data_size) {
			window_ready = 1;
			window_idx = 0;
		}
	}
}

// Process complete FFT window: compute FFT, magnitude, and transmit.
// Called from main loop (NOT from ISR) to avoid blocking.
void process_window(void) {
	uint32_t t_start = DWT->CYCCNT;

	if (!bypass && transform != NULL) {
		// Step 1: Prepare complex buffer (real part = window_buffer, imag = 0)
		for (uint16_t i = 0; i < data_size; i++) {
			q15_complex_buffer[2 * i] = window_buffer[i];
			q15_complex_buffer[2 * i + 1] = 0;
		}

		// Step 2: FFT computation
		uint32_t t_fft_start = DWT->CYCCNT;
		arm_cfft_q15(transform, q15_complex_buffer, 0, 1);
		uint32_t t_fft_end = DWT->CYCCNT;

		// Step 3: Magnitude computation using CMSIS-DSP (correct Q15 arithmetic)
		uint32_t t_mag_start = DWT->CYCCNT;
		uint16_t N = data_size / 2 + 1;
		arm_cmplx_mag_q15(q15_complex_buffer, magnitudes_buffer, N);
		uint32_t t_mag_end = DWT->CYCCNT;

		// Step 4: UART transmission (now safe to use blocking function in main context)
		uint32_t t_uart_start = DWT->CYCCNT;

		uint8_t syn = 15;
		HAL_UART_Transmit(&huart4, &syn, 1, HAL_MAX_DELAY);

		uint8_t header[3] = { fs_mode, window_size, bypass };
		HAL_UART_Transmit(&huart4, header, 3, HAL_MAX_DELAY);
		HAL_UART_Transmit(&huart4, (uint8_t*) magnitudes_buffer,
				N * sizeof(q15_t), HAL_MAX_DELAY);

		const uint8_t footer[] = { '\n', '\n' };
		HAL_UART_Transmit(&huart4, footer, sizeof(footer), HAL_MAX_DELAY);
		uint32_t t_uart_end = DWT->CYCCNT;

		// Record timing
		time_fft = t_fft_end - t_fft_start;
		time_mag = t_mag_end - t_mag_start;
		time_uart = t_uart_end - t_uart_start;
		time_total = t_uart_end - t_start;
	} else {
		// Bypass: send original ADC samples (uint16_t) instead of Q15
		uint32_t t_uart_start = DWT->CYCCNT;

		uint8_t syn = 15;
		HAL_UART_Transmit(&huart4, &syn, 1, HAL_MAX_DELAY);

		uint8_t header[3] = { fs_mode, window_size, bypass };
		HAL_UART_Transmit(&huart4, header, 3, HAL_MAX_DELAY);
		HAL_UART_Transmit(&huart4, (uint8_t*) raw_buffer,
				data_size * sizeof(uint16_t), HAL_MAX_DELAY);

		const uint8_t footer[] = { '\n', '\n' };
		HAL_UART_Transmit(&huart4, footer, sizeof(footer), HAL_MAX_DELAY);
		uint32_t t_uart_end = DWT->CYCCNT;
		time_uart = t_uart_end - t_uart_start;
		time_fft = 0;
		time_mag = 0;
		time_total = t_uart_end - t_start;
	}
}

// Called when first half of the DMA buffer is filled (BUFFER_SIZE/2 samples)
void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef *hadc) {
	if (hadc->Instance == ADC1) {
		// Only accumulate, do NOT process FFT or transmit UART here (heavy work belongs in main)
		accumulate_adc_to_window(&adc_buffer[0], BUFFER_SIZE / 2);
	}
}

// Called when the second half of the DMA buffer is filled (BUFFER_SIZE/2 samples)
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
	if (hadc->Instance == ADC1) {
		// Only accumulate, do NOT process FFT or transmit UART here
		accumulate_adc_to_window(&adc_buffer[BUFFER_SIZE / 2], BUFFER_SIZE / 2);
	}
}

// Handle GPIO interrupts for mode control
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {

	uint32_t current_time = HAL_GetTick();
	if ((current_time - last_interrupt_time) < DEBOUNCE_DELAY) {
		return;
	}
	last_interrupt_time = current_time;

	if (GPIO_Pin == SW_FS_Pin) {
		fs_mode = (fs_mode + 1) % 6;         // Cycle through 6 sampling rates
		__HAL_TIM_DISABLE(&htim2);

		switch (fs_mode) {
		case 0:
			__HAL_TIM_SET_AUTORELOAD(&htim2, FS_8K);           // ~8 kHz
			HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, GPIO_PIN_RESET);
			break;
		case 1:
			__HAL_TIM_SET_AUTORELOAD(&htim2, FS_16K);       // ~16 kHz
			HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, GPIO_PIN_RESET);
			break;
		case 2:
			__HAL_TIM_SET_AUTORELOAD(&htim2, FS_22K);       // ~22 kHz
			HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, GPIO_PIN_RESET);
			break;
		case 3:
			__HAL_TIM_SET_AUTORELOAD(&htim2, FS_44K);       // ~44 kHz
			HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, GPIO_PIN_RESET);
			break;
		case 4:
			__HAL_TIM_SET_AUTORELOAD(&htim2, FS_48K);       // ~48 kHz
			HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, GPIO_PIN_RESET);
			break;
		case 5:
			__HAL_TIM_DISABLE(&htim2);                         // Stop sampling
			HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, GPIO_PIN_RESET);
			return;
		}

		__HAL_TIM_SET_COUNTER(&htim2, 0);
		TIM2->EGR = TIM_EGR_UG; // Update event
		__HAL_TIM_ENABLE(&htim2);
	}

	if (GPIO_Pin == SW_MODE_1_Pin) {
		// Cycle through FFT sizes (protected from race conditions)
		__HAL_TIM_DISABLE(&htim2);
		__disable_irq();

		// Update FFT configuration
		window_size = (window_size + 1) % 3;
		switch (window_size) {
		case 0:
			transform = &arm_cfft_sR_q15_len512;
			data_size = 512;
			break;
		case 1:
			transform = &arm_cfft_sR_q15_len1024;
			data_size = 1024;
			break;
		case 2:
			transform = &arm_cfft_sR_q15_len2048;
			data_size = 2048;
			break;
		}

		// Clear window buffer and reset state
		for (uint16_t i = 0; i < MAX_FFT_SIZE; i++) {
			window_buffer[i] = 0;
		}
		window_idx = 0;     // Reset accumulator position
		window_ready = 0;   // Clear ready flag to ensure clean start

		__enable_irq();

		// Restart timer
		__HAL_TIM_SET_COUNTER(&htim2, 0);
		TIM2->EGR = TIM_EGR_UG; // Update event
		__HAL_TIM_ENABLE(&htim2);
	}

	if (GPIO_Pin == SW_MODE_2_Pin) {
		bypass = !bypass; // Toggle FFT processing
	}
}
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {

	/* USER CODE BEGIN 1 */
	// Enable DWT counter for execution time measurement
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
	MX_UART4_Init();
	/* USER CODE BEGIN 2 */
	// Default mode: 8kHz sampling and 512-point FFT
	HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, GPIO_PIN_RESET);

	HAL_TIM_Base_Start(&htim2); // Start timer
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*) adc_buffer, BUFFER_SIZE); // Start ADC DMA

	transform = &arm_cfft_sR_q15_len512;
	data_size = 512;
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
		// Process complete FFT window when ready (safe from main loop, not ISR)
		if (window_ready) {
			__disable_irq();
			window_ready = 0;
			__enable_irq();
			process_window(); // FFT, magnitude computation, and UART transmission
		}
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

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
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK) {
		Error_Handler();
	}
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
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
