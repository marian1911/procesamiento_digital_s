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

/* TODO: insert other include files here. */

/* TODO: insert other definitions and declarations here. */

/* ---------------------------------------------Declaracion de pines*/
#define sw2 ((uint32_t)(1<<6)) // switch start/stop
#define sw3 ((uint32_t)(1<<4)) // switch change sample time

/* ----------------------------------------Declaracion de constantes*/
#define uint32_t ticks8k 7500
#define uint32_t ticks16k 3720
#define uint32_t ticks22k 2700
#define uint32_t ticks44k 1620
#define uint32_t ticks48k 1260

/* -----------------------------------------------Variables globales*/
uint8_t cont_ts =0;
uint8_t status=0;
uint8_t indice=0;
uint32_t buffer[255]={};
uint16_t adc_value=0;

/* -----------------------------------------------Funciones privadas*/
void Toggle_Status(){
	if(status==1){
		status=0;
	}
	else{
		status=1;
	}
}
void Clean_Buffer(){
	for(int i=0; i<256;i++){
		buffer[i]=0;
	}
}

/* ---------------------------------------Handlers de interrupciones*/
/* PIT0_IRQn interrupt handler */
void PIT_CHANNEL_0_IRQHANDLER(void) {
  uint32_t intStatus;
  /* Reading all interrupt flags of status register */
  intStatus = PIT_GetStatusFlags(PIT_PERIPHERAL, PIT_CHANNEL_0);
  PIT_ClearStatusFlags(PIT_PERIPHERAL, PIT_CHANNEL_0, intStatus);

  /* Place your code here */
  adc_value=ADC16_GetChannelConversionValue(ADC0_BASE, 0);
  adc_value=(adc_value << 4); //rotar 4 izq dac de 12b
  indice++;
  if(indice==256)
	  indice=0;
  buffer[indice]=adc_value;
  DAC_SetBufferValue(DAC0_BASE, 0, buffer[indice]);
  /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F
     Store immediate overlapping exception return operation might vector to incorrect interrupt. */
  #if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
  #endif
}

/* PORTA_IRQn interrupt handler */
void GPIOA_IRQHANDLER(void) {
  /* Get pin flags */
  uint32_t pin_flags = GPIO_PortGetInterruptFlags(GPIOA);
  /* Place your interrupt code here */
  if(pin_flags & sw2){
	  Toggle_Status();
	  if(status==1){
		  PIT_Init(PIT_BASE, config);
		  PIT_StartTimer(PIT_BASE, PIT_CHANNEL_0);
		  Clean_Buffer();
	  }
	  else{
		  PIT_StopTimer(PIT_BASE, PIT_CHANNEL_0);
		  PIT_Deinit(PIT_BASE);
		  Clean_Buffer();
		  DAC_SetBufferValue(DAC0_BASE, 0, 0); // limpia el dac
	  }
  }

  if(pin_flags & sw3){
	  cont_ts++;
	  if(cont_ts==5){
		  cont_ts=0;
      }

	  switch (cont_ts){
	  case 0:
		  PIT_StopTimer(PIT_BASE, PIT_CHANNEL_0);
		  PIT_SetTimerPeriod(PIT_BASE, PIT_CHANNEL_0, ticks8k);
		  PIT_StartTimer(PIT_BASE, PIT_CHANNEL_0);
		  break;
	  case 1:
		  PIT_StopTimer(PIT_BASE, PIT_CHANNEL_0);
		  PIT_SetTimerPeriod(PIT_BASE, PIT_CHANNEL_0, ticks16k);
		  PIT_StartTimer(PIT_BASE, PIT_CHANNEL_0);
	  	  break;
	  case 2:
		  PIT_StopTimer(PIT_BASE, PIT_CHANNEL_0);
		  PIT_SetTimerPeriod(PIT_BASE, PIT_CHANNEL_0, ticks22k);
		  PIT_StartTimer(PIT_BASE, PIT_CHANNEL_0);
	  	  break;
	  case 3:
		  PIT_StopTimer(PIT_BASE, PIT_CHANNEL_0);
		  PIT_SetTimerPeriod(PIT_BASE, PIT_CHANNEL_0, ticks144k);
		  PIT_StartTimer(PIT_BASE, PIT_CHANNEL_0);
	  	  break;
	  case 4:
		  PIT_StopTimer(PIT_BASE, PIT_CHANNEL_0);
		  PIT_SetTimerPeriod(PIT_BASE, PIT_CHANNEL_0, ticks48k);
		  PIT_StartTimer(PIT_BASE, PIT_CHANNEL_0);
	  	  break;
	  }
  }
  /* Clear pin flags */
  GPIO_PortClearInterruptFlags(GPIOA, pin_flags);
  /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F
     Store immediate overlapping exception return operation might vector to incorrect interrupt. */
  #if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
  #endif
}

/*
 * @brief   Application entry point.
 */

int main(void) {
    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();

    while(1) {
    	ADC16_Init(ADC_BASE, config);
    }
    return 0 ;
}
