#ifndef __SERIAL_H
#define __SERIAL_H
#include "stm32f0xx.h"

typedef struct{
	uint8_t *Name;
	uint32_t BaudRate;
	uint8_t  WordLength;
	uint8_t  StopBits; 
	uint8_t  Parity;   
	uint8_t  TransMode;      
	uint8_t  IntType;
}Serial_ParaDef; 


#endif