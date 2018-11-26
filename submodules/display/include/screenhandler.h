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

#include "ch.h"
#include "hal.h"

#define MAX_COMMAND_LEN 50



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
        char* serial_number;
    } screen_variables;

    //Function used to compare incoming serial with a given possible input
    bool bufcmp(uint8_t* array1, uint8_t* array2);

    //These functions used in screen_fetch_info function
    bool get_tilt_state(void);
    bool get_overtemp_state(void);
    bool get_overweight_state(void);
    bool get_charge_state(void);
    uint16_t get_lifting_weight(void);
    uint8_t get_battery_level(void);
    uint8_t get_charge_remaining_state(void);
    uint8_t get_meas_unit(void);
    uint8_t get_time_hh(void);
    uint8_t get_time_mm(void);
    uint8_t get_date_dd(void);
    uint8_t get_date_mm(void);
    unsigned int get_date_yyyy(void);
    uint8_t get_calib_result(void);
    char* getSerialNumber(void);

    //Event fetcher
    void StateMachine_GetEvent(void);
   
    //Functions for each states
    void sm_main(void);
    void sm_menu(void);
    void sm_menu_calib(void);
    void sm_sysinfo(void);
    void sm_overload_warn(void);
    void sm_overtemp_warn(void);
    void sm_overtilt_warn(void);
    void sm_config(void);
    void sm_charge(void);

    //Collects info for the SM
    void screen_fetch_info(void);
    uint8_t get_lift_speed(void);
    void screen_refresh(void);
    void weight_send(uint16_t weight);
    void refresh_weight(void);
    uint8_t getbuflen(uint8_t array[]);

    //Test functions
    void setovertempstate(bool isovertemp);
    void setovertiltstate(bool isovertilt);
    void setoverweightstate(bool isoverweight);
    void setchargestate(bool ischarging);
    screen_variables getvariables(void);
    const uint8_t getsizeofarr(void);
    const char* StateMachine_GetStateName();


#endif