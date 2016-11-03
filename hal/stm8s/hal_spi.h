#ifndef __HAL_SPI_H
#define __HAL_SPI_H

#include "bsp.h"
/* Private typedef -----------------------------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

/* Private define ------------------------------------------------------------*/

#define HAL_SPI_CS_LOW()       GPIO_WriteLow(SPI_CS_PORT, SPI_CS_PIN)
#define HAL_SPI_CS_HIGH()      GPIO_WriteHigh(SPI_CS_PORT, SPI_CS_PIN)

void HAL_SPI_Init(void);
u8 HAL_SPI_Send_Byte(u8 byte);
u8 HAL_SPI_Receive_Byte(void);
u8 HAL_SPI_Write_Reg(u8 regAddr, u8 data);
u8 HAL_SPI_Read_Reg(u8 regAddr);
void HAL_SPI_Write_Buffer(u8 WriteAddr, u8* pBuffer, u16 NumByteToWrite);
void HAL_SPI_Read_Buffer(u8 ReadAddr, u8* pBuffer, u16 NumByteToRead);
#endif