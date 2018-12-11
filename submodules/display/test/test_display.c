#include "ch.h"
#include "hal.h"

#include "test_display.h"

//GET_FUNC

bool get_tilt_state(void){
  return false;
}

bool get_overtemp_state(void){
  return false;
}

bool get_overweight_state(void){
  return false;
}

bool get_charge_state(void){
  return true;
}

uint8_t get_battery_level(void){
  return 10;
}

uint8_t get_meas_unit(void){
  return 1;
}

uint16_t get_lifting_weight(void){
  return 500;
}

uint8_t get_charge_remaining_state(void){
  return 0;
}

uint8_t get_time_hh(void){
  return 1;
}

uint8_t get_time_mm(void){
  return 50;
}

uint8_t get_date_dd(void){
  return 1;
}

uint8_t get_date_mm(void){
  return 1;
}

uint16_t get_date_yyyy(void){
  return 1970;
}

uint8_t get_calib_result(void){
  return 0;
}

char* get_serial_number(void){
  return "0x00000001";
}

uint8_t get_lift_speed(void){
  return 1;
}


//SET func


uint8_t set_meas_unit(uint8_t measunit){
  return measunit;
}

uint8_t set_lifting_speed(uint8_t liftingspeed){
  return liftingspeed;
}

uint8_t set_time_hh(uint8_t hh){
  return hh;
}

uint8_t set_time_mm(uint8_t mm){
  return mm;
}

uint8_t set_date_dd(uint8_t dd){
  return dd;
}

uint8_t set_date_mm(uint8_t mm){
  return mm;
}

uint16_t set_date_yyyy(uint16_t yr){
  return yr;
}


