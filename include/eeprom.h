#ifndef EEPROM_H_
#define EEPROM_H_

#define LENGTH_OF_MACHINE_SERIAL	10

typedef struct {
	char MachineSerialNumber[LENGTH_OF_MACHINE_SERIAL];
} MachineID_t;


uint8_t InitEeprom(void);
void cmd_storagetest(BaseSequentialStream *chp, int argc, char *argv[]);
void cmd_machineid(BaseSequentialStream *chp, int argc, char *argv[]);

char* GetMachineID(void);

uint8_t GetMeasUnit(void);
uint32_t GetUptimeMin(void);

uint8_t EepromSetMeasUnit(uint8_t val);



#endif