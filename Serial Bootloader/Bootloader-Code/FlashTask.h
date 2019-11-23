
#ifndef __FLASH_TASK_BOOTLOADER_H_
#define __FLASH_TASK_BOOTLOADER_H_

#include <cstdint>
#include "stm32f0xx_hal.h"


void flash_write_word(uint32_t address, uint32_t data);
void flash_erage_page(uint32_t address, uint8_t number_of_page);


#endif  //__FLASH_TASK_BOOTLOADER_H_
