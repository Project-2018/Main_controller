#ifndef TEST_DISPLAY_H_INCLUDED
#define TEST_DISPLAY_H_INCLUDED

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
uint16_t get_date_yyyy(void);
uint8_t get_calib_result(void);
char* getSerialNumber(void);




uint8_t set_meas_unit(uint8_t measunit);
uint8_t set_lifting_speed(uint8_t liftingspeed);
uint8_t set_time_hh(uint8_t hh);
uint8_t set_time_mm(uint8_t mm);
uint8_t set_date_dd(uint8_t dd);
uint8_t set_date_mm(uint8_t mm);
uint16_t set_date_yyyy(uint16_t yr);


#endif