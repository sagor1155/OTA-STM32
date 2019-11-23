
#include "Bootloader.h" 
#include "FlashTask.h"
#include "SerialTask.h"


static uint8_t hex_buff[50];
static uint32_t base_address = 0;

typedef void (*pFunction)(void);
static bool flash_load_finished = false;



void bootloader_begin(void)
{
	flash_load_finished = false;
	memset(hex_buff, 0, sizeof(hex_buff));
}


void bootloader_update_firmware(void)
{
	bootloader_check_for_incoming_firmware();
	
	if(HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == GPIO_PIN_SET)
	{
		uint32_t start_time = HAL_GetTick();
		while((HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) != GPIO_PIN_RESET) && (HAL_GetTick()-start_time)<300){}
		if(HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == GPIO_PIN_RESET)
		{
			serial_println("Going to jump to application address");
			HAL_Delay(500);
			bootloader_jump_to_main_application();
		}				
	}
}


bool bootloader_check_application_address(void)
{
	return ( (*(__IO uint32_t*)APP_ADDRESS != 0xFFFFFFFF) ? true : false );	
}


void bootloader_jump_to_main_application(void)
{
	if(bootloader_check_application_address())
	{
		uint32_t jump_address = *(__IO uint32_t*) (APP_ADDRESS + 4);
		pFunction jump_to_application = (pFunction) jump_address;
		
    HAL_RCC_DeInit();
    HAL_DeInit();
    
    SysTick->CTRL = 0;
    SysTick->LOAD = 0;
    SysTick->VAL  = 0;
		
		__disable_irq(); 

		__set_MSP(*(__IO uint32_t*) APP_ADDRESS);
		jump_to_application();
	}	
}


uint8_t bootloader_calculate_checksum(uint8_t *input_buff)
{
  uint8_t checksum = 0;
	
  for(int i = 0; i<(input_buff[0]+4); i++)
    checksum = (uint8_t)((checksum + input_buff[i]) & 0xff);
  
  checksum = ~checksum;
  checksum = checksum + 1;
  
	return checksum;
}



/*
TO DO:
	1. check flash start address, end address, page size & number
  2. check time gap between two data packet & reject if timeout occures 
  3. keep session flag & clear after flash load finishes or exception occures 
  4. build & parse packet for transfering hex string data 

*/
void bootloader_check_for_incoming_firmware(void)
{
	uint8_t program_data_length = 0, record_type = 0, checksum_byte = 0;
	uint16_t offset_address = 0;
	uint32_t program_data = 0;	
	
	if(serial_is_hex_received())
	{
		serial_process_hex_buffer(&hex_buff[0]);
			
		program_data_length = hex_buff[0];
    offset_address = hex_buff[1]<<8 | hex_buff[2];
    record_type = hex_buff[3];
			
		switch(record_type)
		{
			case RECORD_TYPE_START_ADDR:
				base_address = (hex_buff[4]<<8 | hex_buff[5]) << 16 | 0x0000;
				serial_print_u32(base_address);
				serial_print("  ");
					
				base_address = 0x08000000;    				//temporary. for testing only
				flash_erage_page(APP_ADDRESS , 25);
				break;
			
			case RECORD_TYPE_FLASH_DATA:			
				if(program_data_length != 0)
				{
					for(int i=0; i<program_data_length; i+=4)
					{
						program_data = (uint32_t)(hex_buff[4+i]<<0 | hex_buff[5+i]<<8 | hex_buff[6+i]<<16 | hex_buff[7+i]<<24);
							
						serial_print_u32(base_address|offset_address);
						serial_print(" ");							
						//serial_print_u32(program_data);
						//serial_print(" ");						
							
						flash_write_word(base_address|offset_address, program_data);
						offset_address = offset_address + 4;
							
						//serial_print("....");
					}
				}
				break;				
					
			case RECORD_TYPE_END_OF_FILE:
				serial_print(" Flash Write Done");
				flash_load_finished = true;
				break;
				
			default:
				break;
		}

		checksum_byte = bootloader_calculate_checksum(&hex_buff[0]);
			
		serial_print("\n");
		serial_clear_hex_receive_flag();
		serial_flush_buffer();
		memset(hex_buff, 0, sizeof(hex_buff));
	}
			
	if(flash_load_finished == true)
	{
		bootloader_jump_to_main_application();
	}
	
}



