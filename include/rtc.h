#ifndef RTC_H_
#define RTC_H_

#include "hal.h"


void cmd_date(BaseSequentialStream *chp, int argc, char *argv[]);
void GetTimeTm(struct tm *timp);
void SetDateTm(struct tm tim);

time_t GetTimeUnixSec(void);


#endif