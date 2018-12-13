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

#include "data_display.h"
#include "screenhandler.h"
#include "serial_comm.h"
#include "screeninfo_cfg.h"
#include "comm_display.h"

#include "test_display.h"
#include "rtc.h"
#include "esc_comm.h"


#define TIMING_TASK   20
#define TIMING_1     100
#define TIMING_2     250
#define TIMING_3   10000

#define YEAR_MAX    2038
#define YEAR_MIN    1970
#define YEAR_LEAP   1968

#define LBSMUL    2.20462

screen_variables screenvar;
screen_variables localvar;

bool overtemp_flag    = false;
bool overtilt_flag    = false;
bool overweight_flag  = false;
bool charge_flag      = false;

bool datetime_changed = false;
bool spd_changed      = false;
bool measunit_changed = false;


void refresh_serial_no(void){
    screenvar.serial_number = FetchSerialnumber();
    display_send_serial(screenvar.serial_number);
}

void refresh_bat(void){
  uint8_t bat_level=0;
  if(screenvar.battery_level > 90)  bat_level = 10;
  else if(screenvar.battery_level > 80)  bat_level = 9;
  else if(screenvar.battery_level > 70)  bat_level = 8;
  else if(screenvar.battery_level > 60)  bat_level = 7;
  else if(screenvar.battery_level > 50)  bat_level = 6;
  else if(screenvar.battery_level > 40)  bat_level = 5;
  else if(screenvar.battery_level > 30)  bat_level = 4;
  else if(screenvar.battery_level > 20)  bat_level = 3;
  else if(screenvar.battery_level > 10)  bat_level = 2;
  else if(screenvar.battery_level >  5)  bat_level = 1;
  else bat_level=0;
  display_send(&battery[bat_level]);
}

void refresh_chg_remaining(void){
    if(screenvar.ischarging == true){
      uint16_t chg_h_rem = 0;
      uint16_t chg_m_rem = 0;
      

      if(screenvar.charge_remaining !=0){
        chg_h_rem = screenvar.charge_remaining / 60;
        if(chg_h_rem > 99) 
          chg_h_rem = 99;
        chg_m_rem = screenvar.charge_remaining - (chg_h_rem * 60);
      }
      //display_send(chg_remaining_hour[chg_h_rem]);
      //display_send(chg_remaining_min[chg_m_rem]);
    }
}

bool bufcmp(uint8_t* array1, uint8_t* array2){ //compare incoming buffer with one of the arrays
    int i = 0;
    while (i < (sizeof(array1)/sizeof(uint8_t))){
      if(array1[i] == array2[i]){
        i++;  
      } else {
        break;
      }
    }
    if(i == (sizeof(array1)/sizeof(uint8_t))){
      return true;
    }
    return false;
}

//This function watches the inputs and returns the correct event happened eg. touch on screen, tilt on the forklift, etc.
void StateMachine_GetEvent(void) {
    uint8_t *cb = getbuf();

    //Charge event (L1 -Most important)
    
    if(screenvar.ischarging == true && charge_flag == false){
      charge_flag = true;
      display_send(&screen_charge);
      //sendbuf(&screen_charge,10);
    }

    if(screenvar.ischarging == false && charge_flag == true){
      charge_flag = false;
           if (overtemp_flag == true)   display_send(&screen_overtemp_warn);
      else if (overtilt_flag == true)   display_send(&screen_overtilt_warn);
      else if (overweight_flag == true) display_send(&screen_overload_warn);
      else                              display_send(&screen_main);
 
      
    }
    //Overtemperature event (L2)
    if(screenvar.overtemp == true && overtemp_flag == false && screenvar.ischarging == false){
      overtemp_flag = true;
      display_send(&screen_overtemp_warn);
    }

    if(screenvar.overtemp == false && overtemp_flag == true){
      overtemp_flag = false;
           if (charge_flag == true)     display_send(&screen_charge);
      else if (overtilt_flag == true)   display_send(&screen_overtilt_warn);
      else if (overweight_flag == true) display_send(&screen_overload_warn);
      else                              display_send(&screen_main);
    }
    //Overtilt event (L3)
    if(screenvar.overtilt == true && overtilt_flag == false && screenvar.ischarging == false && screenvar.overtemp == false){
      overtilt_flag = true;
      display_send(&overtilt_on);
      display_send(&screen_overtilt_warn);
    }

    if(screenvar.overtilt == false && overtilt_flag == true){
      overtilt_flag = false;
      display_send(&overtilt_off);
           if (charge_flag == true)     display_send(&screen_charge);
      else if (overtemp_flag == true)   display_send(&screen_overtemp_warn);
      else if (overweight_flag == true) display_send(&screen_overload_warn);
      else                              display_send(&screen_main);
    }

    //Overload event (L4 -Least important)
    if(screenvar.overweight == true && overweight_flag == false && screenvar.ischarging == false && screenvar.overtemp == false && screenvar.overtilt == false){
      overweight_flag = true;
      display_send(&screen_overload_warn);
    }

    if(screenvar.overweight == false && overweight_flag == true){
      overweight_flag = false;
           if (charge_flag == true)     display_send(&screen_charge);
      else if (overtemp_flag == true)   display_send(&screen_overtemp_warn);
      else if (overtilt_flag == true)   display_send(&screen_overtilt_warn);
      else                              display_send(&screen_main);
    }

    if(cb[0] != 0x00){
      //main screen related events
       if (bufcmp(cb, rxbutton_rabb)){
        flush_buffer();
        if(screenvar.lift_speed < 2) {
          screenvar.lift_speed++;
        } else {
          screenvar.lift_speed = 2;
        }
        spd_changed = true;
      }

      else if (bufcmp(cb, rxbutton_turt)){
        flush_buffer();
        if(screenvar.lift_speed > 0) {
          screenvar.lift_speed--;
        } else {
          screenvar.lift_speed = 0;
        }
        spd_changed = true;
      }

      //configuration related
      else if (bufcmp(cb, rxbutton_config_date_yyyy_plus)){
        flush_buffer();
        if(screenvar.date_yyyy < YEAR_MAX){
          screenvar.date_yyyy++;
          datetime_changed = true;
        }

      }

      else if (bufcmp(cb, rxbutton_config_date_yyyy_minus)){
        flush_buffer();
        if(screenvar.date_yyyy > YEAR_MIN){
          screenvar.date_yyyy--;
          datetime_changed = true;
        }
      }

      else if (bufcmp(cb, rxbutton_config_date_mm_plus)){
        flush_buffer();
        if(screenvar.date_mm < 12)  
          screenvar.date_mm++;
          datetime_changed = true;
      }

      else if (bufcmp(cb, rxbutton_config_date_mm_minus)){
        flush_buffer();
        if(screenvar.date_mm > 1)
          screenvar.date_mm--;
          datetime_changed = true;
      }

      else if (bufcmp(cb, rxbutton_config_date_dd_plus)){
        flush_buffer();
        if(screenvar.date_mm == 1 || screenvar.date_mm == 3 || screenvar.date_mm == 5 || screenvar.date_mm == 7 || screenvar.date_mm == 8 || screenvar.date_mm == 10 || screenvar.date_mm == 12){
          if(screenvar.date_dd < 31){
            screenvar.date_dd++;
          } else {
            screenvar.date_dd=31;
          }
          datetime_changed = true;
        }
        if(screenvar.date_mm == 4 || screenvar.date_mm == 6 || screenvar.date_mm == 9 || screenvar.date_mm == 11){
          if(screenvar.date_dd < 30){
            screenvar.date_dd++;
          }else {
            screenvar.date_dd=30;
          }
          datetime_changed = true;

        }
        if(screenvar.date_mm == 2){
          if((screenvar.date_yyyy-YEAR_LEAP)%4 == 0){
            if(screenvar.date_dd < 29) screenvar.date_dd++;
          } else {
            if(screenvar.date_dd < 28) screenvar.date_dd++;
          }
          datetime_changed = true;
        }
      }

    else if (bufcmp(cb, rxbutton_config_date_dd_minus)){
      flush_buffer();
      if(screenvar.date_dd > 1)
        screenvar.date_dd--;
        datetime_changed = true;
    }

    else if (bufcmp(cb, rxbutton_config_time_hh_plus)){
      flush_buffer();
      if(screenvar.time_hh < 23){
        screenvar.time_hh++;
      }else {
      screenvar.time_hh = 0;
      }
      datetime_changed = true;
    }

    else if (bufcmp(cb, rxbutton_config_time_hh_minus)){
      flush_buffer();
      if(screenvar.time_hh > 0){
        screenvar.time_hh--;
      }else{
        screenvar.time_hh=23;
      }
      datetime_changed = true;
    }

    else if (bufcmp(cb, rxbutton_config_time_mm_plus)){
      flush_buffer();
      if(screenvar.time_mm < 59){
      screenvar.time_mm++;
      }else {
        screenvar.time_mm = 0;
      }
      datetime_changed = true;
    }

    else if (bufcmp(cb, rxbutton_config_time_mm_minus)){
      zerobuf();
      if(screenvar.time_mm > 0){
        screenvar.time_mm--;
      } else {
        screenvar.time_mm=59;
      }
      datetime_changed = true;
    }

    //measunit
    else if (bufcmp(cb, rxbutton_config_metric)){
      //flush_buffer();
      zerobuf();
      screenvar.meas_unit = 1;
      measunit_changed = true;
    }

    else if (bufcmp(cb, rxbutton_config_imperial)){
      //flush_buffer();
      zerobuf();
      screenvar.meas_unit = 2;
      measunit_changed = true;
    }

    }
}   

//A thread that keeps the screen updated
static THD_WORKING_AREA(ScreenhandlerTask_wa, 2048);
static THD_FUNCTION(ScreenhandlerTask, p) {

  (void)p;
  screenvar.meas_unit = 1;
  uint8_t  tm0=0;
  uint8_t  tm1=0;
  uint16_t tm2=0;
    screen_fetch_info();
    refresh_bat();
    refresh_chg_remaining();
    refresh_serial_no();
    display_send(&screen_main);
  chThdSleepMilliseconds(10);
  chRegSetThreadName("ScreenHandler task");
  while (TRUE) {
    screen_fetch_info();
    StateMachine_GetEvent();
    screen_set_info();
    if((TIMING_1/TIMING_TASK) == tm0){
        tm0=0;
        
        screen_refresh();
    } else {
        tm0++;
    }
    if((TIMING_2/TIMING_TASK) == tm1){
        tm1=0;
        refresh_bat();
        refresh_chg_remaining();
    } else {
        tm1++;
    }
    if((TIMING_3/TIMING_TASK) == tm2){
      tm2=0;
      refresh_serial_no();
    } else {
        tm2++;
    }
    chThdSleepMilliseconds(20);
  }
}

void sm_init(void){
    chThdCreateStatic(ScreenhandlerTask_wa, sizeof(ScreenhandlerTask_wa), NORMALPRIO, ScreenhandlerTask, NULL);
}


void screen_fetch_info(void){ //get parameters
    screenvar.meas_unit          =    FetchMeasUnit();
    screenvar.overtilt           =    FetchTiltState();
    screenvar.overtemp           =    FetchOverTempState();
    screenvar.ischarging         =    FetchChargeState();
    screenvar.overweight         =    FetchOverWeightState();
    screenvar.liftingweight      =    FetchLiftedWeight();
    screenvar.battery_level      =    FetchBatteryLevel();
    screenvar.charge_remaining   =    FetchChargeRemainMinutes();
    screenvar.time_hh            =    FetchHour();
    screenvar.time_mm            =    FetchMinute();
    screenvar.date_dd            =    FetchDay();
    screenvar.date_mm            =    FetchMonth();
    screenvar.date_yyyy          =    FetchYear();
    screenvar.lift_speed         =    FetchLiftingSpeed();
}

void screen_set_info(void){  //set parameters
/*
This structure is used to hold the values representing a calendar time.
It contains the following members, with the meanings as shown.

int tm_sec       seconds after minute [0-61] (61 allows for 2 leap-seconds)
int tm_min       minutes after hour [0-59]
int tm_hour      hours after midnight [0-23]
int tm_mday      day of the month [1-31]
int tm_mon       month of year [0-11]
int tm_year      current year-1900
int tm_wday      days since Sunday [0-6]
int tm_yday      days since January 1st [0-365]
int tm_isdst     daylight savings indicator (1 = yes, 0 = no, -1 = unknown)
*/


  if(datetime_changed == true){
    datetime_changed = false;
    struct tm tim;

    GetTimeTm(&tim);
    tim.tm_year = screenvar.date_yyyy-1900;
    tim.tm_mon = screenvar.date_mm-1;
    tim.tm_mday = screenvar.date_dd;
    
    tim.tm_hour = screenvar.time_hh;
    tim.tm_min = screenvar.time_mm;
    SetDateTm(tim);
  }
  if(spd_changed == true){
    spd_changed = false;
    SetLiftingSpeed(screenvar.lift_speed);
  }
  if(measunit_changed == true){
    measunit_changed = false;
    SetMeasUnit(screenvar.meas_unit);
  }
}

void screen_refresh(void){
    if(screenvar.date_mm == 4 || screenvar.date_mm == 6 || screenvar.date_mm == 9 || screenvar.date_mm == 11){
      if(screenvar.date_dd > 30)
        screenvar.date_dd=30;
    }
    if(screenvar.date_mm == 2){
      if((screenvar.date_yyyy-YEAR_LEAP)%4 == 0){
        if(screenvar.date_dd > 29){
          screenvar.date_dd =29;
        }
      } else {
      if(screenvar.date_dd > 28)
        screenvar.date_dd = 28;
      }
    }

    display_send(&lifting_spd[screenvar.lift_speed]);
    display_send(&arr_measunit[screenvar.meas_unit]);
    display_send(&years[screenvar.date_yyyy-YEAR_MIN]);
    display_send(&months[screenvar.date_mm-1]);
    display_send(&days[screenvar.date_dd-1]);
    display_send(&clock_hh[screenvar.time_hh]);
    display_send(&clock_mm[screenvar.time_mm]);
    weight_send(screenvar.liftingweight,screenvar.meas_unit);
}

void weight_send(uint16_t weight, uint8_t meas_unit){
    uint8_t w100 = 0;
    uint8_t w10  = 0;
    uint8_t w1   = 0;

    if(meas_unit !=1){
      float val = (float)weight;
      val = val * LBSMUL;
      weight = val;
    }
    if(weight > 999) weight = 999;
    if(weight > 0){
      w1 = weight%10;
      weight/=10;
    } 
    if(weight > 0){
      w10 = weight%10;
      weight/=10;
    } 
    if(weight > 0){
      w100 = weight%10;
      weight/=10;
    } 
    if(w10 == 0 && w100 == 0)
      w10=10;

    if(w100 == 0)
      w100=10;

    display_send(&weight_100[w100]);
    display_send(&weight_10[w10]);
    display_send(&weight_1[w1]);
}

void setovertempstate(bool isovertemp){
    screenvar.overtemp = isovertemp;
  }

  void setovertiltstate(bool isovertilt){
    screenvar.overtilt = isovertilt;
  }

  void setoverweightstate(bool isoverweight){
    screenvar.overweight = isoverweight;
  }

  void setchargestate(bool chg){
    screenvar.ischarging = chg;
  }


screen_variables getvariables (void){
    return screenvar;
}