#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "ch.h"
#include "hal.h"

#include "shell.h"
#include "chprintf.h"

#include "console.h"
#include "usbcfg.h"

#include "bldc_interface.h"

#include "rollsensor.h"
#include "measure.h"
#include "calc.h"
#include "battery.h"
#include "rtc.h"
#include "syslog.h"
#include "eeprom.h"

#include "esc_comm.h"

#define usb_lld_connect_bus(usbp)
#define usb_lld_disconnect_bus(usbp)

#define SHELL_WA_SIZE   THD_WORKING_AREA_SIZE(2048)

void cmd_clear(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_val(BaseSequentialStream *chp, int argc, char *argv[]);

thread_t *consoleThread;

event_listener_t shell_el;

/* Shell commands */
static const ShellCommand commands[] = {
  {"\r\n| ", NULL},
  {"clear",      cmd_clear},
  {"gyro",       cmd_gyro},
  {"measvalues", cmd_measvalues},
  {"batt",       cmd_batt},
  {"val",        cmd_val},
  {"btnVal",     cmd_escCommBtnValues},
  {"date",       cmd_date},
  {"syslog",     cmd_GetSysLog},
  {"storage",    cmd_storagetest},
  {"reset",      cmd_reset},
  {"machineid",  cmd_machineid},
  {"cpu",        cmd_GetCPUUsage},
  {NULL, NULL}
};

static const ShellConfig shell_cfg1 = {
  (BaseSequentialStream *)&SDU1,
  commands
};


void InitUsb(void){
   /*
   * Initializes two serial-over-USB CDC drivers.
   */
  sduObjectInit(&SDU1);
  sduStart(&SDU1, &serusbcfg);

  /*
   * Activates the USB driver and then the USB bus pull-up on D+.
   * Note, a delay is inserted in order to not have to disconnect the cable
   * after a reset.
   */
  usbDisconnectBus(serusbcfg.usbp);
  chThdSleepMilliseconds(1500);
  usbStart(serusbcfg.usbp, &usbcfg);
  usbConnectBus(serusbcfg.usbp); 
}

bool usbdrvGetActive(void) {
  return serusbcfg.usbp->state == USB_ACTIVE ? TRUE : FALSE;
}

void consoleInit(void){

  InitUsb();

  shellInit();
  
  consoleThread = NULL;
  
  chEvtRegister(&shell_terminated, &shell_el, 0);
}



void consoleStart(void){
//  if (!consoleThread && (SDU1.config->usbp->state == USB_ACTIVE))
//    consoleThread = chThdCreateFromHeap(NULL, SHELL_WA_SIZE,
//                                       "shell1", NORMALPRIO + 1,
//                                       shellThread, (void *)&shell_cfg1);
//  else if (chThdTerminatedX(consoleThread)) {
//    chThdRelease(consoleThread);    /* Recovers memory of the previous shell.   */
//    consoleThread = NULL;           /* Triggers spawning of a new shell.        */
//  }
  if (SDU1.config->usbp->state == USB_ACTIVE) {
      thread_t *shelltp = chThdCreateFromHeap(NULL, SHELL_WA_SIZE,
                                              "shell", NORMALPRIO + 1,
                                              shellThread, (void *)&shell_cfg1);
      chThdWait(shelltp);               /* Waiting termination.             */
  }
}

void cmd_clear(BaseSequentialStream *chp, int argc, char *argv[]){

  (void)argv;
  chprintf(chp, "\x1B\x63");
  chprintf(chp, "\x1B[2J");
  chprintf(chp, "\x1B[%d;%dH", 0, 0);
}

void cmd_val(BaseSequentialStream *chp, int argc, char *argv[]) {
  (void)argv;
  
  while (chnGetTimeout((BaseChannel *)chp, TIME_IMMEDIATE) == Q_TIMEOUT) {

    chprintf(chp, "\x1B\x63");
    chprintf(chp, "\x1B[2J");
    chprintf(chp, "\x1B[%d;%dH", 0, 0);

    chprintf(chp, "ESC parameters: \r\n");
    chprintf(chp, "Input voltage: %0.2f\r\n" ,escGetVoltage()         );
    chprintf(chp, "Temp:          %0.2f\r\n" ,escGetESCTemp()         );
    chprintf(chp, "Current motor: %0.2f\r\n" ,escGetAcCurrent()       );
    chprintf(chp, "Current in:    %0.2f\r\n" ,escGetDcCurrent()       );
    chprintf(chp, "RPM:           %d\r\n" ,escGetERPM()            );
    chprintf(chp, "Duty cycle:    %0.2f\r\n" ,escGetDutyCycle()       );
    chprintf(chp, "Ah Drawn:      %0.4f\r\n" ,escGetAmpHours()        );
    chprintf(chp, "Ah Regen:      %0.4f\r\n" ,escGetAmpHoursCharged() );
    chprintf(chp, "Wh Drawn:      %0.4f\r\n" ,escGetWattHours()       );
    chprintf(chp, "Wh Regen:      %0.4f\r\n" ,escGetWattHoursCharged());
    chprintf(chp, "Tacho:         %d\r\n" ,escGetTachometer()      );
    chprintf(chp, "Tacho ABS:     %d\r\n" ,escGetTachometerAbs()   );
    chprintf(chp, "Fault Code:    %s\r\n", escGetFaultcode()       );
        
    chThdSleepMilliseconds(10);
  }
}

void cmd_gyro(BaseSequentialStream *chp, int argc, char *argv[]) {
  

  (void)argv;

  while (chnGetTimeout((BaseChannel *)chp, TIME_IMMEDIATE) == Q_TIMEOUT) {

    chprintf(chp, "\x1B\x63");
    chprintf(chp, "\x1B[2J");


    chprintf(chp, "x: %d\r\n", GetXAxis());
    chprintf(chp, "y: %d\r\n", GetYAxis());
    chprintf(chp, "z: %d\r\n", GetZAxis());
    chprintf(chp, "MonitoredAxis %d\r\n", GetMonitoredAxis());
    chprintf(chp, "IsRollingDetected: %d\r\n", IsRollingDetected());

    chThdSleepMilliseconds(200);

  }
}

void cmd_measvalues(BaseSequentialStream *chp, int argc, char *argv[]) {
  

  (void)argv;

  while (chnGetTimeout((BaseChannel *)chp, TIME_IMMEDIATE) == Q_TIMEOUT) {

    chprintf(chp, "\x1B\x63");
    chprintf(chp, "\x1B[2J");

    chprintf(chp, "Measured ADC channels: \r\n");
    chprintf(chp, "MEAS_CHG_VOLTAGE: \t%d\t%.2f\r\n", measGetValue(MEAS_CHG_VOLTAGE), GetChargeVoltage());
    chprintf(chp, "MEAS_BATT_CURRENT: \t%d\t%.2f\r\n", measGetValue(MEAS_BATT_CURRENT), GetBatteryCurrent());
    chprintf(chp, "MEAS_BATT_TEMP: \t%d\t%d\r\n", measGetValue(MEAS_BATT_TEMP), GetBatteryTemp());


    chThdSleepMilliseconds(200);

  }
}

void cmd_batt(BaseSequentialStream *chp, int argc, char *argv[]) {
  

  (void)argv;

  while (chnGetTimeout((BaseChannel *)chp, TIME_IMMEDIATE) == Q_TIMEOUT) {

    chprintf(chp, "\x1B\x63");
    chprintf(chp, "\x1B[2J");


    BatteryState_t state = GetBatteryState();

    switch(state){
      case BATTERY_DISCHARGE:
        chprintf(chp, "Battery state: \t\tBATTERY_DISCHARGE\r\n");
      break;
      case BATTERY_RELAXED:
        chprintf(chp, "Battery state: \t\tBATTERY_RELAXED\r\n");
      break;
      case BATTERY_BEGIN_CHRAGE:
        chprintf(chp, "Battery state: \t\tBATTERY_BEGIN_CHRAGE\r\n");
      break;
      case BATTERY_CHARGE:
        chprintf(chp, "Battery state: \t\tBATTERY_CHARGE\r\n");
      break;
      case BATTERY_CHARGE_FINISHED:
        chprintf(chp, "Battery state: \t\tBATTERY_CHARGE_FINISHED\r\n");
      break;
    }

    chprintf(chp, "ChargeTimeLeft: \t%d\r\n", GetChargeTimeLeftMin());
    chprintf(chp, "StateOfCharge: \t\t%d\r\n", GetStateOfCharge());
    chprintf(chp, "Battery voltage: \t%.2f\r\n", (float)escGetVoltage());
    chprintf(chp, "Charge voltage: \t%.2f\r\n", GetChargeVoltage());
    chprintf(chp, "Battery current: \t%.2f %.2f\r\n", GetBatteryCurrent(), (float)escGetDcCurrent());


    chThdSleepMilliseconds(200);

  }
}

void cmd_reset(BaseSequentialStream *chp, int argc, char *argv[]) {

  (void)argc;
  (void)argv;

  NVIC_SystemReset();

}

void cmd_GetCPUUsage(BaseSequentialStream *chp, int argc, char *argv[]){


  (void)argc;
  (void)argv;

  chprintf(chp, "\x1B\x63");
  chprintf(chp, "\x1B[2J");
  thread_t *tp;
  while (chnGetTimeout((BaseChannel *)chp, TIME_IMMEDIATE) == Q_TIMEOUT) {
    chprintf(chp, "\x1B\x63");
    chprintf(chp, "\x1B[2J");

    chprintf(chp, "---   RUNNING TASKS   ---\r\n\r\n");

    static const char *states[] = {CH_STATE_NAMES};

    tp = chRegFirstThread();

    uint32_t idle_time = 0;
    uint32_t busy_time = 0;

    do
    {
      uint32_t tmp = (tp->stats.cumulative/tp->stats.n);
      if (tp->prio == 1) /* idle task */
      {
        idle_time = tmp;
      }
      else
      {
        busy_time += tmp;
      }

      chprintf(chp, "%.8lx %4lu \t %20ls %12ls %lu\r\n",
               (uint32_t)tp,
               (uint32_t)tp->prio,
               tp->name,
               states[tp->state],
               tmp);
      tp = chRegNextThread(tp);
    } while (tp != NULL);
    
    chprintf(chp, "\r\nSystem load is %lu %%\r\n", (uint32_t)busy_time*100/(busy_time + idle_time));

    chThdSleepMilliseconds(200);
    
  }

}