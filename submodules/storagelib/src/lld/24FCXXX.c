#include "ch.h"
#include "hal.h"
#include "storageconf.h"


#if STORAGELIB_USE_EEPROM || defined(__DOXYGEN__)


#if defined(__GNUC__)
__attribute__((aligned (32)))
#endif
static uint8_t txbuf[I2C_BUFFERS_SIZE];   // I2C transmit buffer
 
#if defined(__GNUC__)
__attribute__((aligned (32)))
#endif
static uint8_t rxbuf[I2C_BUFFERS_SIZE];   // I2C receive buffer


uint16_t WAddrPtr = 0x0000;     // Write pointer address
uint16_t RAddrPtr = 0x0000;     // Read pointer address

uint16_t MemorySize = 32768;    // Size in byte organization

I2CDriver *i2cdrv;              // I2C driver pointer for communication

uint8_t EEPRPOM_address = 0x50; // EEPROM address (default 0x50)

uint16_t pagesize = 64;         // Page size (default 64 byte in case 256K memory)

i2cflags_t i2cerror;

/* 
 *  I2C config, 1 MHz I2C clock
 */
static const I2CConfig i2cfg = {
    OPMODE_I2C,
    400000,
    FAST_DUTY_CYCLE_2,
};


/**
 * @brief   Direct EEPROM write function
 * @details Writes the given data and length into the eeprom 
 *          accroding to the given memory address.
 *
 */
msg_t lld_eepromWrite(uint16_t addr, uint8_t *data, uint16_t len){
  msg_t i2cmsg;

  txbuf[0] = (uint8_t)(addr >> 8);
  txbuf[1] = (uint8_t)(addr);

  memcpy(&txbuf[2], data, len);

  //cacheBufferFlush(txbuf, len + 2);

  i2cAcquireBus(i2cdrv);
  i2cmsg = i2cMasterTransmitTimeout(i2cdrv, EEPRPOM_address, &txbuf, len + 2, NULL, 0, MS2ST(I2C_WRITE_TIMEOUT_MS));
  i2cReleaseBus(i2cdrv);

  chThdSleepMilliseconds(5);
  return i2cmsg;
}

/**
 * @brief   Find the next page start address
 * @details Find the next page start address according to the 
 *          given address.
 *          ex. addr = 97 then it returns 128 in case of 64 
 *          byte page size.
 *
 */
uint16_t GetNextPageAddr(uint16_t addr){
  uint16_t counter = 0;
  while(counter <= addr && counter < MemorySize)
  {
    counter += pagesize;
  }
  return counter;
}


/**
 * @brief   Writes the given data to the next available address
 * @details Writes the given data tot the next available address.
 *          The function handles the page write issue. It stops
 *          the writing in a case of a page border and restart it.
 *
 */
msg_t lld_eepromWriteNext(uint8_t *data, uint16_t len){
  msg_t i2cmsg;
  uint16_t BytesLeft = len;
  uint16_t BytesToWrite = 0;

  while (BytesLeft > 0){

    uint16_t NextPageBeginsAt = GetNextPageAddr(WAddrPtr);                        // Find where the next page border is
    uint16_t RemainedBytesInThePage = NextPageBeginsAt - WAddrPtr;                // Calculate how much bytes remained in the page

    /* Calculate how much data needs to be writed in the actual cycle */
    if(BytesLeft < RemainedBytesInThePage)
      BytesToWrite = BytesLeft;
    else
      BytesToWrite = RemainedBytesInThePage;

    /* Write to eeprom */
    i2cmsg = lld_eepromWrite(WAddrPtr, data + (len - BytesLeft), BytesToWrite); 

    BytesLeft = BytesLeft - BytesToWrite;                                         // Calculate how much bytes left to write
    WAddrPtr  = WAddrPtr  + BytesToWrite;                                         // Increase Write addr pointer
  }
  return i2cmsg;
}

/**
 * @brief   EEPROM read function
 * @details It reads the given length from the eeprom and store
 *          it in the data pointer.
 *
 */
msg_t lld_eepromRead(uint16_t addr, uint8_t *data, uint16_t len){
  msg_t i2cmsg;

  txbuf[0] = (uint8_t)(addr >> 8);
  txbuf[1] = (uint8_t)(addr);

  //cacheBufferFlush(txbuf, len);

  i2cAcquireBus(i2cdrv);
  i2cmsg = i2cMasterTransmitTimeout(i2cdrv, EEPRPOM_address, txbuf, 2, &rxbuf, len, MS2ST(I2C_READ_TIMEOUT_MS));
  i2cReleaseBus(i2cdrv);
  
  //cacheBufferInvalidate(rxbuf, len);
  memcpy(data, &rxbuf, len);

  chThdSleepMilliseconds(5);
  return i2cmsg;
}

/**
 * @brief   EEPROM read next available address
 * @details It reads the next available 'len' bytes from the EEPROM
 *
 */
msg_t lld_eepromReadNext(uint8_t *data, uint16_t len){
  msg_t i2cmsg;
  i2cmsg = lld_eepromRead(RAddrPtr, data, len);
  RAddrPtr = RAddrPtr + len;
  return i2cmsg;
}


/**
 * @brief   It sets the write address pointer to the given value
 */
void lld_SetWAddrPtr(uint16_t value)
{
  if(value > MemorySize)
    WAddrPtr = MemorySize;
  else
    WAddrPtr = value;
}

/**
 * @brief   It sets the read address pointer to the given value
 */
void lld_SetRAddrPtr(uint16_t value)
{
  if(value > MemorySize)
    RAddrPtr = MemorySize;
  else
    RAddrPtr = value;
}

/**
 * @brief   EEPROM Init
 * @details It initialize the driver according to the given parameters
 *          @param[in] addr   - EEPROM address
 *          @param[in] msize  - Memory size
 *          @param[in] psize  - Page size
 *
 */
uint8_t lld_eepromInit(I2CDriver *drv, uint8_t addr, uint16_t msize, uint16_t psize){
  i2cdrv = drv;
  EEPRPOM_address = addr;
  MemorySize = msize;
  pagesize = psize;

  /*
   * Starts I2C
   */
  i2cStart(i2cdrv, &i2cfg);

  /*
   *  Test the communication with the eeprom.
   *  Read one byte if there is no timeout,
   *  EEPROM is present.
   */
  uint8_t testbuffer;
  if(lld_eepromReadNext(&testbuffer, 1) == MSG_OK){
    lld_SetRAddrPtr(0x0000);
    return EEPROM_OK;
  }
  else{
    i2cerror = i2cGetErrors(i2cdrv);
    i2cStop(i2cdrv);
    return EEPROM_FAULT;
  }

}

/**
 * @brief   Get latest I2C driver error
 */
i2cflags_t GetI2CErrors(){
  return i2cerror;
}

#endif