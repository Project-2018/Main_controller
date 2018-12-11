#ifndef DATA_DISPLAY_H_INCLUDED
#define DATA_DISPLAY_H_INCLUDED

#define MAX_COMMAND_LEN 20

extern uint8_t screen_welcome[];
extern uint8_t screen_main[];
extern uint8_t screen_overload_warn[];
extern uint8_t screen_overtemp_warn[];
extern uint8_t screen_overtilt_warn[];
extern uint8_t screen_charge[];
extern uint8_t screen_sysinfo[];

extern uint8_t rxbutton_config_metric[];
extern uint8_t rxbutton_config_imperial[];
extern uint8_t rxbutton_config_meas[];
extern uint8_t rxbutton_turt[]; 
extern uint8_t rxbutton_rabb[];
extern uint8_t rxbutton_config_date_yyyy_plus[];
extern uint8_t rxbutton_config_date_yyyy_minus[];
extern uint8_t rxbutton_config_date_mm_plus[];
extern uint8_t rxbutton_config_date_mm_minus[];
extern uint8_t rxbutton_config_date_dd_plus[];
extern uint8_t rxbutton_config_date_dd_minus[];
extern uint8_t rxbutton_config_time_hh_plus[];
extern uint8_t rxbutton_config_time_hh_minus[];
extern uint8_t rxbutton_config_time_mm_plus[];
extern uint8_t rxbutton_config_time_mm_minus[];

extern uint8_t overtilt_on[];
extern uint8_t overtilt_off[];
extern uint8_t init_weight_lbs[];
extern uint8_t init_weight_kgs[];

extern uint8_t lifting_spd[][17];
extern uint8_t battery[][18];
extern uint8_t clock_hh[][16];
extern uint8_t clock_mm[][16];
extern uint8_t years[][16];
extern uint8_t months[][17];
extern uint8_t days[][16];
extern uint8_t weight_100[][17];
extern uint8_t weight_10[][17];
extern uint8_t weight_1[][17];
extern uint8_t arr_measunit[][16];

extern uint8_t chg_remaining_hour[][17];
extern uint8_t chg_remaining_min[][17];

extern uint8_t serial_header[14];
extern uint8_t serial_footer[4];


#endif