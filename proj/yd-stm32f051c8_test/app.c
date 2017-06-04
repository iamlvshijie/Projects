/*  
	@file:			app.c
	@author:		Jacob Lui
	@description: app for yd_stm32f051c8 board
	@revision:	1.0
	@history:		2017.3.26
	@copyright: Jacob
*/

/* Includes ------------------------------------------------------------------*/
#include "app.h"

#include "bsp.h"

#include "led.h"
#include "tick.h"
#include "serial.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/																																

/* Private function prototypes -----------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
extern uart_io_t console_uart_io;
extern uart_cfg_t console_uart_cfg;

extern io_cfg_t led_io_cfg;
/* Private functions ---------------------------------------------------------*/
void USART1_Init(void);

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
	serial_cfg_t serial_cfg;

	led_init(led0, &led_io_cfg);
	tick_start(1000000);
	
	serial_cfg.uart = uart0;
	serial_cfg.uart_io = &console_uart_io;
	serial_cfg.uart_cfg = &console_uart_cfg;

	//console_init(&serial_cfg);
	USART1_Init();
	//printf(">>system initiated\r\n");
	
	while(1)
	{
		tick_delay(1000000);
		led_ctl_toggle(led0);
	}

}
/*********************************** END OF FILE *******************************/
void USART1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);  //??GPIOA???
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);//??USART???
	/* USART1????? */
//	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);//??PA9???????	TX
//	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);//??PA10???????  RX	

//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
	

	/* USART1????? */
	USART_InitStructure.USART_BaudRate = 115200;              //???
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART1, &USART_InitStructure);		
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);           //??????
	USART_Cmd(USART1, ENABLE);                             //??USART1
	
	/* USART1?NVIC???? */
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPriority = 0x02;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
				
}