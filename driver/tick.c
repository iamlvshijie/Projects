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

void tick_delay(u32_t time)
{
	hal_systick_delay(time);
}

/*********************************** END OF FILE *******************************/
