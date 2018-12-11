/*
    ChibiOS - Copyright (C) 2006..2016 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License@    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include "ch.h"
#include "hal.h"
#include "shell.h"
#include "chprintf.h"
#include "stdlib.h"
#include "console.h"

#define UART_DEV UARTD1
#define BUFLEN 8

//static const uint8_t message[] = "0123456789ABCDEF";
static uint8_t buffer[BUFLEN];

// This callback is invoked when a transmission buffer has been completely
// read by the driver.
static void txend1(UARTDriver *uartp) {
	(void)uartp;
}

// This callback is invoked when a transmission has physically completed.
static void txend2(UARTDriver *uartp) {
	(void)uartp;
}

// This callback is invoked on a receive error, the errors mask is passed
// as parameter.
static void rxerr(UARTDriver *uartp, uartflags_t e) {
	(void)uartp;
	(void)e;
}

// This callback is invoked when a character is received but the application
// was not ready to receive it, the character is passed as parameter.
static void rxchar(UARTDriver *uartp, uint16_t c) {
	(void)c;
	(void)uartp;
}
	
// This callback is invoked when a receive buffer has been completely written.
static void rxend(UARTDriver *uartp) {
	(void)uartp;
}
	
// UART driver configuration structure.
static UARTConfig uart_cfg_1 = {
	txend1,
	txend2,
	rxend,
	rxchar,
	rxerr,
	115200,
	0,
	USART_CR2_LINEN,
	0
};

static THD_WORKING_AREA(SerialCommTask_wa, 128);
static THD_FUNCTION(SerialCommTask, p) {
 	(void)p;
	chRegSetThreadName("SerialComm task");
	while (TRUE) {
	    // Starts both a transmission and a receive operations, both will be
	    // handled entirely in background.
	    uartStopReceive(&UART_DEV);
	    uartStartReceive(&UART_DEV, BUFLEN, buffer);
	    chThdSleepMilliseconds(20);
	}
}

uint8_t *getbuf (void){
	return buffer;
}

void zerobuf(void){
	int i = 0; 
	for(i=0;i<BUFLEN;i++){
		buffer[i]=0x00;
	}
}

void sendbuf (uint8_t* array, uint8_t arraysz){
	uartStopSend(&UART_DEV);
	if(array != NULL && arraysz > 0)
	//;
 	uartStartSend(&UART_DEV, arraysz, array);
}

void initSerialComm(void){
	//uartStart(&UARTD1, &uart_cfg_1);
	uartStart(&UART_DEV, &uart_cfg_1);
	chThdCreateStatic(SerialCommTask_wa, sizeof(SerialCommTask_wa), NORMALPRIO + 5, SerialCommTask, NULL);
}



