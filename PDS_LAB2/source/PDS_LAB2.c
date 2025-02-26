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

#define Frec_8K		USEC_TO_COUNT(125,PIT_CLK_FREQ)
#define Frec_16K	USEC_TO_COUNT(62.5,PIT_CLK_FREQ)
#define Frec_22K	USEC_TO_COUNT(45.4,PIT_CLK_FREQ)
#define Frec_44K	USEC_TO_COUNT(22.7,PIT_CLK_FREQ)
#define Frec_48K	USEC_TO_COUNT(20.8,PIT_CLK_FREQ)
#define Num_Coef	32

volatile uint8_t 	Flag_Filtro = 0;
volatile uint8_t 	Count_Frec = 0;
		 q15_t   	Buffer_muestras[512] = {0};
volatile uint16_t 	Count_buffer = 0;
		 q15_t		Input_Buffer[512] = {0};
const 	 q15_t 		FIR_Coef1[Num_Coef + 1]  = {50, -62, 81, -102, 110, -83, 0, 161, -413, 758, -1182, 1657, -2143, 2591, -2954, 3190, 29450, 3190, -2954, 2591, -2143, 1657, -1182, 758, -413, 161, 0, -83, 110, -102, 81, -66, 50};
const 	 q15_t 		FIR_Coef2[Num_Coef + 1]  = {-31, 44, 69, -57, -177, 42, 379, 82, -669, -426, 1007, 1173, -1326, -2856, 1555, 10207, 14739, 10207, 1555, -2856, -1326, 1173, 1007, -426, -669, 82, 379, 42, -177, -57, 69, 44, -31};
const	 q15_t 		FIR_Coef3[Num_Coef + 1]  = {-35, 17, 83, 91, -42, -256, -287, 89, 656, 743, -142, -1509, -1861, 183, 4448, 8848, 10718, 8848, 4448, 183, -1861, -1509, -142, 743, 656, 89, -287, -256, -42, 91, 83, 17, -35};
const 	 q15_t 		FIR_Coef4[Num_Coef + 1]  = {48, 61, 68, 49, -21, -158, -344, -518, -580, -413, 71, 895, 1990, 3197, 4297, 5069, 5347, 5069, 4297, 3197, 1990, 895, 71, -413, -580, -518, -344, -158, -21, 49, 68, 61, 48};
const 	 q15_t 		FIR_Coef5[Num_Coef + 1]  = {49, 44, 26, -20, -109, -240, -378, -463, -412, -146, 383, 1170, 2139, 3157, 4058, 4678, 4899, 4678, 4058, 3157, 2139, 1170, 383, -146, -412, -463, -378, -240, -109, -20, 26, 44, 49};
		 q15_t		FIR_State[Num_Coef + 4]  = {0};

/* TODO: instaciacion function here. */
arm_fir_instance_q15 Filtro;

/* TODO: insert funcion definitions here. */
void Changer_Frecuency();
void Filtro_R_S();
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



    arm_fir_init_q15(&Filtro,(uint16_t)Num_Coef, FIR_Coef1, FIR_State, 1);
    PIT_StartTimer(PIT_PERIPHERAL, PIT_CHANNEL_0);

    while(1){

    }
    return 0 ;
}

void Changer_Frecuency(void){

	PIT_StopTimer(PIT_PERIPHERAL, PIT_CHANNEL_0);
	memset(FIR_State, 0, sizeof(FIR_State));
	if(Count_Frec == 0){
		PIT_SetTimerPeriod(PIT_PERIPHERAL, PIT_CHANNEL_0, Frec_8K);
		arm_fir_init_q15(&Filtro, (uint16_t)Num_Coef, FIR_Coef1, FIR_State, 1);
		// Color Rojo:
		GPIO_PinWrite(BOARD_LED_RED_GPIO, BOARD_LED_RED_PIN, 0);
		GPIO_PinWrite(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_PIN, 1);
		GPIO_PinWrite(BOARD_LED_BLUE_GPIO, BOARD_LED_BLUE_PIN, 1);

	}else if(Count_Frec == 1){
		PIT_SetTimerPeriod(PIT_PERIPHERAL, PIT_CHANNEL_0, Frec_16K);
		arm_fir_init_q15(&Filtro, (uint16_t)Num_Coef, FIR_Coef2, FIR_State, 1);
		// Color Verde:
		GPIO_PinWrite(BOARD_LED_RED_GPIO, BOARD_LED_RED_PIN, 1);
		GPIO_PinWrite(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_PIN, 0);
		GPIO_PinWrite(BOARD_LED_BLUE_GPIO, BOARD_LED_BLUE_PIN, 1);

	}else if(Count_Frec == 2){
		PIT_SetTimerPeriod(PIT_PERIPHERAL, PIT_CHANNEL_0, Frec_22K);
		arm_fir_init_q15(&Filtro, (uint16_t)Num_Coef, FIR_Coef3, FIR_State, 1);
		// Color Azul:
		GPIO_PinWrite(BOARD_LED_RED_GPIO, BOARD_LED_RED_PIN, 1);
		GPIO_PinWrite(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_PIN, 1);
		GPIO_PinWrite(BOARD_LED_BLUE_GPIO, BOARD_LED_BLUE_PIN, 0);

	}else if(Count_Frec == 3){
		PIT_SetTimerPeriod(PIT_PERIPHERAL, PIT_CHANNEL_0, Frec_44K);
		arm_fir_init_q15(&Filtro, (uint16_t)Num_Coef, FIR_Coef4, FIR_State, 1);
		// Color Amarillo:
		GPIO_PinWrite(BOARD_LED_RED_GPIO, BOARD_LED_RED_PIN, 0);
		GPIO_PinWrite(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_PIN, 0);
		GPIO_PinWrite(BOARD_LED_BLUE_GPIO, BOARD_LED_BLUE_PIN, 1);

	}else if(Count_Frec == 4){
		PIT_SetTimerPeriod(PIT_PERIPHERAL, PIT_CHANNEL_0, Frec_48K);
		arm_fir_init_q15(&Filtro, Num_Coef, FIR_Coef5, FIR_State, 1);
		// Color Blanco:
		GPIO_PinWrite(BOARD_LED_RED_GPIO, BOARD_LED_RED_PIN, 0);
		GPIO_PinWrite(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_PIN, 0);
		GPIO_PinWrite(BOARD_LED_BLUE_GPIO, BOARD_LED_BLUE_PIN, 0);
	}
	PIT_StartTimer(PIT_PERIPHERAL, PIT_CHANNEL_0);
}

void Filtro_R_S(void){

	if(Flag_Filtro == 0){
		Flag_Filtro = 1;
	}else if(Flag_Filtro == 1){
		Flag_Filtro = 0;
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
		Filtro_R_S();
	}

	GPIO_PortClearInterruptFlags(GPIOC_BASE, intStatus);
}

void PIT_CHANNEL_0_IRQHANDLER(void){

	uint32_t intStatus; // 1 Ciclo
	q15_t ADC_Value; // 1 Ciclo

	ADC_Value = (q15_t)(ADC16_GetChannelConversionValue(ADC0_BASE, 0)); // 22 Ciclos
	Input_Buffer[Count_buffer] = ADC_Value;

	if(Flag_Filtro == 1){
		arm_fir_q15(&Filtro, &Input_Buffer[0]+Count_buffer, &Buffer_muestras[0]+Count_buffer, (uint32_t)1); // 100 ciclos aprox
	}else{
		Buffer_muestras[Count_buffer] = ADC_Value; // 2 ciclos
	}

	DAC_SetBufferValue(DAC0_BASE, 0, Buffer_muestras[Count_buffer]); // 12 Ciclos
	Count_buffer = (Count_buffer + 1) % 512; // 4 Ciclos

	intStatus = PIT_GetStatusFlags(PIT_PERIPHERAL, PIT_CHANNEL_0); // 5 Ciclos
	PIT_ClearStatusFlags(PIT_PERIPHERAL, PIT_CHANNEL_0, intStatus); // 5 Ciclos
	// Total = 53 Ciclos -> 442 nS ---> RUTINA APROBADA
}
