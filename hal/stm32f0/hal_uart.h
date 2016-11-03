/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HAL_UART_H
#define __HAL_UART_H


#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "bsp.h"

/** @addtogroup Utilities
  * @{
  */



/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
uint32_t HAL_UART_DeInit(COM_TypeDef COM);	 
uint32_t HAL_UART_Init(COM_TypeDef COM,USART_InitTypeDef* USART_InitStructure); 
uint32_t HAL_UART_Send_Byte(COM_TypeDef COM, uint8_t ch);
uint32_t HAL_UART_Receive_Byte(COM_TypeDef COM, uint8_t*ch, uint32_t*timeout);
	 
uint32_t HAL_UART_Send_Buffer(COM_TypeDef COM, uint8_t * pkt, int16_t len);
uint32_t HAL_UART_Receive_Buffer(COM_TypeDef COM,uint8_t * pkt, int16_t len, uint32_t*timeout);

uint32_t HAL_UART_Send_String(COM_TypeDef COM, uint8_t *s);
/** @defgroup 
  * @{
  */ 

	 
/**
  * @}
  */
  
#ifdef __cplusplus
}
#endif

#endif /* __HAL_UART_H */
/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */

/**
  * @}
  */  

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
