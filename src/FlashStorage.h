#ifndef FlashStorage_h
#define FlashStorage_h

#ifdef __cplusplus
extern "C" {
#endif

#include "sam.h"
#include <Arduino.h>

extern char __etext;
extern char __data_start;


#define NVM_CMD_ER           0x02
#define NVM_CMD_WP           0x04


/**
 * @brief Available memory in flash
 * 
 * @return uint32_t The amount of free memory in flash.
 */
uint32_t flash_free_memory(void);

/**
 * @brief Initialize flash memory
 * 
 */
void flash_init(void);

void flash_flush();

void *flash_write(void *data, uint32_t size);

#ifdef __cplusplus
}
#endif
#endif