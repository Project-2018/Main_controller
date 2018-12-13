#include "ch.h"
#include "hal.h"
#include "eeprom.h"
#include "memorymap.h"
#include "storageconf.h"
#include "syslog.h"

struct StorageConfig config = {
	&I2CD3,
	DEFAULT_EEPROM_ADDR,
	MEM_256K,
	PAGE_64
};

static MachineID_t MachineID;

const MachineID_t DefaultMID = {
  "0000000000"
};

uint8_t MeasUnit = 1;

uint32_t UptimeInMin = 0;

uint8_t InitEeprom(void){

	StorageLib_state_t init_res = InitStorage(&config);
	if(init_res != STORAGELIB_ACTIVE){
    return init_res;
	}

  AssignToMemoryMap(MACHINEID, (uint8_t*)&MachineID, sizeof(MachineID_t));
  AssignToMemoryMap(MEASUNIT, (uint8_t*)&MeasUnit, sizeof(uint8_t));
  AssignToMemoryMap(UPTIMEMIN, (uint8_t*)&UptimeInMin, sizeof(uint32_t));


  uint8_t res = ReadMapFromEeprom();

  if(res != STORAGELIB_OK){
    memcpy(&MachineID, &DefaultMID, sizeof(MachineID_t));
    StoreRecordToEeprom(MACHINEID);
    
    MeasUnit = 1;
    StoreRecordToEeprom(MEASUNIT);
    
    UptimeInMin = 0;
    StoreRecordToEeprom(UPTIMEMIN);
  }

  return init_res;
}

uint8_t GetMeasUnit(void){
  return MeasUnit;
}

uint8_t EepromSetMeasUnit(uint8_t val){
  uint8_t old = MeasUnit;

  if(val == 1)
    MeasUnit = 1;

  if(val == 2)
    MeasUnit = 2;

  uint8_t res = StoreRecordToEeprom(MEASUNIT);

  if(res != STORAGELIB_OK){
    ADD_SYSLOG(SYSLOG_ERROR, "EEPROM", "Failed to save meas unit: %d", MeasUnit);
    MeasUnit = old;
  }else{
    ADD_SYSLOG(SYSLOG_INFO, "EEPROM", "Meas unit saved: %d", MeasUnit);
  }

  return MeasUnit;
}

uint32_t* GetUptimeMin(void){
  return &UptimeInMin;
}

void cmd_setuptime(BaseSequentialStream *chp, int argc, char *argv[]) {

  (void)argc;
  (void)argv;

  uint32_t val = atoi(argv[0]);
  UptimeInMin = val;

  uint8_t res = StoreRecordToEeprom(UPTIMEMIN);

  if(res != STORAGELIB_OK){
    ADD_SYSLOG(SYSLOG_ERROR, "EEPROM", "Failed to save uptime: %d", UptimeInMin);
    chprintf(chp, "Uptime failed to save!\r\n");
  }else{
    ADD_SYSLOG(SYSLOG_INFO, "EEPROM", "Uptime saved: %d", MeasUnit);
    chprintf(chp, "Uptime changed to: %d\r\n", UptimeInMin);
  }
}

void cmd_storagetest(BaseSequentialStream *chp, int argc, char *argv[]) {

  (void)argc;
  (void)argv;

  if(atoi(argv[0]) == 0){
    uint8_t ret = StoreMapToEeprom();
    chprintf(chp, "%s\r\n", ret == STORAGELIB_OK ? "STORAGELIB_OK" : "ERROR");
    return;
  }
    
  if(atoi(argv[0]) == 1){  
    uint8_t ret = ReadMapFromEeprom();
    chprintf(chp, "%s\r\n", ret == STORAGELIB_OK ? "STORAGELIB_OK" : "ERROR");
    return;
  }

  if(atoi(argv[0]) == 2){

    uint16_t i;
    for(i=0; i < MAX_RECORD_NUMBER; i++){
      switch (GetEepromRecordState(i)){
        case CRC_OK:
          chprintf(chp, "ID: %d STATE: CRC_OK\r\n", i);
        break;
        case CRC_WRONG:
          chprintf(chp, "ID: %d STATE: CRC_WRONG\r\n", i);
        break;
        case NOT_CHECKED:
          chprintf(chp, "ID: %d STATE: NOT_CHECKED\r\n", i);
        break;

      }
      
    }

    chprintf(chp, "Used space: %d bytes / %d bytes\r\n", GetEepromUsedSpace(), GetEepromTotalSpace());

  return;
  }
  
}

void cmd_machineid(BaseSequentialStream *chp, int argc, char *argv[]){
  if (argc == 0) {
    chprintf("Wrong command");
  }

  if ((argc == 1) && (strcmp(argv[0], "get") == 0)){

      chprintf(chp, "%s\r\n", MachineID.MachineSerialNumber);

    return;
  }

  if ((argc == 2) && (strcmp(argv[0], "set") == 0)){

    memcpy(&MachineID.MachineSerialNumber, argv[1], LENGTH_OF_MACHINE_SERIAL);
    uint8_t res = StoreRecordToEeprom(MACHINEID);

    if(res != STORAGELIB_OK){
      chprintf(chp, "Saving machine id failed\r\n");
    }else{
      chprintf(chp, "Machine id saved succesfully\r\n");
    }

  }else{
    chprintf(chp, "Wrong command");
  }
}

char* GetMachineID(void){
  return &MachineID.MachineSerialNumber[0];
}