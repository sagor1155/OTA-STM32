
#ifndef __SERIAL_TASK_BOOTLOADER_H_
#define __SERIAL_TASK_BOOTLOADER_H_


#include "stm32f0xx_hal.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


void serial_begin(void);
void serial_print(const char* ptr);
void serial_println(const char* ptr);
void serial_print_u32(uint32_t data);
void serial_println_u32(uint32_t data);
bool serial_is_hex_received(void);
void serial_clear_hex_receive_flag(void);
void serial_flush_buffer(void);
uint8_t serial_get_hex_string_length(void);
void serial_process_hex_buffer(uint8_t* output_buffer);



#endif  //__SERIAL_TASK_BOOTLOADER_H_
