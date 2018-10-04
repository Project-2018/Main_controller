#ifndef STORAGELIB_STORAGECONF_H_
#define STORAGELIB_STORAGECONF_H_

#include "storage.h"


/**
 * @file    storageconf.h
 * @brief   storagelib configuration header
 * @details Here you can change the storagelib settings.
 *
 * @addtogroup storagelib_CONF
 * @{
 */

#define STORAGELIB_VERSION 1.2


/**
 * @brief   Enables the storagelib eeprom driver subsystem.
 */
#if !defined(STORAGELIB_USE_EEPROM) || defined(__DOXYGEN__)
#define STORAGELIB_USE_EEPROM      TRUE
#endif


/**
 * @brief   Enables the storagelib flash subsystem.
 */
#if !defined(STORAGELIB_USE_FLASH) || defined(__DOXYGEN__)
#define STORAGELIB_USE_FLASH       FALSE
#endif


#if !defined(STORAGELIB_DYN_MEM_ALLOC) || defined(__DOXYGEN__)
#define STORAGELIB_DYN_MEM_ALLOC   TRUE
#endif


#if STORAGELIB_DYN_MEM_ALLOC

#include "memorymap.h"

#else

#define MAX_RECORD_NUMBER	50

#endif



#if STORAGELIB_USE_EEPROM

#include "24FCXXX.h"

#endif


#endif 