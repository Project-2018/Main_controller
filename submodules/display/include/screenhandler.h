#ifndef SCREENHANDLER_H_INCLUDED
#define SCREENHANDLER_H_INCLUDED
/*
    ChibiOS - Copyright (C) 2006..2016 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

    void sm_init(void); 
    //variables need to be taken care of
    typedef struct {
        uint8_t* readbuf;
        bool overtilt;
        bool overtemp;
        bool overweight;
        bool ischarging;
        uint16_t liftingweight;
        uint8_t meas_unit;
        uint8_t battery_level;
        uint16_t charge_remaining;
        uint8_t time_hh;
        uint8_t time_mm;
        uint8_t date_dd;
        uint8_t date_mm;
        uint16_t date_yyyy;
        uint8_t lift_speed;
        uint8_t* serial_number;
    } screen_variables;

    //Event fetcher
    void StateMachine_GetEvent(void);



void setovertempstate(bool isovertemp);
  void setovertiltstate(bool isovertilt);
  void setoverweightstate(bool isoverweight);
  void setchargestate(bool chg);



    //test functions
    screen_variables getvariables (void);


#endif