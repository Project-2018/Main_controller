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
