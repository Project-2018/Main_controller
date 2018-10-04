#ifndef EEPROM_H_
#define EEPROM_H_

#define LENGTH_OF_MACHINE_SERIAL	20

typedef struct {
	char MachineSerialNumber[LENGTH_OF_MACHINE_SERIAL];
} MachineID_t;


void InitEeprom(void);
void cmd_storagetest(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_machineid(BaseSequentialStream *chp, int argc, char *argv[]);



#endif