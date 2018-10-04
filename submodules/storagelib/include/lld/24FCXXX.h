#ifndef LLDEEPROM_H_INCLUDED
#define LLDEEPROM_H_INCLUDED

#if CH_KERNEL_MAJOR >= 5
#define MS2ST(x) TIME_MS2I(x)
#endif

/**
 * @brief   EEPROM Init
 * @details It initialize the driver according to the given parameters
 *          @param[in] addr   - EEPROM address
 *          @param[in] msize  - Memory size
 *          @param[in] psize  - Page size
 *
 */
uint8_t lld_eepromInit(I2CDriver *drv, uint8_t addr, uint16_t msize, uint16_t psize);

/**
 * @brief   Writes the given data to the next available address
 * @details Writes the given data tot the next available address.
 *          The function handles the page write issue. It stops
 *          the writing in a case of a page border and restart it.
 *
 */
msg_t lld_eepromWriteNext(uint8_t *data, uint16_t len);

/**
 * @brief   EEPROM read next available address
 * @details It reads the next available 'len' bytes from the EEPROM
 *
 */
msg_t lld_eepromReadNext(uint8_t *data, uint16_t len);

/**
 *
 *	Enumeration for standard memory sizes
 */
enum{
	MEM_128K = 16384,
	MEM_256K = 32768,
	MEM_512K = 65536
};

/**
 *
 *	Enumeration for standard page sizes
 */
enum{
	PAGE_64 = 64,
	PAGE_128 = 128
};

/**
 *
 *	Enumeration for EEPROM state
 */
enum{
	EEPROM_OK = 0,
	EEPROM_FAULT = -1
};

#define I2C_BUFFERS_SIZE   		200U		// I2C rx, tx buffer size
#define DEFAULT_EEPROM_ADDR 	0x50		// Default eeprom address

#define I2C_WRITE_TIMEOUT_MS	10			// Write operation timout in ms
#define I2C_READ_TIMEOUT_MS		500			// Read operation timout in ms


#endif