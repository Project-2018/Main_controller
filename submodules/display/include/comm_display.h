#ifndef COMM_DISPLAY_H_INCLUDED
#define COMM_DISPLAY_H_INCLUDED

uint8_t get_buflen(uint8_t *arrray);


void flush_buffer(void);
void display_send_serial(uint8_t* sn);
void display_send(uint8_t* array);
uint8_t* display_get(void);





#endif