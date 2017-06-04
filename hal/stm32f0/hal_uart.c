
/* Includes ------------------------------------------------------------------*/
#include "hal_uart.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
USART_TypeDef* stm32_usart[] = {USART1,USART2};

/**
  * @brief  send bytes into the uart_x
  * @param  str: The string to be printed
  * @retval None
  */
u32_t hal_uart_init(uart_num uart_x, uart_io_t* uart_x_io, uart_cfg_t* uart_x_cfg)
{
	USART_InitTypeDef USART_InitStructure;
  //NVIC_InitTypeDef NVIC_InitStructure;	
	GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable GPIO clock */
  //RCC_AHBPeriphClockCmd(uart_x_io->rx_io->periph | uart_x_io->tx_io->periph, ENABLE);

  /* Connect PXx to USARTx_Tx */
  //GPIO_PinAFConfig(uart_x_io->tx_io->port, uart_x_io->tx_io->pin_source, uart_x_io->tx_io->pin_af);
	//GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);
	
  /* Connect PXx to USARTx_Rx */
  //GPIO_PinAFConfig(uart_x_io->rx_io->port, uart_x_io->rx_io->pin_source, uart_x_io->rx_io->pin_af);
  //GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);
	
//  /* Configure USART Tx as alternate function push-pull */
//  GPIO_InitStructure.GPIO_Pin = uart_x_io->tx_io->pin;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//	
//  GPIO_Init(uart_x_io->tx_io->port, &GPIO_InitStructure);
//    
//  /* Configure USART Rx as alternate function push-pull */
//  GPIO_InitStructure.GPIO_Pin = uart_x_io->rx_io->pin;
//  GPIO_Init(uart_x_io->rx_io->port, &GPIO_InitStructure);

//  /* Enable USART clock */
//	if(uart_x == uart1)
//	{
//		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); 		
//	}
//	else if(uart_x == uart2)
//	{
//		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
//	}
//	else
//		;
//	
	/* USART configuration */
	USART_InitStructure.USART_BaudRate = uart_x_cfg->baud_rate;
	USART_InitStructure.USART_Parity = uart_x_cfg->parity;
	USART_InitStructure.USART_StopBits = uart_x_cfg->stop_bits;
	
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//Yá÷¿ØÖÆ
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
 // USART_Init(stm32_usart[uart_x], &USART_InitStructure);
	USART_Init(USART1, &USART_InitStructure);		
    
  /* Enable USART */
  //USART_Cmd(stm32_usart[uart_x], ENABLE);
	

	
//  /* USART1 IRQ Channel configuration */
//  NVIC_InitStructure.NVIC_IRQChannel = WIFI_COM_IRQn;
//  NVIC_InitStructure.NVIC_IRQChannelPriority = 0x01;
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);
//  
////	NVIC_InitStructure.NVIC_IRQChannel = COM2_IRQn;
////  NVIC_Init(&NVIC_InitStructure);
	
	
//	while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET)
//	{;}
//	
//	/* USART configuration */
//  USART_DeInit(stm32_usart[uart_x]);
//    
//  /* Enable USART */
//  USART_Cmd(stm32_usart[uart_x], DISABLE);
	
	return SUCCESS;
}

/**
  * @brief  send bytes into the uart_x
  * @param  str: The string to be printed
  * @retval None
  */
u32_t hal_uart_deinit(uart_num uart_x)
{
  USART_DeInit(stm32_usart[uart_x]);
}	

/**
  * @brief  send byte into the uart_x
  * @param  str: The string to be printed
  * @retval None
  */
u32_t hal_uart_send_byte(uart_num uart_x, u8_t ch)
{
  USART_SendData(stm32_usart[uart_x], ch);
  
  while (USART_GetFlagStatus(stm32_usart[uart_x], USART_FLAG_TXE) == RESET)
  {}
		
	return 0;
}

/**
  * @brief  receive byte from the uart_x
  * @param  str: The string to be printed
  * @retval None
  */
u32_t hal_uart_receive_byte(uart_num uart_x, u8_t*ch, u32_t timeout)
{
	if(timeout==0)
	{
	  while(USART_GetFlagStatus(stm32_usart[uart_x], USART_FLAG_RXNE) == RESET)
		{
		
		}
		*ch = USART_ReceiveData(stm32_usart[uart_x]);
		return 0;
	}
	else
	{
		
		while(timeout>0)
		{
			if(USART_GetFlagStatus(stm32_usart[uart_x], USART_FLAG_RXNE) != RESET)
			{
				*ch = USART_ReceiveData(stm32_usart[uart_x]);
				return 0;
			}
		}
	
	}
	return -1;
}

/**
  * @brief  send bytes into the uart_x
  * @param  str: The string to be printed
  * @retval None
  */
u32_t hal_uart_send_bytes(uart_num uart_x, u8_t * bytes, u16_t len)
{
	u8_t* ptr= bytes;
	while(len--)
		hal_uart_send_byte(uart_x, *(ptr++));
	return 0;

}

/**
  * @brief  send a string into the uart_x
  * @param  str: The string to be printed
  * @retval None
  */
u32_t hal_uart_send_string(uart_num uart_x, u8_t *str)
{
  while (*str != '\0')
  {
    hal_uart_send_byte(uart_x, *str);
    str++;
  }
	return 0;
}

/**
  * @brief  receive bytes from the uart_x
  * @param  
  * @retval None
  */
u32_t hal_uart_receive_bytes(uart_num uart_x, u8_t * bytes, u16_t len, u32_t* timeout)
{
	return 0;
}
