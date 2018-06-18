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

/*
 * Blinker thread.
 */
static THD_WORKING_AREA(thread1_wa, 128);
static THD_FUNCTION(thread1, p) {

  (void)p;
  chRegSetThreadName("blinker");
  while (TRUE) {
    systime_t time;
    time = 500;

    //palSetPad(GPIOD, GPIOD_STAT_LED);
    chThdSleepMilliseconds(time/20);
    //palClearPad(GPIOD, GPIOD_STAT_LED);
    chThdSleepMilliseconds(time);

  }
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

  /*
   * Shell manager initialization.
   */
  consoleInit();

  /* 
   * Initialization of ESC controlling
   */
  ESC_ControlInit();

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