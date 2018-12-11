
Lifted weight:
	#include "esc_comm.h"
	uint16_t GetLiftedWeightKg(void);
	uint16_t GetLiftedWeightLbs(void);

Rolling
	#include "rollsensor.h"
	bool IsRollingDetected(void);


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

Set date
	#include "rtc.h"
	void SetDateTm(struct tm tim);

Battery
	#include "battery.h"
	bool IsBatteryInCharging(void);
	uint16_t GetChargeTimeLeftMin(void);
	uint8_t GetStateOfCharge(void);

Serial number
	#include "eeprom.h"
	char* GetMachineID(void); (10 char)

Temperature
	#include "esc_comm.h"
	bool IsEscInOverTemperature(void);