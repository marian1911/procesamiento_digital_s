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
#define sw2 ((uint32_t)(1<<6))
#define sw3 ((uint32_t)(1<<4))

uint8_t cont_ts =0;
uint8_t status=0;
uint8_t ind=0;
uint32_t buffer[255]={};


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



/*
 * @brief   Application entry point.
 */
int main(void) {

    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();

    while(1) {

    }
    return 0 ;
}
