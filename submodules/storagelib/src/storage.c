#include "ch.h"
#include "hal.h"
#include "crc16.h"
#include "storageconf.h"


/*
 *	Struct of a record in a memory
 */
static struct MemoryRecord 
{
  uint8_t* source;
  uint16_t size;
  uint16_t crc_orig;
  uint16_t crc_gen;
  uint8_t state;
};

/*
 *	It points to the storagelib configuration
 */
struct StorageConfig *conf;

/*
 *	It represents the current state of the storagelib
 */
StorageLib_state_t libstate = STORAGELIB_UNINIT;

/*
 *	Array of the records from the MemoryRecord struct
 */
static struct MemoryRecord MMap[MAX_RECORD_NUMBER] = { NULL };

static mutex_t SyncMutex;


/**
 * @brief   Assign a memory address and length to a memory record
 * @details It writes the given parameters into the MMap array
 *          @param[in] Id    - Id of the record
 *          @param[in] *data - Memory address of the data which should be saved
 *          @param[in] len   - Length of the variable/array/object
 *
 */
void AssignToMemoryMap(uint16_t Id, uint8_t* data, uint16_t len){
	MMap[Id].source = data;
	MMap[Id].size = len;
	MMap[Id].state = NOT_CHECKED;
	MMap[Id].crc_gen = 0;
	MMap[Id].crc_orig = 0;	
}


/**
 * @brief   Storagelib Init
 * @details It initialize the whole storagelib
 *          @param[in] *config  - Storagelib configuration
 *
 */
StorageLib_state_t InitStorage(struct StorageConfig *config){

	conf = config;

	chMtxObjectInit(&SyncMutex);

	#if STORAGELIB_USE_EEPROM
		uint8_t ret = lld_eepromInit(conf->i2cdrv, conf->eeprom_addr, conf->memorysize, conf->pagesize);

	if(ret == EEPROM_FAULT)
		libstate = STORAGELIB_MEM_CONN_ERROR;
	else
		libstate = STORAGELIB_ACTIVE;

	#endif

	return libstate;
}

#if STORAGELIB_USE_EEPROM

/**
 * @brief   StoreMapToEeprom
 * @details It stores the whole memory map into the eeprom
 *
 */
uint8_t StoreMapToEeprom(void){
	if(!chMtxTryLock(&SyncMutex))
		return STORAGELIB_MEMORY_IN_USE;

	lld_SetWAddrPtr(0x0000);

	uint16_t i;
	for(i = 0; i < MAX_RECORD_NUMBER; i++)
	{
		if(MMap[i].source != NULL && MMap[i].size != NULL){
			uint8_t ret = lld_eepromWriteNext(MMap[i].source, MMap[i].size);
			if(ret != MSG_OK){
				chMtxUnlock(&SyncMutex);
				return STORAGELIB_MEM_CONN_ERROR;
			}

			uint16_t crc_get = GetCRC16(MMap[i].source, MMap[i].size);
			ret = lld_eepromWriteNext(&crc_get, 2);
			if(ret != MSG_OK){
				chMtxUnlock(&SyncMutex);
				return STORAGELIB_MEM_CONN_ERROR;
			}
		}
	}
	chMtxUnlock(&SyncMutex);
	return STORAGELIB_OK;
}

/**
 * @brief   Read map from eeprom
 * @details It reads the whole map from eeprom
 *
 */
uint8_t ReadMapFromEeprom(void){
	if(!chMtxTryLock(&SyncMutex))
		return STORAGELIB_MEMORY_IN_USE;

	lld_SetRAddrPtr(0x0000);
	uint8_t result = STORAGELIB_OK;

	uint16_t i;
	for(i = 0; i < MAX_RECORD_NUMBER; i++)
	{
		if(MMap[i].source != NULL && MMap[i].size != NULL){
			uint8_t ret = lld_eepromReadNext(MMap[i].source, MMap[i].size);
			
			if(ret != MSG_OK){
				chMtxUnlock(&SyncMutex);
				return STORAGELIB_MEM_CONN_ERROR;
			}

			uint16_t crc_orig = 0x0000;
			ret = lld_eepromReadNext(&crc_orig, 2);

			if(ret != MSG_OK){
				chMtxUnlock(&SyncMutex);
				return STORAGELIB_MEM_CONN_ERROR;
			}

			uint16_t crc_get = GetCRC16(MMap[i].source, MMap[i].size);

			MMap[i].crc_gen = crc_get;
			MMap[i].crc_orig = crc_orig;

			if(crc_get == crc_orig)
				MMap[i].state = CRC_OK;
			else
				MMap[i].state = CRC_WRONG;
		}
		
		if(MMap[i].state == CRC_WRONG)
			result = STORAGELIB_CRC_ERROR;
	}
	chMtxUnlock(&SyncMutex);
	return result;
}

/**
 * @brief   Store only one record into the eeprom
 * @details It stores only just one record into the eeprom
 *          @param[in] Id  - Id of the record
 *
 */
uint8_t StoreRecordToEeprom(uint16_t Id){
	if(!chMtxTryLock(&SyncMutex))
		return STORAGELIB_MEMORY_IN_USE;


	uint16_t i;
	uint16_t addr = 0;
	for(i = 0; i < Id; i++)
		addr += (MMap[i].size + 2);
	lld_SetWAddrPtr(addr);

	if(MMap[Id].source != NULL && MMap[Id].size != NULL){
		uint8_t ret = lld_eepromWriteNext(MMap[Id].source, MMap[Id].size);

		if(ret != MSG_OK)
			return STORAGELIB_MEM_CONN_ERROR;
		
		uint16_t crc_get = GetCRC16(MMap[Id].source, MMap[Id].size);
		ret = lld_eepromWriteNext(&crc_get, 2);
		if(ret != MSG_OK)
			return STORAGELIB_MEM_CONN_ERROR;
	}
	chMtxUnlock(&SyncMutex);
	return STORAGELIB_OK;
}

/**
 * @brief   Read only one record from the eeprom
 * @details It reads only just one record from the eeprom
 *          @param[in] Id  - Id of the record
 *
 */
uint8_t ReadRecordFromEeprom(uint16_t Id){
	if(!chMtxTryLock(&SyncMutex))
		return STORAGELIB_MEMORY_IN_USE;

	uint8_t result = STORAGELIB_OK;
	uint16_t i;
	uint16_t addr = 0;
	for(i = 0; i < Id; i++)
		addr += (MMap[i].size + 2);
	lld_SetRAddrPtr(addr);


	if(MMap[Id].source != NULL && MMap[Id].size != NULL){
		uint8_t ret = lld_eepromReadNext(MMap[Id].source, MMap[Id].size);
		
		if(ret != MSG_OK){
			return STORAGELIB_MEM_CONN_ERROR;
		}

		uint16_t crc_orig = 0x0000;
		ret = lld_eepromReadNext(&crc_orig, 2);

		if(ret != MSG_OK){
			return STORAGELIB_MEM_CONN_ERROR;
		}

		uint16_t crc_get = GetCRC16(MMap[Id].source, MMap[Id].size);

		MMap[Id].crc_gen = crc_get;
		MMap[Id].crc_orig = crc_orig;

		if(crc_get == crc_orig)
			MMap[Id].state = CRC_OK;
		else
			MMap[Id].state = CRC_WRONG;
	}
	
	if(MMap[Id].state == CRC_WRONG)
		result = STORAGELIB_CRC_ERROR;

	chMtxUnlock(&SyncMutex);
	return result;
}

/**
 * @brief   Read one block from the eeprom
 * @details It reads one block from the eeprom. One block consists more record.
 *          @param[in] start  - First record id of the block
 *			@param[in] end    - Last record id of the block
 *
 */
uint8_t ReadBlockFromEeprom(uint16_t start, uint16_t end){

	if(start >= end || end >= MAX_RECORD_NUMBER)
		return STORAGELIB_WRONG_PARAM;

	uint16_t i;
	uint8_t finalres = STORAGELIB_OK;
	for(i = start; i <= end; i++){
		uint8_t res = ReadRecordFromEeprom(i);
		if(res != STORAGELIB_OK)
			finalres = STORAGELIB_CRC_ERROR;
	}
	return finalres;
}

/**
 * @brief   Store one block into the eeprom
 * @details It stores one block into the eeprom. One block consists more record.
 *          @param[in] start  - First record id of the block
 *			@param[in] end    - Last record id of the block
 *
 */
uint8_t StoreBlockToEeprom(uint16_t start, uint16_t end){

	if(start >= end || end >= MAX_RECORD_NUMBER)
		return STORAGELIB_WRONG_PARAM;

	uint16_t i;
	for(i = start; i <= end; i++){
		uint8_t res = StoreRecordToEeprom(i);
		if(res != STORAGELIB_OK)
			return res;
	}
	return STORAGELIB_OK;
}

/**
 * @brief   Used space
 * @details It gives back the used space from the eeprom
 *
 */
uint16_t GetEepromUsedSpace(void){
	uint16_t i;
	uint16_t bytes = 0;
	for(i = 0; i < MAX_RECORD_NUMBER; i++)
		bytes += MMap[i].size;
	return bytes;
}

/**
 * @brief   Total space
 * @details It gives back the total space of the eeprom
 *
 */
uint16_t GetEepromTotalSpace(void){
	return conf->memorysize;
}

/**
 * @brief   Record state
 * @details It gives back CRC check state of the record
 *
 */
uint8_t GetEepromRecordState(uint16_t Id){
	return MMap[Id].state;
}

#endif