/*  
	@file:			tick.h
	@author:		Jacob Lui
	@description: tick driver for MCU
	@revision:	1.0
	@history:		2017.3.26
	@copyright: Jacob
*/
#include "hal_systick.h"

void tick_start(u32_t freq);
void tick_delay(u32_t time);

/*********************************** END OF FILE *******************************/
