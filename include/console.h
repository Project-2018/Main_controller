#ifndef CONSOLE_H_INCLUDED
#define CONSOLE_H_INCLUDED

void consoleInit(void);
void consoleStart(void);

void cmd_gyro(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_measvalues(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_batt(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_reset(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_SMTest(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_GetCPUUsage(BaseSequentialStream *chp, int argc, char *argv[]);


void cmd_set_overtemp(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_clear_overtemp(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_set_overtilt(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_clear_overtilt(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_set_overweight(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_clear_overweight(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_set_charging(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_clear_charging(BaseSequentialStream *chp, int argc, char *argv[]);

bool usbdrvGetActive(void);

#endif
