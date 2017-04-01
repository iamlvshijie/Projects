/*  
	@file:			tick.c
	@author:		Jacob Lui
	@description: tick driver for MCU
	@revision:	1.0
	@history:		2017.3.26
	@copyright: Jacob
*/

#include "tick.h"

void tick_start(u32_t freq)
{
	hal_systick_init(freq);
}


void tick_call_delay(tick_func fp, u32_t time)
{
	fp(time);
}


void tick_delay_ms(u32_t time)
{
	hal_systick_delay(time*1000);
}


void tick_delay_us(u32_t time)
{
	hal_systick_delay(time);
}


void	tick_delay(u32_t time)
{
	hal_systick_delay(time);
}


void tick_register_cb(u32_t time, void(*fp))
{

}
/*********************************** END OF FILE *******************************/
