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

/* Private functions ---------------------------------------------------------*/


/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
	serial_cfg_t serial_cfg;

	
	led_init(led0);
	tick_start(1000000);
	
	serial_cfg.uart = uart0;
	serial_cfg.uart_io = &console_uart_io;
	serial_cfg.uart_cfg = &console_uart_cfg;

	console_init(&serial_cfg);
	
	printf(">>system initiated\r\n");
	
	while(1)
	{
		tick_delay(1000000);
		led_toggle(led0);
	}
	return 0;
}
/*********************************** END OF FILE *******************************/
