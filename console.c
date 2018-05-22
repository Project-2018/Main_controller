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
  {"clear", cmd_clear},
  {"val",   cmd_val},

  {NULL, NULL}
};

static const ShellConfig shell_cfg1 = {
  (BaseSequentialStream *)&SDU1,
  commands
};

void consoleInit(void){

  /* Shell initialization.*/
  sduObjectInit(&SDU1);
  sduStart(&SDU1, &serusbcfg);

  consoleThread = NULL;

  /*
   * Activates the USB driver and then the USB bus pull-up on D+.
   * Note, a delay is inserted in order to not have to disconnect the cable
   * after a reset.
   */
  usbDisconnectBus(serusbcfg.usbp);
  chThdSleepMilliseconds(1500);
  usbStart(serusbcfg.usbp, &usbcfg);
  usbConnectBus(serusbcfg.usbp);

  shellInit();
  chEvtRegister(&shell_terminated, &shell_el, 0);
}

void consoleStart(void){
  if (!consoleThread && (SDU1.config->usbp->state == USB_ACTIVE))
    consoleThread = chThdCreateFromHeap(NULL, SHELL_WA_SIZE,
                                       "shell1", NORMALPRIO + 1,
                                       shellThread, (void *)&shell_cfg1);
  else if (chThdTerminatedX(consoleThread)) {
    chThdRelease(consoleThread);    /* Recovers memory of the previous shell.   */
    consoleThread = NULL;           /* Triggers spawning of a new shell.        */
  }
}

void cmd_clear(BaseSequentialStream *chp, int argc, char *argv[]){

  (void)argv;
  chprintf(chp, "\x1B\x63");
  chprintf(chp, "\x1B[2J");
  chprintf(chp, "\x1B[%d;%dH", 0, 0);
}

void bldc_val_received(mc_values *val){
    chprintf((BaseSequentialStream*)&SDU1, "\r\n");
    chprintf((BaseSequentialStream*)&SDU1, "Input voltage: %.2f V\r\n",    val->v_in);
    chprintf((BaseSequentialStream*)&SDU1, "Temp:          %.2f degC\r\n", val->temp_pcb);
    chprintf((BaseSequentialStream*)&SDU1, "Current motor: %.2f A\r\n",    val->current_motor);
    chprintf((BaseSequentialStream*)&SDU1, "Current in:    %.2f A\r\n",    val->current_in);
    chprintf((BaseSequentialStream*)&SDU1, "RPM:           %.1f RPM\r\n",  val->rpm);
    chprintf((BaseSequentialStream*)&SDU1, "Duty cycle:    %.1f %%\r\n",   val->duty_now * 100.0);
    chprintf((BaseSequentialStream*)&SDU1, "Ah Drawn:      %.4f Ah\r\n",   val->amp_hours);
    chprintf((BaseSequentialStream*)&SDU1, "Ah Regen:      %.4f Ah\r\n",   val->amp_hours_charged);
    chprintf((BaseSequentialStream*)&SDU1, "Wh Drawn:      %.4f Wh\r\n",   val->watt_hours);
    chprintf((BaseSequentialStream*)&SDU1, "Wh Regen:      %.4f Wh\r\n",   val->watt_hours_charged);
    chprintf((BaseSequentialStream*)&SDU1, "Tacho:         %i counts\r\n", val->tachometer);
    chprintf((BaseSequentialStream*)&SDU1, "Tacho ABS:     %i counts\r\n", val->tachometer_abs);
    chprintf((BaseSequentialStream*)&SDU1, "Fault Code:    %s\r\n", bldc_interface_fault_to_string(val->fault_code));
}

void cmd_val(BaseSequentialStream *chp, int argc, char *argv[]) {
  (void)argv;
  
  if (argc > 0) {
    chprintf(chp, "Usage: val\r\n");
    return;
  }
  while (chnGetTimeout((BaseChannel *)chp, TIME_IMMEDIATE) == Q_TIMEOUT) {
    chprintf((BaseSequentialStream*)&SDU1, "\x1B\x63");
    chprintf((BaseSequentialStream*)&SDU1, "\x1B[2J");
    chprintf((BaseSequentialStream*)&SDU1, "\x1B[%d;%dH", 0, 0);

    chprintf((BaseSequentialStream*)&SDU1, "Continuous reading of parameters... \r\n");
    bldc_interface_set_rx_value_func(bldc_val_received);
    bldc_interface_get_values();
        
    chThdSleepMilliseconds(500);
  }
}