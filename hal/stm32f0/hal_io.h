#ifndef __HAL_IO_H_
#define __HAL_IO_H_
#include "bsp.h"
extern const uint16_t GPIO_PIN[];
extern GPIO_TypeDef* GPIO_PORT[];
extern const uint32_t GPIO_CLK[];
	
typedef enum
{
	IO_Normal = 0,
	IO_Interrupt = 1
} IO_ModeDef;


void 		HAL_IO_Init(IO_TypeDef io);
void 		HAL_IO_High(IO_TypeDef io);
void 		HAL_IO_Low(IO_TypeDef io);
void 		HAL_IO_Toggle(IO_TypeDef io);
uint16_t HAL_IO_Read(IO_TypeDef io);

#endif /* __HAL_IO_H_ */
