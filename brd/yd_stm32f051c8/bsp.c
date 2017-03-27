/*  
	@file:			bsp.c
	@author:		Jacob Lui
	@description: bsp for yd_stm32f051c8 board
	@revision:	1.0
	@history:		2017.3.26
	@copyright: Jacob
*/

#include "bsp.h"

/* led */
#if LED_NUM >0
io_t led_ios[LED_NUM] ={{GPIOC,GPIO_Pin_13,RCC_AHBPeriph_GPIOC} };
io_cfg_t led_io_cfg = {GPIO_Mode_OUT,GPIO_Speed_Level_1,GPIO_OType_PP,GPIO_PuPd_NOPULL};

#endif
/*********************************** END OF FILE *******************************/
