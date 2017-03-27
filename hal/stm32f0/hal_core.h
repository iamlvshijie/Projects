/*  
	@file:			hal_core.h
	@author:		Jacob Lui
	@description: hal core for yd_stm32f0
	@revision:	1.0
	@history:		2017.3.26
	@copyright: Jacob
*/

#ifndef __HAL_CORE_H
#define __HAL_CORE_H

/* core configurations for MCU */
#include "stm32f0xx.h"
typedef uint32_t u32_t;
typedef uint16_t u16_t;
typedef uint8_t u8_t;

void int_all_disable(void);
void int_all_enable(void);	
void msr_msp(u32_t addr);	

#endif
/*********************************** END OF FILE *******************************/
