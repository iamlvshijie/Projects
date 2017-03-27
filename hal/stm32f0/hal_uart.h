/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HAL_UART_H
#define __HAL_UART_H


#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "bsp.h"



/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
u32_t hal_uart_DeInit(COM_TypeDef COM);	 
u32_t hal_uart_Init(COM_TypeDef COM,USART_InitTypeDef* USART_InitStructure); 
u32_t hal_uart_Send_Byte(COM_TypeDef COM, uint8_t ch);
u32_t hal_uart_Receive_Byte(COM_TypeDef COM, uint8_t*ch, u32_t*timeout);
	 
u32_t hal_uart_Send_Buffer(COM_TypeDef COM, uint8_t * pkt, int16_t len);
u32_t hal_uart_Receive_Buffer(COM_TypeDef COM,uint8_t * pkt, int16_t len, u32_t*timeout);

u32_t hal_uart_Send_String(COM_TypeDef COM, uint8_t *s);

  
#ifdef __cplusplus
}
#endif

#endif /* __HAL_UART_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
