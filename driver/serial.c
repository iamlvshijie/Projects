#include "hal_uart.h"
#include <stdio.h>
#include "serial.h"

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
	#define GETCHAR_PROTOTYPE int fgetc(FILE *f)
#endif /* __GNUC__ */
	
/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
	* @note Need MicroLib supporting;
  */
PUTCHAR_PROTOTYPE
{      
	HAL_UART_Send_Byte(DEBUG_COM, (uint8_t)ch);    
	return ch;
}
/**
  * @brief  Retargets the C library scanf function to the USART.
  * @param  None
  * @retval None
	* @note Need MicroLib supporting;
  */
GETCHAR_PROTOTYPE
{ 
	uint8_t ch;
	/*HAL_UART_Receive_Byte(DEBUG_COM,&ch, 0);
	return ch;*/
	
	while(USART_GetFlagStatus(USART1,USART_FLAG_RXNE) == RESET)
	{
	}
	ch = USART_ReceiveData(USART1);
	HAL_UART_Send_Byte(DEBUG_COM, (uint8_t)ch);  
	return ch;
}

void Serial_Init(Serial_ParaDef para)
{


}