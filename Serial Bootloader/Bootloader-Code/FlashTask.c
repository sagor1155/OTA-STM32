
#include "FlashTask.h"
#include "SerialTask.h"



uint32_t FirstSector = 0, NbOfSectors = 0;
uint32_t SectorError = 0;
__IO uint32_t data32 = 0 , MemoryProgramStatus = 0;



void flash_write_word(uint32_t address, uint32_t data)
{	
	
	HAL_FLASH_Unlock();		
	if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address, data) != HAL_OK)
	{
		serial_print("R/W ERROR");
	}
	else
	{
		data32 = *(__IO uint32_t*)address;
		if (data32 != data)
		{
			serial_print("R/W ERROR");
		}
		else
		{
			serial_print("R/W OK");
		}			
	}
		
	serial_print(" ");
	HAL_FLASH_Lock();
}


void flash_erage_page(uint32_t address, uint8_t number_of_page)
{
	FLASH_EraseInitTypeDef EraseInitStruct;
	EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
	EraseInitStruct.PageAddress = address;
	EraseInitStruct.NbPages = number_of_page;
	
	HAL_FLASH_Unlock();
  if (HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError) != HAL_OK)
	{ 
		serial_print(" Flash Erase ERROR");
	}
  else
	{
		serial_print(" Flash Erase OK");	
	}
		
	serial_print("  ");	
	HAL_FLASH_Lock();
}


