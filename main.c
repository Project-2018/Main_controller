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
#include "esc_comm.h"
#include "console.h"
#include "rollsensor.h"
#include "measure.h"
#include "calc.h"
#include "battery.h"
#include "usbcfg.h"
#include "eeprom.h"
#include "storageconf.h"
#include "syslog.h"

#include "screenhandler.h"
#include "serial_comm.h"

/*
 * Blinker thread.
 */
static THD_WORKING_AREA(thread1_wa, 128);
static THD_FUNCTION(thread1, p) {

  (void)p;
  chRegSetThreadName("blinker");
  while (TRUE) {
    uint16_t time;
    time = 500;
    
    time = usbdrvGetActive() ? 250 : 500;

    palSetPad(GPIOD, GPIOD_STAT_LED);
    chThdSleepMilliseconds(time / 10);
    palClearPad(GPIOD, GPIOD_STAT_LED);
    chThdSleepMilliseconds(time);

  }
}

static const RollSensorConfig_t RollSenCfg = {
  &SPID1,
  GPIOE,
  GPIOE_SPI1_CS,
  X_AXIS,
  0,
  300,
  100
};

float GetBattVoltage(void){
  return (float)escGetVoltage();
}

float GetEscDcCurrent(void){
  return (float)escGetDcCurrent();
}

static BatteryConfig_t BattManCfg = {
  {
    34.8f, 35.1f, 35.8f, 36.4f, 36.8f, 37.2f, 37.6f, 37.9f, 38.2f, 38.5f, 40.0f
  },
  {
    0.0f, 5.5f, 16.0f, 26.5f, 37.0f, 47.5f, 58.0f, 68.5f, 79.0f, 89.5f, 100.0f
  },
  0.2f,
  10.7f,
  43.0f,
  100,
  420,
  GetBattVoltage,
  GetEscDcCurrent,
  GetChargeVoltage,
};

static EscControlConf_t EscCtrlConf = {
  IsDischargeAllowed,
  IsRollingDetected
};


void DiagMode(void){

}

/*
 * Application entry point.
 */
int main(void) {

  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();
  chSysInit();

  palClearLine(LINE_SOLENOID);

  /*
   * Creates the blinker thread.
   */
  chThdCreateStatic(thread1_wa, sizeof(thread1_wa), NORMALPRIO, thread1, NULL);

  /*
   * Eeprom initialization.
   */
  uint8_t EepromInit = InitEeprom();


  /*
   *  Init display communication
   */
  initSerialComm();

  /*
   * Shell manager initialization.
   */
  consoleInit();


  /*
   * Syslog initialization.
   */
  if(EepromInit == STORAGELIB_ACTIVE)
    SyslogInit((BaseSequentialStream *)&SDU1);

  /*
   * Init ADC measures
   */
  InitMeasures();

  /*
   *  Initialization of roll sensor
   */
  InitRollSensor(&RollSenCfg);

  /* 
   * Initialization of ESC controlling
   */
  ESC_ControlInit(&EscCtrlConf);


  /*
   *  Wait for ESC bootup
   */
  chThdSleepMilliseconds(3000);

  /*
   *  Init battery management module
   */
  InitBatteryManagement(&BattManCfg);


  
  if(usbdrvGetActive()){
    //SetEscMaintenence();
    ADD_SYSLOG(SYSLOG_INFO, "General", "Diagnostic mode activated.");
    ADD_SYSLOG(SYSLOG_INFO, "General", "Machine started. (%d, %.2fV, %dC)", GetStateOfCharge(), GetBattVoltage(), (int16_t)escGetESCTemp());
    while (true) {
      consoleStart();
      chThdSleepMilliseconds(1000);
    }
  }

  /*
   * Init display
   */
  sm_init();
 

  /*
   * Welcome message
   */
  ADD_SYSLOG(SYSLOG_INFO, "General", "Machine started. (%d, %.2fV, %dC)", GetStateOfCharge(), GetBattVoltage(), (int16_t)escGetESCTemp());

  if(EepromInit != STORAGELIB_ACTIVE)
    DoBeep();

  /* 
   * Main loop where only the shell is handled
   */
  while (true) {
    chThdSleepMilliseconds(100);
  }
}