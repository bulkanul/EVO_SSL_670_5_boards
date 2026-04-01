#include "params_func.h"

ErrorStatus flash_save_config_data (uint32_t* array_to_write, int size, uint32_t AddressDes)
{

	uint32_t* AddressSrc = array_to_write;
	uint32_t PageError = 0;
	uint16_t i;

	FLASH_EraseInitTypeDef EraseInitStruct;

	HAL_FLASH_Unlock();

	EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
	EraseInitStruct.Sector = PARAMS_MEMORY_FLASH_SECTOR;
	EraseInitStruct.NbSectors = 1;
	EraseInitStruct.Banks = FLASH_BANK_1;
	EraseInitStruct.VoltageRange = VOLTAGE_RANGE_3;

	if (HAL_FLASHEx_Erase(&EraseInitStruct, &PageError) != HAL_OK) {

		HAL_FLASH_Lock();
		return ERROR;

	}

	for (i=0;i<size/4;i++) {

		 if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, AddressDes, *(uint32_t *)AddressSrc) == HAL_OK) {

				AddressDes+=4;
				AddressSrc++;

		 }
		 else {

				HAL_FLASH_Lock();
				return ERROR;

		 }

	}

	HAL_FLASH_Lock();
	return SUCCESS;
	
}

ErrorStatus flash_read_config_data (uint32_t* read_to_array, int size, uint32_t AddressSrc)
{
	
	uint32_t* AddressDes = read_to_array;
	uint32_t PageError = 0;
	uint16_t i;

	HAL_FLASH_Unlock();

	for (i=0;i<size/4;i++) {
		 
		read_to_array[i] = flash_read_u32(AddressSrc);
		AddressSrc += 4;
		
	}
	
	HAL_FLASH_Lock();
	return SUCCESS;

	UNUSED(PageError);
	UNUSED(AddressDes);
	
}

uint32_t flash_read_u32(uint32_t address)
{
    return (*(__IO uint32_t*)address);
}
