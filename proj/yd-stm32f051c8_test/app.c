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

/* Private functions ---------------------------------------------------------*/


/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
	
	led_init(led0);
	tick_start(1000000);
	
	while(1)
	{
		tick_delay(1000000);
		led_toggle(led0);
	}
	return 0;
}
/*********************************** END OF FILE *******************************/
