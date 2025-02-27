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
#include "fsl_uart.h"
#include "arm_math.h"
/* TODO: insert other include files here. */

/* TODO: insert other definitions and declarations here. */
#define Frec_8K			USEC_TO_COUNT(125,PIT_CLK_FREQ)
#define Frec_16K		USEC_TO_COUNT(62.5,PIT_CLK_FREQ)
#define Frec_22K		USEC_TO_COUNT(45.4,PIT_CLK_FREQ)
#define Frec_44K		USEC_TO_COUNT(22.7,PIT_CLK_FREQ)
#define Frec_48K		USEC_TO_COUNT(20.8,PIT_CLK_FREQ)
#define FFT_SIZE_MAX	2048
#define FFT_SIZE_512	512
#define FFT_SIZE_1024	1024
#define FFT_SIZE_2048	2048
#define UART_BAUDRATE  	115200

volatile uint8_t 	Flag_R_S = 0;
volatile uint8_t	Flag_FFT = 0;
volatile uint8_t 	Count_Frec = 0;
volatile uint8_t	Count_FFT = 0;
volatile uint16_t	FFT_Size = FFT_SIZE_512;
volatile q15_t   	Buffer_muestras[FFT_SIZE_MAX] = {0};
volatile uint16_t 	Count_buffer = 0;
		 q15_t		FFT_Input_Buffer[FFT_SIZE_MAX * 2] = {0};
		 q15_t		FFT_Output_Buffer[FFT_SIZE_MAX + 2] = {0};

/* TODO: insert funcion definitions here. */
void Changer_Frecuency();
void Reset_Buffers();
void UART0_SendData(uint16_t *data, uint16_t length);

/* TODO: insert instances here. */
arm_rfft_instance_q15 FFT_Instance;

/*
 * @brief   Application entry point.
 */
int main(void) {

    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitButtonsPins();
    BOARD_InitDEBUG_UARTPins();
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

    arm_rfft_init_q15(&FFT_Instance, FFT_Size, 0, 1);

    PIT_StartTimer(PIT_PERIPHERAL, PIT_CHANNEL_0);

    while(1){
    	if(Flag_FFT == 1){
    		arm_rfft_q15(&FFT_Instance, FFT_Input_Buffer, FFT_Output_Buffer);
    		UART0_SendData((uint16_t *)FFT_Output_Buffer, FFT_Size);
    		Flag_FFT = 0;
    	}
    }
    return 0 ;
}

void UART0_SendData(uint16_t *data, uint16_t length) {

    for(uint16_t i = 0; i < length; i++) {

        uint8_t lowByte = data[i] & 0xFF;
        uint8_t highByte = (data[i] >> 8) & 0xFF;
        UART_WriteBlocking(UART0, &lowByte, 1);
        UART_WriteBlocking(UART0, &highByte, 1);

    }
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

void Reset_Buffers(void){

	Count_buffer = 0;
	memset(FFT_Input_Buffer, 0, sizeof(q15_t) * FFT_SIZE_MAX * 2);  	// Limpiar fft_input
	memset(FFT_Output_Buffer, 0, sizeof(q15_t) * (FFT_SIZE_MAX + 2));  // Limpiar fft_output
	memset(Buffer_muestras, 0, sizeof(q15_t) * FFT_SIZE_MAX);

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

		Reset_Buffers();

		Count_FFT = (Count_FFT + 1) % 4;

		if(Count_FFT == 0){
			Flag_R_S = 0;

		}else if(Count_FFT == 1){
			Flag_R_S = 1;
			FFT_Size = FFT_SIZE_512;
			arm_rfft_init_q15(&FFT_Instance, FFT_Size, 0, 1);

		}else if(Count_FFT == 2){
			Flag_R_S = 1;
			FFT_Size = FFT_SIZE_1024;
			arm_rfft_init_q15(&FFT_Instance, FFT_Size, 0, 1);

		}else if(Count_FFT == 3){
			Flag_R_S = 1;
			FFT_Size = FFT_SIZE_2048;
			arm_rfft_init_q15(&FFT_Instance, FFT_Size, 0, 1);
		}
	}

	GPIO_PortClearInterruptFlags(GPIOC_BASE, intStatus);
}

void PIT_CHANNEL_0_IRQHANDLER(void){

	uint32_t intStatus; // 1 Ciclo
	q15_t ADC_Value = 0; // 1 Ciclo

	ADC_Value = (q15_t)(ADC16_GetChannelConversionValue(ADC0_BASE, 0)); // 22 Ciclos
	Buffer_muestras[Count_buffer] =  ADC_Value; // 4 Ciclos

	if(Flag_R_S == 1){

		FFT_Input_Buffer[2 * Count_buffer] = Buffer_muestras[Count_buffer];
		FFT_Input_Buffer[2 * Count_buffer + 1] = 0;

		if(Count_buffer >= (FFT_Size - 1)){
			Flag_FFT = 1;
		}
	}
	Count_buffer = (Count_buffer + 1) % FFT_Size; // 4 Ciclos

	intStatus = PIT_GetStatusFlags(PIT_PERIPHERAL, PIT_CHANNEL_0); // 5 Ciclos
	PIT_ClearStatusFlags(PIT_PERIPHERAL, PIT_CHANNEL_0, intStatus); // 5 Ciclos
	// Total = 53 Ciclos -> 442 nS ---> RUTINA APROBADA
}

