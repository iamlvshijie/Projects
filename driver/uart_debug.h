#include <stdio.h>

#include "stm32f0xx.h"

#include "hal_uart.h"

#ifndef __UART_DEBUG_H_
#define __UART_DEBUG_H_


#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
	

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{	
	HAL_UART_Send_Byte(DEBUG_COM, (uint8_t) ch);

  return ch;
}	

#endif /* __UART_DEBUG_H_ */

