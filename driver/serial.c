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


uart_num console_uart;
	
/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
	* @note Need MicroLib supporting;
  */
PUTCHAR_PROTOTYPE
{      
	hal_uart_send_byte(console_uart, (uint8_t)ch);    
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
	u8_t ch;
	
	hal_uart_receive_byte(console_uart, &ch, 0);
	return ch;
	
//	while(USART_GetFlagStatus(USART1,USART_FLAG_RXNE) == RESET)
//	{
//	}
//	ch = USART_ReceiveData(USART1);
	
	//echo for console 
	hal_uart_send_byte(console_uart, (uint8_t)ch);  
	return ch;
}

void console_init(serial_cfg_t* serial_cfg)
{
	console_uart = serial_cfg->uart;
	hal_uart_init(console_uart, serial_cfg->uart_io, serial_cfg->uart_cfg);
}

void serial_init(serial_cfg_t* serial_cfg)
{
	hal_uart_init(console_uart, serial_cfg->uart_io, serial_cfg->uart_cfg);
}