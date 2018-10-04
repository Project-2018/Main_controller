# storagelib

The storagelib can handle the EEPROM operations.

# Usage

```c
struct StorageConfig config = {
	&I2CD1,
	DEFAULT_EEPROM_ADDR,
	MEM_256K,
	PAGE_64
};

void InitEepromModule(void){
	InitStorage(&config);

	AssignToMemoryMap(MAININJ, GetMainInj(), 880);
	AssignToMemoryMap(IATTABLE, GetIatTable(), 80);
	AssignToMemoryMap(MAXRPM, GetMAXRPMPtr(), 2);

	ReadMapFromEeprom();
	
	StoreMapToEeprom(); // To store the map into the eeprom
	StoreRecordToEeprom(1); // It stored the IATTABLE into the eeprom
}
```
