#include "ch.h"
#include "rtc.h"
#include "chprintf.h"
#include <stdlib.h>
#include <string.h>


static RTCDateTime timespec;

time_t GetTimeUnixSec(void) {
  struct tm tim;

  rtcGetTime(&RTCD1, &timespec);
  rtcConvertDateTimeToStructTm(&timespec, &tim, NULL);
  return mktime(&tim);
}

void GetTimeTm(struct tm *timp) {
  rtcGetTime(&RTCD1, &timespec);
  rtcConvertDateTimeToStructTm(&timespec, timp, NULL);
}

uint16_t GetYear(void){
  struct tm timp;
  GetTimeTm(&timp);
  uint16_t year = (uint16_t)(timp.tm_year + 1900);
  return year;
}

uint8_t GetMonth(void){
  struct tm timp;
  GetTimeTm(&timp);
  uint8_t month = (uint8_t)timp.tm_mon;
  return month + 1;
}

uint8_t GetDay(void){
  struct tm timp;
  GetTimeTm(&timp);
  uint8_t day = (uint8_t)timp.tm_mday;
  return day;
}

uint8_t GetHour(void){
  struct tm timp;
  GetTimeTm(&timp);
  uint8_t hour = (uint8_t)timp.tm_hour;
  return hour;
}

uint8_t GetMin(void){
  struct tm timp;
  GetTimeTm(&timp);
  uint8_t min = (uint8_t)timp.tm_min;
  return min;
}

void SetTimeUnixSec(time_t unix_time) {
  struct tm tim;
  struct tm *canary;

  /* If the conversion is successful the function returns a pointer
     to the object the result was written into.*/
  canary = localtime_r(&unix_time, &tim);
  osalDbgCheck(&tim == canary);

  rtcConvertStructTmToDateTime(&tim, 0, &timespec);
  rtcSetTime(&RTCD1, &timespec);
}

void SetDateTm(struct tm tim){
  rtcConvertStructTmToDateTime(&tim, 0, &timespec);
  rtcSetTime(&RTCD1, &timespec);  
}

/*
 * Console applet for date set and get
 */
void cmd_date(BaseSequentialStream *chp, int argc, char *argv[]) {
  (void)argv;
  struct tm timp = {0};
  time_t unix_time;

  if (argc == 0) {
    chprintf(chp, "%d %d %d, %d %d", GetYear(), GetMonth(), GetDay(), GetHour(), GetMin());


    goto ERROR;
  }

  if ((argc == 1) && (strcmp(argv[0], "get") == 0)){
    unix_time = GetTimeUnixSec();

    if (unix_time == -1){
      chprintf(chp, "incorrect time in RTC cell\r\n");
    }
    else{
      chprintf(chp, "%D%s", unix_time, "\r\n");
      GetTimeTm(&timp);
      chprintf(chp, "%s", asctime(&timp));
    }
    return;
  }

  if ((argc == 2) && (strcmp(argv[0], "set") == 0)){
    unix_time = atol(argv[1]);
    if (unix_time > 0){
      SetTimeUnixSec(unix_time);
      return;
    }
    else{
      goto ERROR;
    }
  }
  else{
    goto ERROR;
  }

ERROR:
  chprintf(chp, "Usage: date get\r\n");
  chprintf(chp, "       date set N\r\n");
  chprintf(chp, "where N is time in seconds sins Unix epoch\r\n");
  chprintf(chp, "you can get current N value from unix console by the command\r\n");
  chprintf(chp, "%s", "date +\%s\r\n");
  return;
}
