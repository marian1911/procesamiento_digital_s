/*
 * Copyright 2016-2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file    Lab_1_PDS.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_debug_console.h"
/* TODO: insert other include files here. */
#define PIN_LED_R 		((uint32_t) (1<<6));

/* TODO: insert other definitions and declarations here. */

 /* PORTA_IRQn interrupt handler */
void GPIOA_IRQHANDLER(void) {
  /* Get pin flags */
  uint32_t pin_flags = GPIO_PortGetInterruptFlags(GPIOA);
  uint32_t pin_flag_Start_Stop = (1<<6);
  /* Place your interrupt code here */

  if(pin_flags & pin_flag_Start_Stop){
	  //GPIO_PinWrite(GPIO_Type, PIN_LED_R, 1);
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
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
#endif

    PRINTF("Hello World\r\n");

    /* Force the counter to be placed into memory. */
    volatile static int i = 0 ;
    /* Enter an infinite loop, just incrementing a counter. */
    while(1) {
        i++ ;
        /* 'Dummy' NOP to allow source level single stepping of
            tight while() loop */
        __asm volatile ("nop");
    }
    return 0 ;
}


