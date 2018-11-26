#ifndef RTC_H_
#define RTC_H_

#include "hal.h"


void cmd_date(BaseSequentialStream *chp, int argc, char *argv[]);
void GetTimeTm(struct tm *timp);
void SetDateTm(struct tm tim);

time_t GetTimeUnixSec(void);

uint16_t GetYear(void);
uint8_t GetMonth(void);
uint8_t GetDay(void);
uint8_t GetHour(void);
uint8_t GetMin(void);


#endif