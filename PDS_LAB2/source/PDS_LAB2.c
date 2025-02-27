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
#define BLOCK_SIZE	(uint32_t)1

volatile 	uint8_t 	Count_Filtro = 0;
volatile 	uint8_t 	Count_Frec = 0;
volatile 	uint16_t 	Count_buffer = 0;
		 	q15_t   	Buffer_muestras[512] = {0};
		 	q15_t		Input_Buffer[512] = {0};

/* Parametros filtro pasa bajo: */
const 		q15_t 		FIR_PB_Coef1[Num_Coef + 1] = {50, -62, 81, -102, 110, -83, 0, 161, -413, 758, -1182, 1657, -2143, 2591, -2954, 3190, 29450, 3190, -2954, 2591, -2143, 1657, -1182, 758, -413, 161, 0, -83, 110, -102, 81, -62, 50};
const 		q15_t 		FIR_PB_Coef2[Num_Coef + 1] = {-31, 44, 69, -57, -177, 42, 379, 82, -669, -426, 1007, 1173, -1326, -2856, 1555, 10207, 14739, 10207, 1555, -2856, -1326, 1173, 1007, -426, -669, 82, 379, 42, -177, -57, 69, 44, -31};
const 		q15_t 		FIR_PB_Coef3[Num_Coef + 1] = {-50, -57, -39, 30, 151, 270, 269, 41, -415, -915, -1112, -637, 699, 2741, 4982, 6730, 7390, 6730, 4982, 2741, 699, -637, -1112, -915, -415, 41, 269, 270, 151, 30, -39, -57, -50};
const 		q15_t 		FIR_PB_Coef4[Num_Coef + 1] = {48, 61, 68, 49, -21, -158, -344, -518, -580, -413, 71, 895, 1990, 3197, 4297, 5069, 5347, 5069, 4297, 3197, 1990, 895, 71, -413, -580, -518, -344, -158, -21, 49, 68, 61, 48};
const 		q15_t 		FIR_PB_Coef5[Num_Coef + 1] = {49, 44, 26, -20, -109, -240, -378, -463, -412, -146, 383, 1170, 2139, 3157, 4058, 4678, 4899, 4678, 4058, 3157, 2139, 1170, 383, -146, -412, -463, -378, -240, -109, -20, 26, 44, 49};
		 	q15_t		FIR_PB_State[Num_Coef + 4]  = {0};

/* Parametros filtro pasa alto: */
const 		q15_t 		FIR_PA_Coef1[Num_Coef + 1] = {0, 44, 86, 89, 0, -191, -380, -369, 0, 664, 1246, 1174, 0, -2269, -5036, -7315, 24591, -7315, -5036, -2269, 0, 1174, 1246, 664, 0, -369, -380, -191, 0, 89, 86, 44, 0};
const 		q15_t 		FIR_PA_Coef2[Num_Coef + 1] = {0, 24, 61, 117, 187, 249, 269, 200, 0, -359, -880, -1534, -2257, -2964, -3560, -3957, 28681, -3957, -3560, -2964, -2257, -1534, -880, -359, 0, 200, 269, 249, 187, 117, 61, 24, 0};
const 		q15_t 		FIR_PA_Coef3[Num_Coef + 1] = {0, -12, -33, -70, -132, -224, -351, -512, -704, -920, -1150, -1380, -1595, -1782, -1926, -2017, 30715, -2017, -1926, -1782, -1595, -1380, -1150, -920, -704, -512, -351, -224, -132, -70, -33, -12, 0};
const 		q15_t 		FIR_PA_Coef4[Num_Coef + 1] = {-39, -52, -78, -121, -185, -270, -376, -501, -641, -790, -942, -1088, -1221, -1334, -1419, -1473, 31313, -1473, -1419, -1334, -1221, -1088, -942, -790, -641, -501, -376, -270, -185, -121, -78, -52, -39};
const 		q15_t 		FIR_PA_Coef5[Num_Coef + 1] = {-45, -57, -83, -125, -187, -268, -367, -483, -611, -745, -881, -1012, -1130, -1229, -1304, -1351, 31443, -1351, -1304, -1229, -1130, -1012, -881, -745, -611, -483, -367, -268, -187, -125, -83, -57, -45};
			q15_t		FIR_PA_State[Num_Coef + 4] = {0};

/* Parametros filtro pasa banda: */
const 		q15_t 		FIR_PBand_Coef1[Num_Coef + 1] = {50, -18, 167, -13, 110, -274, -379, -208, -414, 1422, 61, 2831, -2144, 327, -7986, -4113, 21286, -4113, -7986, 327, -2144, 2831, 61, 1422, -414, -208, -379, -274, 110, -13, 167, -18, 50};
const 		q15_t 		FIR_PBand_Coef2[Num_Coef + 1] = {-31, 67, 130, 59, 9, 291, 647, 281, -669, -784, 127, -359, -3578, -5814, -2001, 6247, 10637, 6247, -2001, -5814, -3578, -359, 127, -784, -669, 281, 647, 291, 9, 59, 130, 67, -31};
const 		q15_t 		FIR_PBand_Coef3[Num_Coef + 1] = {16, 73, 147, 159, 10, -256, -393, -193, 124, -110, -1371, -3145, -3905, -2235, 1726, 5926, 7726, 5926, 1726, -2235, -3905, -3145, -1371, -110, 124, -193, -393, -256, 10, 159, 147, 73, 16};
const 		q15_t 		FIR_PBand_Coef4[Num_Coef + 1] = {11, 11, -12, -88, -252, -525, -883, -1250, -1496, -1474, -1065, -232, 950, 2295, 3542, 4425, 4744, 4425, 3542, 2295, 950, -232, -1065, -1474, -1496, -1250, -883, -525, -252, -88, -12, 11, 11};
const 		q15_t 		FIR_PBand_Coef5[Num_Coef + 1] = {6, -17, -73, -188, -384, -657, -966, -1226, -1325, -1154, -641, 211, 1317, 2512, 3584, 4329, 4595, 4329, 3584, 2512, 1317, 211, -641, -1154, -1325, -1226, -966, -657, -384, -188, -73, -17, 6};
			q15_t		FIR_PBand_State[Num_Coef + 4] = {0};

/* Parametros filtro rechaza banda: */
const 		q15_t 		FIR_RB_Coef1[Num_Coef + 1] = {-50, 18, -168, 13, -110, 275, 381, 209, 415, -1428, -61, -2844, 2154, -328, 8021, 4131, 11512, 4131, 8021, -328, 2154, -2844, -61, -1428, 415, 209, 381, 275, -110, 13, -168, 18, -50};
const 		q15_t 		FIR_RB_Coef2[Num_Coef + 1] = {31, -67, -129, -59, -9, -290, -646, -280, 667, 782, -126, 358, 3570, 5800, 1996, -6232, 22039, -6232, 1996, 5800, 3570, 358, -126, 782, 667, -280, -646, -290, -9, -59, -129, -67, 31};
const 		q15_t 		FIR_RB_Coef3[Num_Coef + 1] = {-17, -76, -152, -163, -11, 264, 405, 198, -127, 114, 1412, 3239, 4022, 2302, -1778, -6104, 25710, -6104, -1778, 2302, 4022, 3239, 1412, 114, -127, 198, 405, 264, -11, -163, -152, -76, -17};
const 		q15_t 		FIR_RB_Coef4[Num_Coef + 1] = {-13, -13, 15, 105, 301, 625, 1053, 1491, 1784, 1758, 1270, 277, -1133, -2736, -4223, -5276, 32767, -5276, -4223, -2736, -1133, 277, 1270, 1758, 1784, 1491, 1053, 625, 301, 105, 15, -13, -13};
const 		q15_t 		FIR_RB_Coef5[Num_Coef + 1] = {-7, 21, 88, 227, 463, 793, 1165, 1479, 1598, 1392, 774, -255, -1589, -3030, -4324, -5222, 32767, -5222, -4324, -3030, -1589, -255, 774, 1392, 1598, 1479, 1165, 793, 463, 227, 88, 21, -7};
			q15_t		FIR_RB_State[Num_Coef + 4] = {0};

/* TODO: instaciacion function here. */

arm_fir_instance_q15 F_PB;
arm_fir_instance_q15 F_PA;
arm_fir_instance_q15 F_PBand;
arm_fir_instance_q15 F_RB;

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



    arm_fir_init_q15(&F_PB,(uint16_t)Num_Coef, FIR_PB_Coef1, FIR_PB_State, BLOCK_SIZE);
    arm_fir_init_q15(&F_PA,(uint16_t)Num_Coef, FIR_PA_Coef1, FIR_PA_State, BLOCK_SIZE);
    arm_fir_init_q15(&F_PBand, (uint16_t)Num_Coef, FIR_PBand_Coef1, FIR_PBand_State, BLOCK_SIZE);
    arm_fir_init_q15(&F_RB,(uint16_t)Num_Coef, FIR_RB_Coef1, FIR_RB_State, BLOCK_SIZE);

    PIT_StartTimer(PIT_PERIPHERAL, PIT_CHANNEL_0);

    while(1){

    }
    return 0 ;
}

void Changer_Frecuency(void){

	PIT_StopTimer(PIT_PERIPHERAL, PIT_CHANNEL_0);

	memset(FIR_PB_State, 0, sizeof(FIR_PB_State));
	memset(FIR_PA_State, 0, sizeof(FIR_PA_State));
	memset(FIR_PBand_State, 0, sizeof(FIR_PBand_State));
	memset(FIR_RB_State, 0, sizeof(FIR_RB_State));

	if(Count_Frec == 0){

		PIT_SetTimerPeriod(PIT_PERIPHERAL, PIT_CHANNEL_0, Frec_8K);

		arm_fir_init_q15(&F_PB, (uint16_t)Num_Coef, FIR_PB_Coef1, FIR_PB_State, BLOCK_SIZE);
		arm_fir_init_q15(&F_PA,(uint16_t)Num_Coef, FIR_PA_Coef1, FIR_PA_State, BLOCK_SIZE);
		arm_fir_init_q15(&F_PBand, (uint16_t)Num_Coef, FIR_PBand_Coef1, FIR_PBand_State, BLOCK_SIZE);
		arm_fir_init_q15(&F_RB,(uint16_t)Num_Coef, FIR_RB_Coef1, FIR_RB_State, BLOCK_SIZE);
		// Color Rojo:
		GPIO_PinWrite(BOARD_LED_RED_GPIO, BOARD_LED_RED_PIN, 0);
		GPIO_PinWrite(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_PIN, 1);
		GPIO_PinWrite(BOARD_LED_BLUE_GPIO, BOARD_LED_BLUE_PIN, 1);

	}else if(Count_Frec == 1){

		PIT_SetTimerPeriod(PIT_PERIPHERAL, PIT_CHANNEL_0, Frec_16K);

		arm_fir_init_q15(&F_PB, (uint16_t)Num_Coef, FIR_PB_Coef2, FIR_PB_State, BLOCK_SIZE);
		arm_fir_init_q15(&F_PA,(uint16_t)Num_Coef, FIR_PA_Coef2, FIR_PA_State, BLOCK_SIZE);
		arm_fir_init_q15(&F_PBand, (uint16_t)Num_Coef, FIR_PBand_Coef2, FIR_PBand_State, BLOCK_SIZE);
		arm_fir_init_q15(&F_RB,(uint16_t)Num_Coef, FIR_RB_Coef2, FIR_RB_State, BLOCK_SIZE);
		// Color Verde:
		GPIO_PinWrite(BOARD_LED_RED_GPIO, BOARD_LED_RED_PIN, 1);
		GPIO_PinWrite(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_PIN, 0);
		GPIO_PinWrite(BOARD_LED_BLUE_GPIO, BOARD_LED_BLUE_PIN, 1);

	}else if(Count_Frec == 2){

		PIT_SetTimerPeriod(PIT_PERIPHERAL, PIT_CHANNEL_0, Frec_22K);

		arm_fir_init_q15(&F_PB, (uint16_t)Num_Coef, FIR_PB_Coef3, FIR_PB_State, BLOCK_SIZE);
		arm_fir_init_q15(&F_PA,(uint16_t)Num_Coef, FIR_PA_Coef3, FIR_PA_State, BLOCK_SIZE);
		arm_fir_init_q15(&F_PBand, (uint16_t)Num_Coef, FIR_PBand_Coef3, FIR_PBand_State, BLOCK_SIZE);
		arm_fir_init_q15(&F_RB,(uint16_t)Num_Coef, FIR_RB_Coef3, FIR_RB_State, BLOCK_SIZE);
		// Color Azul:
		GPIO_PinWrite(BOARD_LED_RED_GPIO, BOARD_LED_RED_PIN, 1);
		GPIO_PinWrite(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_PIN, 1);
		GPIO_PinWrite(BOARD_LED_BLUE_GPIO, BOARD_LED_BLUE_PIN, 0);

	}else if(Count_Frec == 3){

		PIT_SetTimerPeriod(PIT_PERIPHERAL, PIT_CHANNEL_0, Frec_44K);

		arm_fir_init_q15(&F_PB, (uint16_t)Num_Coef, FIR_PB_Coef4, FIR_PB_State, BLOCK_SIZE);
		arm_fir_init_q15(&F_PA,(uint16_t)Num_Coef, FIR_PA_Coef4, FIR_PA_State, BLOCK_SIZE);
		arm_fir_init_q15(&F_PBand, (uint16_t)Num_Coef, FIR_PBand_Coef4, FIR_PBand_State, BLOCK_SIZE);
		arm_fir_init_q15(&F_RB,(uint16_t)Num_Coef, FIR_RB_Coef4, FIR_RB_State, BLOCK_SIZE);
		// Color Amarillo:
		GPIO_PinWrite(BOARD_LED_RED_GPIO, BOARD_LED_RED_PIN, 0);
		GPIO_PinWrite(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_PIN, 0);
		GPIO_PinWrite(BOARD_LED_BLUE_GPIO, BOARD_LED_BLUE_PIN, 1);

	}else if(Count_Frec == 4){

		PIT_SetTimerPeriod(PIT_PERIPHERAL, PIT_CHANNEL_0, Frec_48K);

		arm_fir_init_q15(&F_PB, (uint16_t)Num_Coef, FIR_PB_Coef5, FIR_PB_State, BLOCK_SIZE);
		arm_fir_init_q15(&F_PA,(uint16_t)Num_Coef, FIR_PA_Coef5, FIR_PA_State, BLOCK_SIZE);
		arm_fir_init_q15(&F_PBand, (uint16_t)Num_Coef, FIR_PBand_Coef5, FIR_PBand_State, BLOCK_SIZE);
		arm_fir_init_q15(&F_RB,(uint16_t)Num_Coef, FIR_RB_Coef5, FIR_RB_State, BLOCK_SIZE);
		// Color Blanco:
		GPIO_PinWrite(BOARD_LED_RED_GPIO, BOARD_LED_RED_PIN, 0);
		GPIO_PinWrite(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_PIN, 0);
		GPIO_PinWrite(BOARD_LED_BLUE_GPIO, BOARD_LED_BLUE_PIN, 0);
	}

	PIT_StartTimer(PIT_PERIPHERAL, PIT_CHANNEL_0);
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

		memset(FIR_PB_State, 0, sizeof(FIR_PB_State));
		memset(FIR_PA_State, 0, sizeof(FIR_PA_State));
		memset(FIR_PBand_State, 0, sizeof(FIR_PBand_State));
		memset(FIR_RB_State, 0, sizeof(FIR_RB_State));

		Count_buffer = 0;
		memset(Input_Buffer, 0, sizeof(Input_Buffer));
		memset(Buffer_muestras, 0, sizeof(Buffer_muestras));

		Count_Filtro = (Count_Filtro + 1) % 5;
	}

	GPIO_PortClearInterruptFlags(GPIOC_BASE, intStatus);
}

void PIT_CHANNEL_0_IRQHANDLER(void){

	uint32_t intStatus; // 1 Ciclo
	q15_t ADC_Value; // 1 Ciclo

	intStatus = PIT_GetStatusFlags(PIT_PERIPHERAL, PIT_CHANNEL_0); // 5 Ciclos

	ADC_Value = (q15_t)(ADC16_GetChannelConversionValue(ADC0_BASE, 0)); // 22 Ciclos
	Input_Buffer[Count_buffer] = ADC_Value;

	switch(Count_Filtro)
	{
	case 0:
		Buffer_muestras[Count_buffer] = Input_Buffer[Count_buffer];
		break;
	case 1:
		arm_fir_q15(&F_PB, &Input_Buffer[Count_buffer], &Buffer_muestras[Count_buffer], BLOCK_SIZE);
		break;
	case 2:
		arm_fir_q15(&F_PA, &Input_Buffer[Count_buffer], &Buffer_muestras[Count_buffer], BLOCK_SIZE);
		break;
	case 3:
		arm_fir_q15(&F_PBand, &Input_Buffer[Count_buffer], &Buffer_muestras[Count_buffer], BLOCK_SIZE);
		break;
	case 4:
		arm_fir_q15(&F_RB, &Input_Buffer[Count_buffer], &Buffer_muestras[Count_buffer], BLOCK_SIZE);
		break;
	}

	DAC_SetBufferValue(DAC0_BASE, 0, Buffer_muestras[Count_buffer]); // 12 Ciclos
	Count_buffer = (Count_buffer + 1) % 512; // 4 Ciclos

	PIT_ClearStatusFlags(PIT_PERIPHERAL, PIT_CHANNEL_0, intStatus); // 5 Ciclos
}
