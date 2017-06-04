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
io_t console_uart_trx_ios[] ={{GPIOA, GPIO_Pin_9, RCC_AHBPeriph_GPIOA, GPIO_PinSource9, GPIO_AF_1},
															{GPIOA, GPIO_Pin_10, RCC_AHBPeriph_GPIOA, GPIO_PinSource10, GPIO_AF_1}
};
io_cfg_t console_uart_io_cfgs[]={{GPIO_Mode_AF, GPIO_Speed_Level_1, GPIO_OType_PP, GPIO_PuPd_UP},
															{GPIO_Mode_AF, GPIO_Speed_Level_1, GPIO_OType_PP, GPIO_PuPd_UP}
};

uart_io_t console_uart_io = {	&console_uart_trx_ios[0],
															&console_uart_io_cfgs[0],
															&console_uart_trx_ios[1],
															&console_uart_io_cfgs[1]
};		
uart_cfg_t console_uart_cfg ={115200,USART_WordLength_8b,USART_StopBits_1, USART_Parity_No,0};

/*********************************** END OF FILE *******************************/
