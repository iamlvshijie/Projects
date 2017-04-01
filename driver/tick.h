/*  
	@file:			tick.h
	@author:		Jacob Lui
	@description: tick driver for MCU
	@revision:	1.0
	@history:		2017.3.26
	@copyright: Jacob
*/
#include "hal_systick.h"

typedef void (*tick_func)(int );


void tick_start(u32_t freq);
void tick_delay(u32_t time);
void tick_delay_ms(u32_t time);
void tick_delay_us(u32_t time);
void tick_call_delay(tick_func fp, u32_t time);
void tick_register_cb(u32_t time, void(*fp));
void tick_unregister_cb(u32_t time, void(*fp));
/*********************************** END OF FILE *******************************/
