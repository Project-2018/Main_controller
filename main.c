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
#include "battery.h";
#include "usbdrv.h"

/*
 * Blinker thread.
 */
static THD_WORKING_AREA(thread1_wa, 128);
static THD_FUNCTION(thread1, p) {

  (void)p;
  chRegSetThreadName("blinker");
  while (TRUE) {
    systime_t time;
    //time = 500;
    
    time = usbdrvGetActive() ? 250 : 500;

    palSetPad(GPIOD, GPIOD_STAT_LED);
    chThdSleepMilliseconds(time/20);
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
  return 0.0f;
}

static BatteryConfig_t BattManCfg = {
  {
    30.0f, 31.32f, 10.88f, 32.64f, 35.28f, 36.6f, 37.92f, 39.24f, 40.56f, 41.88f, 43.2f
  },
  {
    0.0f, 10.0f, 20.0f, 30.0f, 40.0f, 50.0f, 60.0f, 70.0f, 80.0f, 90.0f, 100.0f
  },
  0.5f,
  10.7f,
  43.0f,
  100,
  420,
  GetBattVoltage,
  GetBatteryCurrent,
  GetChargeVoltage,
};

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

  /*
   * Shell manager initialization.
   */
  consoleInit();

  /*
   * Init ADC measures
   */
  InitMeasures();

  /*
   *  Init battery management module
   */
  InitBatteryManagement(&BattManCfg);

  /* 
   * Initialization of ESC controlling
   */
  ESC_ControlInit();

  /*
   *  Initialization of roll sensor
   */
  InitRollSensor(&RollSenCfg);

  /*
   * Creates the blinker thread.
   */
  chThdCreateStatic(thread1_wa, sizeof(thread1_wa), NORMALPRIO, thread1, NULL);

  /* 
   * Main loop where only the shell is handled
   */
  while (true) {
    consoleStart();
    chThdSleepMilliseconds(1000);
  }
}