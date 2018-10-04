#ifndef STORAGE_H_INCLUDED
#define STORAGE_H_INCLUDED

/*
 *	Configuration structure of the storagelib
 */
struct StorageConfig {
	I2CDriver *i2cdrv;
	uint8_t eeprom_addr;
	uint16_t memorysize;
	uint16_t pagesize;
};

/*
 *	CRC check states
 */
enum CRCState{
	NOT_CHECKED,
	CRC_WRONG,
	CRC_OK
};

/*
 *	Storagelib states
 */
typedef enum {
	STORAGELIB_UNINIT = 		0,                   /**< Not initialized.                       */
	STORAGELIB_STOP = 			1,					 /**< Storagelib STOPped	          		 */
  	STORAGELIB_ACTIVE = 		2					 /**< Storagelib ACTIVE 					 */                               
} StorageLib_state_t;

/*
 *	Storagelib return messages
 */
enum {
	STORAGELIB_MEMORY_IN_USE	=	4,				/**< Memory currently in use      	 */
	STORAGELIB_CRC_ERROR		=	3,		 		/**< One of the module has CRC error      	 */
  	STORAGELIB_MEM_CONN_ERROR	=	2,			 	/**< Connection with the memory has failed  */
  	STORAGELIB_WRONG_PARAM		=	1,			 	/**< Wrong parameter  */
  	STORAGELIB_OK				=	0				/**< Operation OK  */
};

/**
 * @brief   Storagelib Init
 * @details It initialize the whole storagelib
 *          @param[in] *config  - Storagelib configuration
 *
 */
StorageLib_state_t InitStorage(struct StorageConfig *config);

/**
 * @brief   Assign a memory address and length to a memory record
 * @details It writes the given parameters into the MMap array
 *          @param[in] Id    - Id of the record
 *          @param[in] *data - Memory address of the data which should be saved
 *          @param[in] len   - Length of the variable/array/object
 *
 */
void AssignToMemoryMap(uint16_t Id, uint8_t* data, uint16_t len);

/**
 * @brief   StoreMapToEeprom
 * @details It stores the whole memory map into the eeprom
 *
 */
uint8_t StoreMapToEeprom(void);

/**
 * @brief   Read map from eeprom
 * @details It reads the whole map from eeprom
 *
 */
uint8_t ReadMapFromEeprom(void);

/**
 * @brief   Store only one record into the eeprom
 * @details It stores only just one record into the eeprom
 *          @param[in] Id  - Id of the record
 *
 */
uint8_t StoreRecordToEeprom(uint16_t Id);

/**
 * @brief   Read only one record from the eeprom
 * @details It reads only just one record from the eeprom
 *          @param[in] Id  - Id of the record
 *
 */
uint8_t ReadRecordFromEeprom(uint16_t Id);

/**
 * @brief   Read one block from the eeprom
 * @details It reads one block from the eeprom. One block consists more record.
 *          @param[in] start  - First record id of the block
 *			@param[in] end    - Last record id of the block
 *
 */
uint8_t ReadBlockFromEeprom(uint16_t start, uint16_t end);

/**
 * @brief   Store one block into the eeprom
 * @details It stores one block into the eeprom. One block consists more record.
 *          @param[in] start  - First record id of the block
 *			@param[in] end    - Last record id of the block
 *
 */
uint8_t StoreBlockToEeprom(uint16_t start, uint16_t end);

/**
 * @brief   Used space
 * @details It gives back the used space from the eeprom
 *
 */
uint16_t GetEepromUsedSpace(void);

/**
 * @brief   Total space
 * @details It gives back the total space of the eeprom
 *
 */
uint16_t GetEepromTotalSpace(void);

/**
 * @brief   Record state
 * @details It gives back CRC check state of the record
 *
 */
uint8_t GetEepromRecordState(uint16_t Id);

#endif