#ifndef INFOCONFIG_H_INCLUDED
#define INFOCONFIG_H_INCLUDED

#include "screenhandler.h"
#include "rollsensor.h"
#include "esc_comm.h"
#include "eeprom.h"
#include "rtc.h"

/*===========================================================================*/
/* Function macros (NWL APIs).                                               */
/*===========================================================================*/
#define FetchMeasUnit() get_meas_unit()
#define FetchTiltState() IsRollingDetected()
#define FetchOverTempState() IsEscInOverTemperature()
#define FetchChargeState() IsBatteryInCharging()
#define FetchOverWeightState() IsInOverWeightState()
#define FetchLiftedWeight() GetLiftedWeightKg()
#define FetchBatteryLevel() GetStateOfCharge()
#define FetchChargeRemainMinutes() GetChargeTimeLeftMin()
#define FetchSerialnumber() GetMachineID()
#define FetchLiftingSpeed() GetLiftingSpeed()

#define FetchYear() GetYear()
#define FetchMonth() GetMonth()
#define FetchDay() GetDay()
#define FetchHour() GetHour()
#define FetchMinute() GetMin()


#endif