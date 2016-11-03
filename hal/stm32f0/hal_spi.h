#ifndef __HAL_SPI_H_
#define __HAL_SPI_H_
#include "bsp.h"

void 		HAL_SPI_Init(SPI_TypeDef* SPI_x);
void 		HAL_SPI_Send_Byte(SPI_TypeDef* SPI_x,uint8_t data);
uint8_t HAL_SPI_Receive_Byte(SPI_TypeDef* SPI_x,uint8_t data);
static void HAL_SPI_DelayUS(uint8_t t);
uint8_t HAL_SPI_Write_Reg(uint8_t reg,uint8_t value);
uint8_t HAL_SPI_Read_Reg(uint8_t reg);
uint8_t HAL_SPI_Read_Buf(uint8_t reg,uint8_t *pBuf,uint8_t bytes);
uint8_t HAL_SPI_Write_Buf(uint8_t reg,uint8_t *pBuf,uint8_t bytes);

#endif
