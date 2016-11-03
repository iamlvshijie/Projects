#include "hal_uart.h"
#include "bsp.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

USART_TypeDef* COM_USART[COMn] = {COM1_USART, COM2_USART}; 

GPIO_TypeDef* COM_TX_PORT[COMn] = {COM1_TX_GPIO_PORT, COM2_TX_GPIO_PORT};
 
GPIO_TypeDef* COM_RX_PORT[COMn] = {COM1_RX_GPIO_PORT, COM2_RX_GPIO_PORT};

const uint32_t COM_USART_CLK[COMn] = {COM1_CLK, COM2_CLK};

const uint32_t COM_TX_PORT_CLK[COMn] = {COM1_TX_GPIO_CLK, COM2_TX_GPIO_CLK};
 
const uint32_t COM_RX_PORT_CLK[COMn] = {COM1_RX_GPIO_CLK, COM2_RX_GPIO_CLK};

const uint16_t COM_TX_PIN[COMn] = {COM1_TX_PIN, COM2_TX_PIN};

const uint16_t COM_RX_PIN[COMn] = {COM1_RX_PIN, COM2_RX_PIN};
 
const uint16_t COM_TX_PIN_SOURCE[COMn] = {COM1_TX_SOURCE, COM2_TX_SOURCE};

const uint16_t COM_RX_PIN_SOURCE[COMn] = {COM1_RX_SOURCE, COM2_RX_SOURCE};
 
const uint16_t COM_TX_AF[COMn] = {COM1_TX_AF, COM2_TX_AF};
 
const uint16_t COM_RX_AF[COMn] = {COM1_RX_AF, COM2_RX_AF};


uint32_t HAL_UART_Init(COM_TypeDef COM, USART_InitTypeDef* USART_InitStructure)
{
	
	
	//USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;	
	GPIO_InitTypeDef GPIO_InitStructure;
	
  /* Enable GPIO clock */
  RCC_AHBPeriphClockCmd(COM_TX_PORT_CLK[COM] | COM_RX_PORT_CLK[COM], ENABLE);

  /* Enable USART clock */
	//if(IS_RCC_APB2_PERIPH(COM_USART_CLK[COM]))
	if(COM == COM2)
		RCC_APB2PeriphClockCmd(COM_USART_CLK[COM], ENABLE); 
	else //if(IS_RCC_APB1_PERIPH(COM_USART_CLK[COM]))
		RCC_APB1PeriphClockCmd(COM_USART_CLK[COM], ENABLE);
	
  /* Connect PXx to USARTx_Tx */
  GPIO_PinAFConfig(COM_TX_PORT[COM], COM_TX_PIN_SOURCE[COM], COM_TX_AF[COM]);

  /* Connect PXx to USARTx_Rx */
  GPIO_PinAFConfig(COM_RX_PORT[COM], COM_RX_PIN_SOURCE[COM], COM_RX_AF[COM]);
  
  /* Configure USART Tx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = COM_TX_PIN[COM];
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	
  GPIO_Init(COM_TX_PORT[COM], &GPIO_InitStructure);
    
  /* Configure USART Rx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = COM_RX_PIN[COM];
  GPIO_Init(COM_RX_PORT[COM], &GPIO_InitStructure);

  /* USART configuration */
  USART_Init(COM_USART[COM], USART_InitStructure);
    
  /* Enable USART */
  USART_Cmd(COM_USART[COM], ENABLE);
	

	
//  /* USART1 IRQ Channel configuration */
//  NVIC_InitStructure.NVIC_IRQChannel = WIFI_COM_IRQn;
//  NVIC_InitStructure.NVIC_IRQChannelPriority = 0x01;
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);
//  
////	NVIC_InitStructure.NVIC_IRQChannel = COM2_IRQn;
////  NVIC_Init(&NVIC_InitStructure);
//	
//	
//	while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET)
//	{;}
//	
//	/* USART configuration */
//  USART_DeInit(COM_USART[COM]);
//    
//  /* Enable USART */
//  USART_Cmd(COM_USART[COM], DISABLE);
	
	return SUCCESS;
}

uint32_t HAL_UART_DeInit(COM_TypeDef COM)
{
  //return BSP_UART_DeInit(COM);
}	

uint32_t HAL_UART_Send_Byte(COM_TypeDef COM, uint8_t ch)
{
  USART_SendData(COM_USART[COM], ch);
  
  while (USART_GetFlagStatus(COM_USART[COM], USART_FLAG_TXE) == RESET)
  {}
		
	return 0;
}
uint32_t HAL_UART_Receive_Byte(COM_TypeDef COM,uint8_t*ch, uint32_t*timeout)
{
	if(timeout==0)
	{
	  while(USART_GetFlagStatus(COM_USART[COM], USART_FLAG_RXNE) == RESET)
		{
		
		}
		*ch = USART_ReceiveData(COM_USART[COM]);
		return 0;
	}
	else
	{
		
		while(*timeout>0)
		{
			if(USART_GetFlagStatus(COM_USART[COM], USART_FLAG_RXNE) != RESET)
			{
				*ch = USART_ReceiveData(COM_USART[COM]);
				return 0;
			}
		}
	
	}
	return -1;
}


uint32_t HAL_UART_Send_Buffer(COM_TypeDef COM, uint8_t * pkt, int16_t len)
{
	uint8_t* ptr=pkt;
	while(len--)
		HAL_UART_Send_Byte(COM, *(ptr++));
	
	return 0;

}
/**
  * @brief  Print a string on the HyperTerminal
  * @param  s: The string to be printed
  * @retval None
  */
uint32_t HAL_UART_Send_String(COM_TypeDef COM, uint8_t *s)
{
  while (*s != '\0')
  {
    HAL_UART_Send_Byte(COM, *s);
    s++;
  }
	return 0;

}


uint32_t HAL_UART_Receive_Buffer(COM_TypeDef COM, uint8_t * pkt, int16_t len, uint32_t* timeout)
{

	return 0;


}
