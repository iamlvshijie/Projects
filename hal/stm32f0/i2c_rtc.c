#include "i2c_rtc.h"
#include "hal_i2c.h"

I2C_TypeDef* RTC_I2C;
//uint8_t PCF8563_I2C_Address=0;

uint32_t I2C_RTC_Init(I2C_TypeDef* I2Cx)
{
	uint8_t txData[2]={0,0},*pBuffer;
	RTC_I2C= I2Cx;
	
	
//#if defined PCF8563	
//	PCF8563_I2C_Address = PCF8563_I2C_Address;
	pBuffer = txData;
	if(HAL_I2C_WriteBytes(RTC_I2C, PCF8563_I2C_Address,0x00,2,pBuffer))
		return ERROR;
	
//#elif defined DS1307
//	PCF8563_I2C_Address = DS1307_I2C_Address;//;//;PCF8563_I2C_Address
//	pBuffer = &rxData;
//	HAL_I2C_ReadBytes(RTC_I2C, PCF8563_I2C_Address,0x00,1,pBuffer);
//	if(*pBuffer&0x80){
//		(*pBuffer) &=0x7F;
//		HAL_I2C_WriteBytes(RTC_I2C, PCF8563_I2C_Address,0x00,1,pBuffer);
//	}
//#endif
}

uint32_t I2C_RTC_ReadTime(uint8_t* pBuffer)
{
	return HAL_I2C_ReadBytes(RTC_I2C, PCF8563_I2C_Address,0x02,7,pBuffer);
}
uint32_t I2C_RTC_SetTime(uint8_t* pBuffer)
{
	return HAL_I2C_WriteBytes(RTC_I2C,PCF8563_I2C_Address,0x00,7,pBuffer);
}
