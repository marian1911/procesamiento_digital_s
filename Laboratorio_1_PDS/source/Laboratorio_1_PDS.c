/*
 * Copyright 2016-2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file    Laboratorio_1_PDS.c
 * @brief   Application entry point.
 */

#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_debug_console.h"
#include "arm_math.h"  // Incluye CMSIS-DSP para trabajar con q15_t

/* Declaración de pines */
#define SW2 ((uint32_t)(1<<6)) // switch start/stop
#define SW3 ((uint32_t)(1<<4)) // switch change sample time

/* Declaración de constantes */
#define TICKS_8K  7500U
#define TICKS_16K 3720U
#define TICKS_22K 2700U
#define TICKS_44K 1620U
#define TICKS_48K 1260U

/* Variables globales */
static uint8_t cont_ts = 0;
static uint8_t status = 0;
static uint8_t indice = 0;
static q15_t buffer[256] = {0};  // Ahora es de tipo q15_t
static uint16_t adc_value = 0;

/* Funciones privadas */
static void Toggle_Status(void) {
    status = !status;  // Alterna el estado entre 0 y 1
}

static void Clean_Buffer(void) {
    for (int i = 0; i < 256; i++) {
        buffer[i] = 0;
    }
}

/* Conversión de ADC a Q15 */
static q15_t Convert_ADC_to_Q15(uint16_t adc_value) {
    // Convierte el valor de ADC de 12 bits (0-4095) a Q15 (-32768 a 32767)
    q15_t q15_value = (q15_t)((adc_value - 2048) << 4);  // Centrar en 0 y escalar a 16 bits
    return q15_value;
}

/* Handlers de interrupciones */
/* PIT0_IRQn interrupt handler */
void PIT_CHANNEL_0_IRQHANDLER(void) {
    uint32_t intStatus;

    /* Leer las flags de interrupción */
    intStatus = PIT_GetStatusFlags(PIT_PERIPHERAL, PIT_CHANNEL_0);
    PIT_ClearStatusFlags(PIT_PERIPHERAL, PIT_CHANNEL_0, intStatus);

    /* Conversión de ADC */
    adc_value = ADC16_GetChannelConversionValue(ADC0_BASE, 0);
    q15_t q15_adc_value = Convert_ADC_to_Q15(adc_value);  // Conversión a Q15

    /* Actualización del buffer */
    buffer[indice] = q15_adc_value;
    indice = (indice + 1) % 256;  // Incrementa índice circular

    /* Enviar valor al DAC */
    DAC_SetBufferValue(DAC0_BASE, 0, buffer[indice]);

    /* ARM errata 838869 (Cortex-M4) */
    #if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
    #endif
}

/* PORTA_IRQn interrupt handler */
void GPIOA_IRQHANDLER(void) {
    uint32_t pin_flags = GPIO_PortGetInterruptFlags(GPIOA);

    /* Manejo de interrupción de SW2 */
    if (pin_flags & SW2) {
        Toggle_Status();
        if (status) {
            PIT_Init(PIT_BASE, config);
            PIT_StartTimer(PIT_BASE, PIT_CHANNEL_0);
            Clean_Buffer();
        } else {
            PIT_StopTimer(PIT_BASE, PIT_CHANNEL_0);
            PIT_Deinit(PIT_BASE);
            Clean_Buffer();
            DAC_SetBufferValue(DAC0_BASE, 0, 0);  // Limpia el DAC
        }
    }

    /* Manejo de interrupción de SW3 */
    if (pin_flags & SW3) {
        cont_ts = (cont_ts + 1) % 5;  // Incrementa y reinicia en 0 si es 5

        uint32_t tick_time;
        switch (cont_ts) {
            case 0: tick_time = TICKS_8K;  break;
            case 1: tick_time = TICKS_16K; break;
            case 2: tick_time = TICKS_22K; break;
            case 3: tick_time = TICKS_44K; break;
            case 4: tick_time = TICKS_48K; break;
        }

        PIT_StopTimer(PIT_BASE, PIT_CHANNEL_0);
        PIT_SetTimerPeriod(PIT_BASE, PIT_CHANNEL_0, tick_time);
        PIT_StartTimer(PIT_BASE, PIT_CHANNEL_0);
    }

    /* Limpiar flags de interrupción */
    GPIO_PortClearInterruptFlags(GPIOA, pin_flags);

    /* ARM errata 838869 (Cortex-M4) */
    #if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
    #endif
}

/*
 * @brief   Application entry point.
 */
int main(void) {
    /* Inicialización de hardware de la placa */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();

    /* Bucle principal */
    while (1) {
        ADC16_Init(ADC_BASE, config);  // Esto se suele hacer fuera del loop
    }

    return 0;
}
