
#ifndef __BOOTLOADER_STM32F051R8T6_H_
#define __BOOTLOADER_STM32F051R8T6_H_



#include <cstdint>
#include <string.h>
#include <stdbool.h>
#include "stm32f0xx_hal.h"



#define RECORD_TYPE_START_ADDR    0x04u
#define RECORD_TYPE_FLASH_DATA    0x00u
#define RECORD_TYPE_END_OF_FILE   0x01u

#define APP_ADDRESS 							0x08004000u
#define FLASH_SIZE                0x10000u           	 	//64K
#define FLASH_SIZE_OF_PAGE        FLASH_PAGE_SIZE    		//1K
#define FLASH_TOTAL_PAGE_NUMBER   64u
#define FLASH_END_ADDRESS         0x0800FFFCu 


void bootloader_begin(void);
void bootloader_update_firmware(void);

uint8_t bootloader_calculate_checksum(uint8_t *input_buff);
bool bootloader_check_application_address(void);
void bootloader_jump_to_main_application(void);
void bootloader_check_for_incoming_firmware(void);


#endif  //__BOOTLOADER_STM32F051R8T6_H_
