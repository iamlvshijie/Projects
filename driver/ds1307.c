#include "ds1307.h"
#include "hal_i2c.h"

I2C_TypeDef* DS1307_I2C;

uint32_t DS1307_Init(I2C_TypeDef* I2Cx)
{
	uint8_t rxData=0,*pBuffer;
	DS1307_I2C= I2Cx;
	
	pBuffer = &rxData;
	
	if(HAL_I2C_ReadBytes(DS1307_I2C,DS1307_I2C_Address,0x00,1,pBuffer))
		return ERROR;
	if(*pBuffer&0x80){
		(*pBuffer) &=0x7F;
		if(HAL_I2C_WriteBytes(DS1307_I2C,DS1307_I2C_Address,0x00,1,pBuffer))
			return ERROR;
	}
}

uint32_t DS1307_ReadTime(uint8_t* pBuffer)
{
	return HAL_I2C_ReadBytes(DS1307_I2C,DS1307_I2C_Address,0x00,7,pBuffer);
}
uint32_t DS1307_SetTime(uint8_t* pBuffer)
{
	return HAL_I2C_WriteBytes(DS1307_I2C,DS1307_I2C_Address,0x00,7,pBuffer);
}

uint32_t DS1307_SetClkOut()
{
	return 0;
}
