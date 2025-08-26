/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Real-Time Signal Acquisition and Filtering using STM32F446RE
 *
 * This program implements a real-time digital signal processing (DSP) pipeline
 * on the STM32F446RE microcontroller. It captures analog input via ADC, processes
 * the signal in Q15 fixed-point format using biquad IIR filters (LP, HP, BP, BS),
 * and outputs the result through the DAC using DMA. The sampling rate (8–48 kHz)
 * and filter type are dynamically selectable using external GPIO interrupts.
 *
 * Key Features:
 * - Double-buffered ADC/DAC using DMA for real-time throughput
 * - Q15 format filtering with CMSIS-DSP `arm_biquad_cascade_df1_q15`
 * - Dynamic selection of filter type and sampling frequency via GPIO
 * - Processing time profiling using DWT->CYCCNT
 * - Visual feedback using RGB LEDs
 *
 * Author          : Campos Mariano
 * Date            : 24/06/2025
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
#include "arm_math.h" // CMSIS-DSP library for fixed-point processing

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define BUFFER_SIZE 512     // Size of ADC/DAC/Q15 buffers
#define FS_8K   11249       // Auto-reload value for TIM2 to achieve ~8kHz sampling with 90 MHz clock
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
uint8_t filter_type = 0;			 // type of the filter LP,HP,BP,BS
uint32_t last_interrupt_time = 0;    // Time of the last interrupt anti-bouncing

const uint8_t postshift [5][4] = {
    {14, 14, 14, 14},  // fs = 8k Hz: LP, HP, BP, BS
    {16, 14, 13, 14},  // fs = 16kHz
    {15, 14, 13, 14},  // fs = 24kHz
    {14, 14, 13, 14},  // fs = 40kHz
    {14, 14, 13, 14}   // fs = 48kHz
};

arm_biquad_casd_df1_inst_q15 filter;

// ===== LOWPASS Filter =====
// fs = 8000 Hz
// CMSIS-DSP format: {b0, b1, b2, -a1, -a2} in Q15
// Suggested postShift: 14
#define stage_low_8k 1
q15_t low_8k[5 * stage_low_8k] = { 26233, 32767, 26233, -32768, -21015 };
q15_t low_8k_status[4 * stage_low_8k] = { 0 };

// ===== LOWPASS Filter =====
// fs = 16000 Hz
// CMSIS-DSP format: {b0, b1, b2, -a1, -a2} in Q15
// Suggested postShift: 16
#define stage_low_16k 1
q15_t low_16k[5 * stage_low_16k] = { 8137/10, 16275/10, 8137/10, 6036/10, -5819/10 };
q15_t low_16k_status[4 * stage_low_16k] = { 0 };

// ===== LOWPASS Filter =====
// fs = 24000 Hz
// CMSIS-DSP format: {b0, b1, b2, -a1, -a2} in Q15
// Suggested postShift: 15
#define stage_low_24k 1
q15_t low_24k[5 * stage_low_24k] = { 4296/10, 8592/10, 4296/10, 24503/10, -8920/10 };
q15_t low_24k_status[4 * stage_low_24k] = { 0 };

// ===== LOWPASS Filter =====
// fs = 40000 Hz
// CMSIS-DSP format: {b0, b1, b2, -a1, -a2} in Q15
// Suggested postShift: 14
#define stage_low_40k 1
q15_t low_40k[5 * stage_low_40k] = { 1850/10, 3699/10, 1850/10, 32767/10, -14760/10 };
q15_t low_40k_status[4 * stage_low_40k] = { 0 };

// ===== LOWPASS Filter =====
// fs = 48000 Hz
// CMSIS-DSP format: {b0, b1, b2, -a1, -a2} in Q15
// Suggested postShift: 14
#define stage_low_48k 1
q15_t low_48k[5 * stage_low_48k] = { 1352/10, 2704/10, 1352/10, 32767/10, -16842/10 };
q15_t low_48k_status[4 * stage_low_48k] = { 0 };

// ===== HIGHPASS Filter =====
// fs = 8000 Hz
// CMSIS-DSP format: {b0, b1, b2, -a1, -a2} in Q15
// Suggested postShift: 14
#define stage_high_8k 1
q15_t high_8k[5 * stage_high_8k] = { 32136, -32768, 32136, 32767, -31518 };
q15_t high_8k_status[4 * stage_high_8k] = { 0 };

// ===== HIGHPASS Filter =====
// fs = 16000 Hz
// CMSIS-DSP format: {b0, b1, b2, -a1, -a2} in Q15
// Suggested postShift: 14
#define stage_high_16k 1
q15_t high_16k[5 * stage_high_16k] = { 32450, -32768, 32450, 32767, -32136 };
q15_t high_16k_status[4 * stage_high_16k] = { 0 };

// ===== HIGHPASS Filter =====
// fs = 24000 Hz
// CMSIS-DSP format: {b0, b1, b2, -a1, -a2} in Q15
// Suggested postShift: 14
#define stage_high_24k 1
q15_t high_24k[5 * stage_high_24k] = { 32555, -32768, 32555, 32767, -32345 };
q15_t high_24k_status[4 * stage_high_24k] = { 0 };

// ===== HIGHPASS Filter =====
// fs = 40000 Hz
// CMSIS-DSP format: {b0, b1, b2, -a1, -a2} in Q15
// Suggested postShift: 14
#define stage_high_40k 1
q15_t high_40k[5 * stage_high_40k] = { 32640, -32768, 32640, 32767, -32513 };
q15_t high_40k_status[4 * stage_high_40k] = { 0 };

// ===== HIGHPASS Filter =====
// fs = 48000 Hz
// CMSIS-DSP format: {b0, b1, b2, -a1, -a2} in Q15
// Suggested postShift: 14
#define stage_high_48k 1
q15_t high_48k[5 * stage_high_48k] = { 32661, -32768, 32661, 32767, -32555 };
q15_t high_48k_status[4 * stage_high_48k] = { 0 };

// ===== BANDPASS Filter =====
// fs = 8000 Hz
// CMSIS-DSP format: {b0, b1, b2, -a1, -a2} in Q15
// Suggested postShift: 13
#define stage_band_8k 2
q15_t band_8k[5 * stage_band_8k] = { 24325, 32767, 24325, -32768, -18813, 32767,
		-32768, 32767, 32767, -31518 };
q15_t band_8k_status[4 * stage_band_8k] = { 0 };

// ===== BANDPASS Filter =====
// fs = 16000 Hz
// CMSIS-DSP format: {b0, b1, b2, -a1, -a2} in Q15
// Suggested postShift: 13
#define stage_band_16k 2
q15_t band_16k[5 * stage_band_16k] = { 7666, 15331, 7666, 7891, -6092, 32767,
		-32768, 32767, 32767, -32136 };
q15_t band_16k_status[4 * stage_band_16k] = { 0 };

// ===== BANDPASS Filter =====
// fs = 24000 Hz
// CMSIS-DSP format: {b0, b1, b2, -a1, -a2} in Q15
// Suggested postShift: 13
#define stage_band_24k 2
q15_t band_24k[5 * stage_band_24k] = { 4041, 8081, 4041, 25939, -9444, 32767,
		-32768, 32767, 32767, -32345 };
q15_t band_24k_status[4 * stage_band_24k] = { 0 };

// ===== BANDPASS Filter =====
// fs = 40000 Hz
// CMSIS-DSP format: {b0, b1, b2, -a1, -a2} in Q15
// Suggested postShift: 13
#define stage_band_40k 2
q15_t band_40k[5 * stage_band_40k] = { 1733, 3466, 1733, 32767, -15322, 32767,
		-32768, 32767, 32767, -32513 };
q15_t band_40k_status[4 * stage_band_40k] = { 0 };

// ===== BANDPASS Filter =====
// fs = 48000 Hz
// CMSIS-DSP format: {b0, b1, b2, -a1, -a2} in Q15
// Suggested postShift: 13
#define stage_band_48k 2
q15_t band_48k[5 * stage_band_48k] = { 1265, 2529, 1265, 32767, -17377, 32767,
		-32768, 32767, 32767, -32555 };
q15_t band_48k_status[4 * stage_band_48k] = { 0 };

// ===== BAND_STOP Filter =====
// fs = 8000 Hz
// CMSIS-DSP format: {b0, b1, b2, -a1, -a2} in Q15
// Suggested postShift: 14
#define stage_band_stop_8k 1
q15_t band_stop_8k[5 * stage_band_stop_8k] = { 32385, -32768, 32385, 32767,
		-32004 };
q15_t band_stop_8k_status[4 * stage_band_stop_8k] = { 0 };

// ===== BAND_STOP Filter =====
// fs = 16000 Hz
// CMSIS-DSP format: {b0, b1, b2, -a1, -a2} in Q15
// Suggested postShift: 14
#define stage_band_stop_16k 1
q15_t band_stop_16k[5 * stage_band_stop_16k] = { 32575, -32768, 32575, 32767,
		-32383 };
q15_t band_stop_16k_status[4 * stage_band_stop_16k] = { 0 };

// ===== BAND_STOP Filter =====
// fs = 24000 Hz
// CMSIS-DSP format: {b0, b1, b2, -a1, -a2} in Q15
// Suggested postShift: 14
#define stage_band_stop_24k 1
q15_t band_stop_24k[5 * stage_band_stop_24k] = { 32639, -32768, 32639, 32767,
		-32511 };
q15_t band_stop_24k_status[4 * stage_band_stop_24k] = { 0 };

// ===== BAND_STOP Filter =====
// fs = 40000 Hz
// CMSIS-DSP format: {b0, b1, b2, -a1, -a2} in Q15
// Suggested postShift: 14
#define stage_band_stop_40k 1
q15_t band_stop_40k[5 * stage_band_stop_40k] = { 32690, -32768, 32690, 32767,
		-32613 };
q15_t band_stop_40k_status[4 * stage_band_stop_40k] = { 0 };

// ===== BAND_STOP Filter =====
// fs = 48000 Hz
// CMSIS-DSP format: {b0, b1, b2, -a1, -a2} in Q15
// Suggested postShift: 14
#define stage_band_stop_48k 1
q15_t band_stop_48k[5 * stage_band_stop_48k] = { 32703, -32768, 32703, 32767,
		-32639 };
q15_t band_stop_48k_status[4 * stage_band_stop_48k] = { 0 };

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
		out[i] = (uint16_t) val;
	}
}

/**
 * @brief Processes half of the ADC buffer: converts to Q15, applies processing, and prepares DAC output
 * @param adc_in: Input ADC buffer
 * @param q15_out: Intermediate Q15 buffer
 * @param dac_out: Output DAC buffer
 * @param offset: Offset to select first or second half of buffer
 */
void process_buffer_half(uint16_t *adc_in, q15_t *q15_out, uint16_t *dac_out,
		uint16_t offset) {
	uint32_t start = DWT->CYCCNT; // Start cycle counter for performance profiling

	// Convert ADC 12-bit values to Q15 format
	for (uint16_t i = 0; i < BUFFER_SIZE / 2; i++) {
		q15_out[i + offset] = (q15_t) (((int32_t) adc_in[i + offset] * 32767)
				/ 4095);
	}

	arm_biquad_cascade_df1_q15(&filter, &q15_out[offset], &q15_out[offset],
	BUFFER_SIZE / 2);

	// Convert processed Q15 data to DAC output format
	convert_q15_to_dac_buffer(&q15_out[offset], &dac_out[offset],
	BUFFER_SIZE / 2);

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
		process_buffer_half(adc_buffer, q15_buffer, dac_buffer,
		BUFFER_SIZE / 2);
	}
}

/**
 * @brief GPIO interrupt callback for switching sampling frequencies or stopping acquisition
 *        Cycles through 6 modes on button press (pin: SW_FS) and switching type filter through
 *        4 modes, BP, HP, BP and BS on button press (pin: SW_MODE_1)
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {

	if (GPIO_Pin == SW_FS_Pin) {
		fs_mode = (fs_mode + 1) % 6;

		// Stop timer before reconfiguring
		__HAL_TIM_DISABLE(&htim2);

		switch (fs_mode) {
		case 0:
			__HAL_TIM_SET_AUTORELOAD(&htim2, FS_8K);        // 8 kHz
			HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, GPIO_PIN_SET); // Blue
			HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, GPIO_PIN_RESET);
			break;
		case 1:
			__HAL_TIM_SET_AUTORELOAD(&htim2, FS_8K / 2);    // ~16 kHz
			HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, GPIO_PIN_SET);  // Red
			HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, GPIO_PIN_RESET);
			break;
		case 2:
			__HAL_TIM_SET_AUTORELOAD(&htim2, FS_8K / 3);    // ~24 kHz
			HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, GPIO_PIN_SET); // Green
			break;
		case 3:
			__HAL_TIM_SET_AUTORELOAD(&htim2, FS_8K / 5);    // ~40 kHz
			HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, GPIO_PIN_SET); // Red + Green = Yellow
			HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, GPIO_PIN_SET);
			break;
		case 4:
			__HAL_TIM_SET_AUTORELOAD(&htim2, FS_8K / 6);    // ~48 kHz
			HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, GPIO_PIN_SET); // Blue + Red = Purple
			HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, GPIO_PIN_RESET);
			break;
		case 5:
			// Stop all activity
			HAL_GPIO_WritePin(LED_B_GPIO_Port, LED_B_Pin, GPIO_PIN_RESET); // All off
			HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED_G_GPIO_Port, LED_G_Pin, GPIO_PIN_RESET);
			__HAL_TIM_DISABLE(&htim2);
			memset(&filter, 0, sizeof(filter));
			return;
		}
		// Force change filter type when switch frequency sample
		HAL_GPIO_EXTI_Callback(SW_MODE_1_Pin);

		// Reset and enable timer
		__HAL_TIM_SET_COUNTER(&htim2, 0);
		TIM2->EGR = TIM_EGR_UG;
		__HAL_TIM_ENABLE(&htim2);
	}

	if (GPIO_Pin == SW_MODE_1_Pin) {
		filter_type = (filter_type + 1) % 4;

		switch (fs_mode) {
		case 0: // 8 kHz

			switch (filter_type) {
			case 0:
				memset(low_8k_status, 0, sizeof(low_8k_status));
				arm_biquad_cascade_df1_init_q15(&filter, stage_low_8k, low_8k,
						low_8k_status, postshift[0][0]);
				break;
			case 1:
				memset(high_8k_status, 0, sizeof(high_8k_status));
				arm_biquad_cascade_df1_init_q15(&filter, stage_high_8k, high_8k,
						high_8k_status, postshift[0][1]);
				break;
			case 2:
				memset(band_8k_status, 0, sizeof(band_8k_status));
				arm_biquad_cascade_df1_init_q15(&filter, stage_band_8k, band_8k,
						band_8k_status, postshift[0][2]);
				break;
			case 3:
				memset(band_stop_8k_status, 0, sizeof(band_stop_8k_status));
				arm_biquad_cascade_df1_init_q15(&filter, stage_band_stop_8k,
						band_stop_8k, band_stop_8k_status, postshift[0][3]);
				break;
			}
			break;

		case 1: // 16 kHz
			switch (filter_type) {
			case 0:
				memset(low_16k_status, 0, sizeof(low_16k_status));
				arm_biquad_cascade_df1_init_q15(&filter, stage_low_16k, low_16k,
						low_16k_status, postshift[1][0]);
				break;
			case 1:
				memset(high_16k_status, 0, sizeof(high_16k_status));
				arm_biquad_cascade_df1_init_q15(&filter, stage_high_16k,
						high_16k, high_16k_status, postshift[1][1]);
				break;
			case 2:
				memset(band_16k_status, 0, sizeof(band_16k_status));
				arm_biquad_cascade_df1_init_q15(&filter, stage_band_16k,
						band_16k, band_16k_status, postshift[1][2]);
				break;
			case 3:
				memset(band_stop_16k_status, 0, sizeof(band_stop_16k_status));
				arm_biquad_cascade_df1_init_q15(&filter, stage_band_stop_16k,
						band_stop_16k, band_stop_16k_status, postshift[1][3]);
				break;
			}
			break;

		case 2: // 24 kHz
			switch (filter_type) {
			case 0:
				memset(low_24k_status, 0, sizeof(low_24k_status));
				arm_biquad_cascade_df1_init_q15(&filter, stage_low_24k, low_24k,
						low_24k_status, postshift[2][0]);
				break;
			case 1:
				memset(high_24k_status, 0, sizeof(high_24k_status));
				arm_biquad_cascade_df1_init_q15(&filter, stage_high_24k,
						high_24k, high_24k_status, postshift[2][1]);
				break;
			case 2:
				memset(band_24k_status, 0, sizeof(band_24k_status));
				arm_biquad_cascade_df1_init_q15(&filter, stage_band_24k,
						band_24k, band_24k_status, postshift[2][2]);
				break;
			case 3:
				memset(band_stop_24k_status, 0, sizeof(band_stop_24k_status));
				arm_biquad_cascade_df1_init_q15(&filter, stage_band_stop_24k,
						band_stop_24k, band_stop_24k_status, postshift[2][3]);
				break;
			}
			break;

		case 3: // 40 kHz
			switch (filter_type) {
			case 0:
				memset(low_40k_status, 0, sizeof(low_40k_status));
				arm_biquad_cascade_df1_init_q15(&filter, stage_low_40k, low_40k,
						low_40k_status, postshift[3][0]);
				break;
			case 1:
				memset(high_40k_status, 0, sizeof(high_40k_status));
				arm_biquad_cascade_df1_init_q15(&filter, stage_high_40k,
						high_40k, high_40k_status, postshift[3][1]);
				break;
			case 2:
				memset(band_40k_status, 0, sizeof(band_40k_status));
				arm_biquad_cascade_df1_init_q15(&filter, stage_band_40k,
						band_40k, band_40k_status, postshift[3][2]);
				break;
			case 3:
				memset(band_stop_40k_status, 0, sizeof(band_stop_40k_status));
				arm_biquad_cascade_df1_init_q15(&filter, stage_band_stop_40k,
						band_stop_40k, band_stop_40k_status, postshift[3][3]);
				break;
			}
			break;

		case 4: // 48 kHz
			switch (filter_type) {
			case 0:
				memset(low_48k_status, 0, sizeof(low_48k_status));
				arm_biquad_cascade_df1_init_q15(&filter, stage_low_48k, low_48k,
						low_48k_status, postshift[4][0]);
				break;
			case 1:
				memset(high_48k_status, 0, sizeof(high_48k_status));
				arm_biquad_cascade_df1_init_q15(&filter, stage_high_48k,
						high_48k, high_48k_status, postshift[4][1]);
				break;
			case 2:
				memset(band_48k_status, 0, sizeof(band_48k_status));
				arm_biquad_cascade_df1_init_q15(&filter, stage_band_48k,
						band_48k, band_48k_status, postshift[4][2]);
				break;
			case 3:
				memset(band_stop_48k_status, 0, sizeof(band_stop_48k_status));
				arm_biquad_cascade_df1_init_q15(&filter, stage_band_stop_48k,
						band_stop_48k, band_stop_48k_status, postshift[4][3]);
				break;
			}
			break;
		}
	}
}
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {

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
	HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, (uint32_t*) dac_buffer,
	BUFFER_SIZE, DAC_ALIGN_12B_R);

	// Start ADC in DMA mode
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*) adc_buffer, BUFFER_SIZE);

	memset(low_8k_status, 0, sizeof(low_8k_status));
	arm_biquad_cascade_df1_init_q15(&filter, stage_low_8k, low_8k,
			low_8k_status, postshift[0][0]);

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
		// Main loop intentionally left empty. All processing handled via DMA callbacks and interrupts.
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
