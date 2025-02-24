/*
 * Copyright 2016-2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file    PDS_LAB1.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_debug_console.h"
#include "arm_math.h"
/* TODO: insert other include files here. */

/* TODO: insert other definitions and declarations here. */

#define R_S_BUTTON 	((uint32_t)(1 << 6))			// Boton Run/Stop switch 2 - puerto C pin 6
#define F_CHANGER	((uint32_t)(1 << 4))			// Boton Frec Changer switch 3 - puerto A pin 4
#define Frec_8K		USEC_TO_COUNT(125,PIT_CLK_FREQ)
#define Frec_16K	USEC_TO_COUNT(62.5,PIT_CLK_FREQ)
#define Frec_22K	USEC_TO_COUNT(45.4,PIT_CLK_FREQ)
#define Frec_44K	USEC_TO_COUNT(22.7,PIT_CLK_FREQ)
#define Frec_48K	USEC_TO_COUNT(20.8,PIT_CLK_FREQ)

volatile uint8_t 	Flag_R_S = 0;
volatile uint8_t 	Count_Frec = 0;
volatile q15_t   	Buffer_muestras[512] = {};
volatile uint16_t 	Count_buffer = 0;

/* TODO: insert funcion definitions here. */
void Changer_Frecuency();
void Run_Stop();
/*
 * @brief   Application entry point.
 */
int main(void) {

    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitButtonsPins();
    BOARD_InitLEDsPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();

#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
#endif

    /* Seteo LEDs */
    GPIO_PinWrite(BOARD_LED_RED_GPIO, BOARD_LED_RED_PIN, 1);
    GPIO_PinWrite(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_PIN, 1);
    GPIO_PinWrite(BOARD_LED_BLUE_GPIO, BOARD_LED_BLUE_PIN, 1);

    GPIO_PinWrite(BOARD_LED_RED_GPIO, BOARD_LED_RED_PIN, 0);
    GPIO_PinWrite(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_PIN, 1);
    GPIO_PinWrite(BOARD_LED_BLUE_GPIO, BOARD_LED_BLUE_PIN, 1);

    while(1){

    }
    return 0 ;
}

void Changer_Frecuency(void){

	PIT_StopTimer(PIT_PERIPHERAL, PIT_CHANNEL_0);
	if(Count_Frec == 0){
		PIT_SetTimerPeriod(PIT_PERIPHERAL, PIT_CHANNEL_0, Frec_8K);
		// Color Rojo:
		GPIO_PinWrite(BOARD_LED_RED_GPIO, BOARD_LED_RED_PIN, 0);
		GPIO_PinWrite(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_PIN, 1);
		GPIO_PinWrite(BOARD_LED_BLUE_GPIO, BOARD_LED_BLUE_PIN, 1);

	}else if(Count_Frec == 1){
		PIT_SetTimerPeriod(PIT_PERIPHERAL, PIT_CHANNEL_0, Frec_16K);
		// Color Verde:
		GPIO_PinWrite(BOARD_LED_RED_GPIO, BOARD_LED_RED_PIN, 1);
		GPIO_PinWrite(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_PIN, 0);
		GPIO_PinWrite(BOARD_LED_BLUE_GPIO, BOARD_LED_BLUE_PIN, 1);

	}else if(Count_Frec == 2){
		PIT_SetTimerPeriod(PIT_PERIPHERAL, PIT_CHANNEL_0, Frec_22K);
		// Color Azul:
		GPIO_PinWrite(BOARD_LED_RED_GPIO, BOARD_LED_RED_PIN, 1);
		GPIO_PinWrite(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_PIN, 1);
		GPIO_PinWrite(BOARD_LED_BLUE_GPIO, BOARD_LED_BLUE_PIN, 0);

	}else if(Count_Frec == 3){
		PIT_SetTimerPeriod(PIT_PERIPHERAL, PIT_CHANNEL_0, Frec_44K);
		// Color Amarillo:
		GPIO_PinWrite(BOARD_LED_RED_GPIO, BOARD_LED_RED_PIN, 0);
		GPIO_PinWrite(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_PIN, 0);
		GPIO_PinWrite(BOARD_LED_BLUE_GPIO, BOARD_LED_BLUE_PIN, 1);

	}else if(Count_Frec == 4){
		PIT_SetTimerPeriod(PIT_PERIPHERAL, PIT_CHANNEL_0, Frec_48K);
		// Color Blanco:
		GPIO_PinWrite(BOARD_LED_RED_GPIO, BOARD_LED_RED_PIN, 0);
		GPIO_PinWrite(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_PIN, 0);
		GPIO_PinWrite(BOARD_LED_BLUE_GPIO, BOARD_LED_BLUE_PIN, 0);
	}
	PIT_StartTimer(PIT_PERIPHERAL, PIT_CHANNEL_0);
}

void Run_Stop(void){

	if(Flag_R_S == 0){
		PIT_StartTimer(PIT_PERIPHERAL, PIT_CHANNEL_0);
		Flag_R_S = 1;
	}else if(Flag_R_S == 1){
		PIT_StopTimer(PIT_PERIPHERAL, PIT_CHANNEL_0);
		Flag_R_S = 0;
	}
}

void GPIOA_IRQHANDLER(void){

	uint32_t intStatus = 0;
	intStatus = GPIO_PortGetInterruptFlags(GPIOA_BASE);

	if(intStatus & BOARD_SW3_GPIO_PIN_MASK){
		Count_Frec = (Count_Frec + 1) % 5;
		Changer_Frecuency();
	}

	GPIO_PortClearInterruptFlags(GPIOA_BASE, intStatus);
}

void GPIOC_IRQHANDLER(void){

	uint32_t intStatus = 0;
	intStatus = GPIO_PortGetInterruptFlags(GPIOC_BASE);

	if(intStatus & BOARD_SW2_GPIO_PIN_MASK){
		Run_Stop();
	}

	GPIO_PortClearInterruptFlags(GPIOC_BASE, intStatus);
}

void PIT_CHANNEL_0_IRQHANDLER(void){

	uint32_t intStatus; // 1 Ciclo
	q15_t ADC_Value = 0; // 1 Ciclo

	ADC_Value = (q15_t)(ADC16_GetChannelConversionValue(ADC0_BASE, 0)); // 22 Ciclos
	Buffer_muestras[Count_buffer] =  ADC_Value; // 4 Ciclos

	//Dac_Value = (uint16_t)(ADC_Value);
	DAC_SetBufferValue(DAC0_BASE, 0, Buffer_muestras[Count_buffer]); // 12 Ciclos
	Count_buffer = (Count_buffer + 1) % 512; // 4 Ciclos

	intStatus = PIT_GetStatusFlags(PIT_PERIPHERAL, PIT_CHANNEL_0); // 5 Ciclos
	PIT_ClearStatusFlags(PIT_PERIPHERAL, PIT_CHANNEL_0, intStatus); // 5 Ciclos
	// Total = 53 Ciclos -> 442 nS ---> RUTINA APROBADA
}
