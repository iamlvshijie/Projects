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

/* uart*/
io_t console_uart_trx_ios[] ={{GPIOA,GPIO_Pin_9,RCC_AHBPeriph_GPIOA},{GPIOA,GPIO_Pin_10,RCC_AHBPeriph_GPIOA}};
io_cfg_t console_uart_cfgs[]={{GPIO_Mode_OUT,GPIO_Speed_Level_1,GPIO_OType_PP,GPIO_PuPd_NOPULL},{GPIO_Mode_OUT,GPIO_Speed_Level_1,GPIO_OType_PP,GPIO_PuPd_NOPULL}};

uart_io_t console_uart_io = {&console_uart_trx_ios[1],&console_uart_cfgs[1],&console_uart_trx_ios[2],&console_uart_cfgs[2]};		
uart_cfg_t console_uart_cfg ={115200,USART_WordLength_8b,USART_StopBits_1, USART_Parity_No};

/*********************************** END OF FILE *******************************/
