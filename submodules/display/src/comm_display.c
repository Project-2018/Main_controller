#include "ch.h"
#include "hal.h"
#include "serial_comm.h"
#include "data_display.h"

#define SERIAL_NO_VAL_LEN	10
#define SERIAL_NO_HDR_LEN	14
#define SERIAL_NO_FTR_LEN	 4

uint8_t get_buflen(uint8_t *array){
    uint8_t i=2;
    while (i<MAX_COMMAND_LEN){
      if(array[i-2] == 0xFF && array[i-1] == 0xFF && array[i] == 0xFF){
        return i+1;  
      } 
      i++;
    }
    return 0;
}

void display_send_serial(uint8_t* sn){
	uint8_t frame[SERIAL_NO_VAL_LEN+SERIAL_NO_HDR_LEN+SERIAL_NO_FTR_LEN];
	uint8_t i = 0;
	while(i<SERIAL_NO_HDR_LEN){
		frame[i] = serial_header[i];
		i++;
	}
	while(i<SERIAL_NO_VAL_LEN+SERIAL_NO_HDR_LEN){
		frame[i] = sn[i-SERIAL_NO_HDR_LEN];
		i++;
	}
	while(i<SERIAL_NO_HDR_LEN + SERIAL_NO_VAL_LEN + SERIAL_NO_FTR_LEN){
		frame[i] = serial_footer[i-(SERIAL_NO_HDR_LEN + SERIAL_NO_VAL_LEN)];
		i++;
	}
	sendbuf(frame,i); 
	chThdSleepMilliseconds(5);
}

void display_send(uint8_t* array){
	sendbuf(array,get_buflen(array)); 
	chThdSleepMilliseconds(2);
}

uint8_t* display_get(void){
	return getbuf();
}

void flush_buffer(void){
	zerobuf();
}

