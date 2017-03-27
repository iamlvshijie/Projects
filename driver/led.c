/*  
	@file:			led.c
	@author:		Jacob Lui
	@description: led driver for MCUs
	@revision:	1.0
	@history:		2017.3.26
	@copyright: Jacob
*/
#include "led.h"
#include "bsp.h"

#if LED_NUM >0
	extern io_t led_ios[];
	extern io_cfg_t led_io_cfg;
	
/**
  * @brief  led_init
  * @param  None
  * @retval None
  */
void led_init(led_t led)
{
	hal_io_init(&led_ios[led], &led_io_cfg);
}

/**
  * @brief  led_ctl_on
  * @param  None
  * @retval None
  */
void led_ctl_on(led_t led)
{
#if LED_ON_POLARITY
	hal_io_set(led_ios[led].port, led_ios[led].pin); 
#else
	hal_io_reset(led_ios[led].port, led_ios[led].pin); 
#endif
}

/**
  * @brief  led_ctl_off
  * @param  None
  * @retval None
  */
void led_ctl_off(led_t led)
{
#if LED_ON_POLARITY
	hal_io_reset(led_ios[led].port, led_ios[led].pin); 
#else
	hal_io_set(led_ios[led].port, led_ios[led].pin); 
#endif
}

/**
  * @brief  led_ctl_toggle
  * @param  None
  * @retval None
  */
void led_toggle(led_t led)
{
	hal_io_toggle(led_ios[led].port, led_ios[led].pin);
}






/**
  * @brief  led_blink_init
  * @param  None
  * @retval None
  */
void led_blink_init(led_t led, led_freq_t freq)
{


}

/**
  * @brief  led_blink_deinit
  * @param  None
  * @retval None
  */
void led_blink_deinit(led_t led)
{

}

#else
	#warning please include led on bsp.c file
#endif
/*********************************** END OF FILE *******************************/

