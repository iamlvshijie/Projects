#include "pcf8563.h"
#include "hal_i2c.h"


I2C_TypeDef* PCF8563_I2C;

uint32_t PCF8563_Init(I2C_TypeDef* I2Cx)
{
	uint8_t txData[2]={0,0},*pBuffer;
	PCF8563_I2C= I2Cx;
	pBuffer = txData;
	return HAL_I2C_WriteBytes(PCF8563_I2C, PCF8563_I2C_Address,CTRL_STATUS_REG1_ADDR,2,pBuffer);
}

uint32_t PCF8563_ReadTime(uint8_t* pBuffer)
{
	return HAL_I2C_ReadBytes(PCF8563_I2C, PCF8563_I2C_Address,SECOND_REG_ADDR,7,pBuffer);
}
uint32_t PCF8563_SetTime(uint8_t* pBuffer)
{
	return HAL_I2C_WriteBytes(PCF8563_I2C,PCF8563_I2C_Address,SECOND_REG_ADDR,7,pBuffer);
}
