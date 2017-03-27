/*  
	@file:			hal_io.c
	@author:		Jacob Lui
	@description: hal for stm32f0 io
	@revision:	1.0
	@history:		2017.3.26
	@copyright: Jacob
*/
#ifndef __HAL_IO_H_
#define __HAL_IO_H_

#include "hal_core.h"

typedef u16_t pin_t;
typedef u32_t periph_t;
typedef GPIO_TypeDef port_t;

typedef struct
{
	port_t* port;
	pin_t pin;
	periph_t periph;
}io_t;

typedef struct 
	{
		GPIOMode_TypeDef GPIO_Mode;    
		GPIOSpeed_TypeDef GPIO_Speed;   
		GPIOOType_TypeDef GPIO_OType;   
		GPIOPuPd_TypeDef GPIO_PuPd;
	}io_cfg_t;
	
typedef enum
{
	IO_Normal = 0,
	IO_Interrupt = 1
} io_mode_enum;


void 		hal_io_init(io_t* io, io_cfg_t* cfg);
void 		hal_io_set(port_t* port, pin_t pin);
void 		hal_io_reset(port_t* port, pin_t pin);
void 		hal_io_toggle(port_t* port, pin_t pin);
pin_t   hal_io_read(port_t* port, pin_t pin);

#endif /* __HAL_IO_H_ */
/*********************************** END OF FILE *******************************/
