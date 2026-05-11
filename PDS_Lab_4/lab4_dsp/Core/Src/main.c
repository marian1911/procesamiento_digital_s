/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    main.c
 * @brief   Cancelacion adaptiva de ruido con filtro LMS - cadena corregida
 *
 * ARQUITECTURA DE LA CADENA DE SEÑAL (CORRECTA):
 *
 *  Fuente de ruido
 *       │
 *       ├─→ x_ref(n)  [referencia limpia]  ──────────────────────────────→ [LMS ref]
 *       │
 *       └─→ [FIR contam.] → x_contam(n)  [ruido colorado/retardado]
 *                                │
 *  ADC → s(n) ──────────────────(+)→ d(n) [señal deseada] ─────────────→ [LMS des]
 *                                                                              │
 *                                              DAC CH2: d(n) (monitor) ←──────┤
 *                                                                              │
 *                                              y(n) = LMS estima x_contam     │
 *                                                                              │
 *                                              e(n) = d(n) - y(n) ≈ s(n) ──→ DAC CH1
 *
 * DIFERENCIA CON VERSION ANTERIOR:
 *   - Antes: FIR se aplicaba a x para obtener x_corr, y ESO iba al LMS como ref.
 *     El resultado era x_corr ≈ x (misma fuente), sin diferencia entre CH1 y CH2.
 *   - Ahora: FIR modela la trayectoria del ruido (contamina), generando x_contam.
 *     El LMS recibe x_ref (sin colorar) como referencia, aprende la trayectoria
 *     del FIR y cancela x_contam de d(n). e(n) converge a s(n).
 ******************************************************************************
 */
/* USER CODE END Header */

#include "main.h"
#include "adc.h"
#include "dac.h"
#include "dma.h"
#include "tim.h"
#include "gpio.h"
#include <stdint.h>
#include "arm_math.h"

/* Private define ------------------------------------------------------------*/
#define BUFFER_SIZE   512
#define CONTAM_TAPS    30   /* Taps del FIR que modela la trayectoria del ruido */
#define ADAPT_TAPS     60   /* Taps del filtro LMS adaptivo */

/* Private variables ---------------------------------------------------------*/
extern TIM_HandleTypeDef htim2;
extern ADC_HandleTypeDef hadc1;
extern DAC_HandleTypeDef hdac;
extern DMA_HandleTypeDef hdma_dac1;
extern DMA_HandleTypeDef hdma_dac2;

uint16_t adc_buffer[BUFFER_SIZE];
uint16_t dac_buffer[BUFFER_SIZE];    /* DAC CH1: señal limpia estimada e(n) ≈ s(n) */
uint16_t dac_buffer2[BUFFER_SIZE];   /* DAC CH2: señal ruidosa d(n) = s(n)+x_contam */

float32_t s_buffer[BUFFER_SIZE / 2];       /* s(n): señal de entrada del ADC, normalizada */
float32_t x_ref_buffer[BUFFER_SIZE / 2];   /* x_ref(n): referencia de ruido LIMPIA (sin colorar) */
float32_t x_contam_buffer[BUFFER_SIZE / 2];/* x_contam(n): ruido COLORADO por FIR (lo que llega a s) */
float32_t d_buffer[BUFFER_SIZE / 2];       /* d(n) = s(n) + x_contam(n) */
float32_t y_buffer[BUFFER_SIZE / 2];       /* y(n): estimacion de x_contam por el LMS */
float32_t e_buffer[BUFFER_SIZE / 2];       /* e(n) = d(n) - y(n) ≈ s(n) */

volatile uint32_t proc_cycles_half = 0;
volatile uint32_t proc_cycles_full = 0;
static volatile uint8_t processing_busy = 0;

/*
 * Estado del LCG: continuo entre bloques (NO reiniciar).
 * El ruido debe ser un proceso estacionario continuo, no una secuencia
 * que se repite cada bloque. Un único estado garantiza que x_ref(n)
 * sea una secuencia de ruido blanco pseudo-aleatoria sin discontinuidades.
 */
static uint32_t rng_state = 0xDEADBEEFu;

/*
 * FIR que modela la trayectoria acustica/electronica del ruido.
 * Convierte x_ref(n) → x_contam(n): introduce retardo y coloracion
 * espectral, simulando cómo el ruido llega a la señal de interés
 * a través de un canal fisico.
 *
 * Estos coeficientes definen un filtro paso-banda (los mismos del original).
 * En un sistema real, se medirían con una señal de prueba.
 */
static arm_fir_instance_f32 fir_contam;
static float32_t fir_contam_coeffs[CONTAM_TAPS] = {
    -0.0016f, -0.002f,  -0.0012f,  0.0018f,  0.0068f,  0.0101f,
     0.006f,  -0.0084f, -0.0279f, -0.0381f, -0.0217f,  0.0305f,
     0.1102f,  0.1918f,  0.2436f,  0.2436f,  0.1918f,  0.1102f,
     0.0305f, -0.0217f, -0.0381f, -0.0279f, -0.0084f,  0.006f,
     0.0101f,  0.0068f,  0.0018f, -0.0012f, -0.002f,  -0.0016f
};
/* Estado persistente del FIR: mantiene memoria entre bloques consecutivos */
static float32_t fir_contam_state[BUFFER_SIZE / 2 + CONTAM_TAPS - 1]; /* 285 elementos */

/* Filtro LMS adaptivo */
arm_lms_instance_f32 lms;
float32_t lms_coeffs[ADAPT_TAPS];  /* Inicializados a 0 por arm_lms_init_f32 */
float32_t lms_state[BUFFER_SIZE / 2 + ADAPT_TAPS - 1]; /* 315 elementos */

/*
 * Paso de adaptacion mu.
 *
 * Criterio de estabilidad: 0 < mu < 1 / (ADAPT_TAPS * Px_ref)
 *
 * x_ref tiene amplitud 0.3, potencia Px ≈ 0.3^2/3 ≈ 0.03
 * Limite superior: 1 / (60 * 0.03) ≈ 0.556
 *
 * Se usa 0.005 para convergencia estable. Aumentar hasta 0.05 si
 * se necesita convergencia más rápida, verificando estabilidad en e(n).
 *
 * NOTA IMPORTANTE: la ganancia del FIR (paso-banda con pico ~0.49)
 * atenua x_contam respecto a x_ref. Esto facilita la convergencia
 * porque el LMS debe aprender coeficientes de magnitud menor a 1.
 */
float32_t mu = 0.005f;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN 0 */

/**
 * @brief Genera ruido blanco pseudo-aleatorio (LCG de 24 bits).
 *
 * El estado es global y continuo entre llamadas: produce una secuencia
 * estacionaria sin saltos ni repeticiones entre bloques.
 *
 * Para produccion: reemplazar por HAL_RNG_GenerateRandomNumber() usando
 * el periférico RNG hardware del STM32F4 (RCC_AHB2Periph_RNG).
 *
 * @param x        Buffer de salida
 * @param N        Numero de muestras
 * @param amplitud Amplitud pico en rango [-amplitud, +amplitud]
 */
void generar_ruido(float32_t *x, uint32_t N, float32_t amplitud)
{
    for (uint32_t i = 0; i < N; i++) {
        rng_state = 1664525u * rng_state + 1013904223u;
        uint32_t rnd = rng_state & 0x00FFFFFFu;
        float32_t u = (float32_t)rnd / 16777216.0f;
        x[i] = amplitud * (2.0f * u - 1.0f);
    }
}

/**
 * @brief Convierte buffer float32_t [-1.0, +1.0] al formato DAC de 12 bits.
 */
void convert_float_to_dac_buffer(float32_t *in, uint16_t *out, uint16_t len)
{
    for (uint16_t i = 0; i < len; i++) {
        float32_t scaled = (in[i] + 1.0f) * 2047.5f;
        int32_t val = (int32_t)(scaled + 0.5f);
        if (val < 0)    val = 0;
        if (val > 4095) val = 4095;
        out[i] = (uint16_t)val;
    }
}

/**
 * @brief Procesa un semibuffer aplicando cancelacion adaptiva de ruido.
 *
 * CADENA DE PROCESAMIENTO:
 *
 *  1. ADC → s(n): señal de interés (normalizada)
 *  2. LCG → x_ref(n): referencia de ruido limpia (blanco)
 *  3. FIR(x_ref) → x_contam(n): ruido colorado/retardado (simula trayectoria)
 *  4. d(n) = s(n) + x_contam(n): señal deseada (contaminada)
 *  5. DAC CH2 ← d(n): monitor de señal ruidosa de entrada
 *  6. LMS(ref=x_ref, des=d) → y(n): estimacion de x_contam
 *  7. e(n) = d(n) - y(n) ≈ s(n): señal limpia recuperada
 *  8. DAC CH1 ← e(n): salida del sistema
 *
 * Por que funciona:
 *   El LMS recibe x_ref como referencia (correlado con x_contam via FIR)
 *   y aprende a modelar el FIR de contaminacion. Una vez convergido,
 *   y(n) ≈ x_contam(n), y e(n) cancela el ruido dejando s(n).
 *
 * @param adc_in  Buffer ADC de 12 bits
 * @param dac_out Buffer DAC CH1 (señal limpia)
 * @param offset  0 o BUFFER_SIZE/2 segun semibuffer
 * @param cycles  Contador de ciclos de ejecucion (para profiling)
 */
void process_buffer_half(uint16_t *adc_in, uint16_t *dac_out,
                         uint16_t offset, volatile uint32_t *cycles)
{
    uint32_t start = DWT->CYCCNT;

    /* ── Paso 1: ADC → s(n) normalizado a [-1, +1] ──────────────────────── */
    for (uint16_t i = 0; i < BUFFER_SIZE / 2; i++) {
        float32_t val = ((float32_t)adc_in[i + offset] - 2048.0f) / 2047.5f;
        /* Clamp por seguridad ante valores ADC extremos */
        if (val >  1.0f) val =  1.0f;
        if (val < -1.0f) val = -1.0f;
        s_buffer[i] = val;
    }

    /* ── Paso 2: Generar x_ref(n) — referencia de ruido limpia ──────────── */
    generar_ruido(x_ref_buffer, BUFFER_SIZE / 2, 0.3f);

    /* ── Paso 3: x_contam(n) = FIR(x_ref) — ruido colorado ─────────────── */
    /*
     * El FIR modela la trayectoria del ruido desde la fuente hasta s(n).
     * x_ref es la señal "en la fuente" (referencia del micrófono de ruido).
     * x_contam es lo que "llega" a la señal útil tras propagación/filtrado.
     *
     * CLAVE: x_ref y x_contam son DISTINTOS en forma espectral y fase,
     * pero correlados. Esta diferencia es lo que el LMS debe aprender.
     * Si fueran iguales, el LMS no tendría nada que hacer.
     */
    arm_fir_f32(&fir_contam, x_ref_buffer, x_contam_buffer, BUFFER_SIZE / 2);

    /* ── Paso 4: d(n) = s(n) + x_contam(n) — señal deseada contaminada ─── */
    for (uint16_t i = 0; i < BUFFER_SIZE / 2; i++) {
        d_buffer[i] = s_buffer[i] + x_contam_buffer[i];
    }

    /* ── Paso 5: DAC CH2 ← d(n) — monitor de señal ruidosa ─────────────── */
    convert_float_to_dac_buffer(d_buffer, &dac_buffer2[offset], BUFFER_SIZE / 2);

    /* ── Paso 6+7: LMS adaptivo ─────────────────────────────────────────── */
    /*
     * Referencia: x_ref_buffer  (ruido blanco en la fuente)
     * Deseada:    d_buffer       (señal + ruido colorado)
     * Salida:     y_buffer       (estimacion del ruido colorado)
     * Error:      e_buffer       (señal limpia recuperada)
     *
     * El LMS aprende los coeficientes del FIR de contaminacion.
     * Cuando converge: y(n) ≈ x_contam(n), e(n) ≈ s(n).
     *
     * Tiempo de convergencia estimado con mu=0.005:
     *   Bloques ≈ 5 / (mu * ADAPT_TAPS * Px_ref) ≈ 5/(0.005*60*0.03) ≈ 556 bloques
     *   A 16kHz con BUFFER_SIZE/2=256: ~8.9 segundos hasta convergencia completa.
     *   Durante ese tiempo e(n) ira reduciendo progresivamente el ruido visible.
     */
    arm_lms_f32(&lms, x_ref_buffer, d_buffer, y_buffer, e_buffer, BUFFER_SIZE / 2);

    /* ── Paso 8: DAC CH1 ← e(n) — señal limpia estimada ────────────────── */
    convert_float_to_dac_buffer(e_buffer, &dac_out[offset], BUFFER_SIZE / 2);

    *cycles = DWT->CYCCNT - start;
}

/**
 * @brief DMA Half Transfer Complete — primer semibuffer [0 .. BUFFER_SIZE/2-1]
 */
void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef *hadc)
{
    if (hadc->Instance != ADC1) return;
    if (processing_busy) return;
    processing_busy = 1;
    process_buffer_half(adc_buffer, dac_buffer, 0, &proc_cycles_half);
    processing_busy = 0;
}

/**
 * @brief DMA Transfer Complete — segundo semibuffer [BUFFER_SIZE/2 .. BUFFER_SIZE-1]
 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    if (hadc->Instance != ADC1) return;
    if (processing_busy) return;
    processing_busy = 1;
    process_buffer_half(adc_buffer, dac_buffer, BUFFER_SIZE / 2, &proc_cycles_full);
    processing_busy = 0;
}

/**
 * @brief TIM2 Period Elapsed Callback (16 kHz).
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM2) { }
}

/* USER CODE END 0 */

int main(void)
{
    /* Habilitar contador de ciclos DWT */
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0;
    DWT->CTRL  |= DWT_CTRL_CYCCNTENA_Msk;

    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_ADC1_Init();
    MX_TIM2_Init();
    MX_DAC_Init();

    /* TIM2: 16 kHz exactos con CLK_TIM2 = 90 MHz → ARR = 5624 */
    __HAL_TIM_SET_AUTORELOAD(&htim2, 5624);

    /*
     * Inicializar FIR de contaminacion.
     * Estado persistente entre bloques: mantiene memoria de las ultimas
     * CONTAM_TAPS muestras para continuidad de la convolucion.
     */
    arm_fir_init_f32(&fir_contam, CONTAM_TAPS, fir_contam_coeffs,
                     fir_contam_state, BUFFER_SIZE / 2);

    /*
     * Inicializar LMS.
     * Los coeficientes arrancan en 0 y convergen durante la operacion.
     * mu = 0.005: convergencia lenta pero estable (~9 seg a 16kHz).
     *
     * Para verificar convergencia en desarrollo:
     *   - Conectar osciloscopio en DAC CH1 y CH2
     *   - CH2 debe mostrar señal ruidosa desde el primer bloque
     *   - CH1 debe mostrar ruido decreciente hasta revelar s(n)
     *   - Si ambas salidas son identicas, verificar que x_contam != x_ref
     *     poniendo un breakpoint y comparando los buffers
     */
    arm_lms_init_f32(&lms, ADAPT_TAPS, lms_coeffs, lms_state, mu, BUFFER_SIZE / 2);

    HAL_TIM_Base_Start_IT(&htim2);

    HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, (uint32_t *)dac_buffer,
                      BUFFER_SIZE, DAC_ALIGN_12B_R);
    HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_2, (uint32_t *)dac_buffer2,
                      BUFFER_SIZE, DAC_ALIGN_12B_R);
    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adc_buffer, BUFFER_SIZE);

    while (1)
    {
        /*
         * Monitoreo opcional desde debugger o UART:
         *   proc_cycles_half  → ciclos del semibuffer 1
         *   proc_cycles_full  → ciclos del semibuffer 2
         *   lms_coeffs[]      → coeficientes adaptados (deben alejarse de 0 al converger)
         *
         * Para verificar convergencia sin osciloscopio:
         *   Calcular energia de e_buffer y d_buffer.
         *   Cuando energia(e) << energia(d), el LMS convergio.
         */
    }
}

void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState            = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState        = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource       = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM            = 8;
    RCC_OscInitStruct.PLL.PLLN            = 180;
    RCC_OscInitStruct.PLL.PLLP            = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ            = 2;
    RCC_OscInitStruct.PLL.PLLR            = 2;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) Error_Handler();

    if (HAL_PWREx_EnableOverDrive() != HAL_OK) Error_Handler();

    RCC_ClkInitStruct.ClockType      = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                     | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK) Error_Handler();
}

void Error_Handler(void)
{
    __disable_irq();
    while (1) {}
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line) { }
#endif
