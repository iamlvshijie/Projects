/*  
	@file:			led.h
	@author:		Jacob Lui
	@description: led driver for MCUs
	@revision:	1.0
	@history:		2017.3.26
	@copyright: Jacob
*/
#ifndef __LED_H_
#define __LED_H_	 

#include "hal_io.h"



typedef enum
{
	led0,
	led1,
	led2,
	led3,
	led4,
	led5,
	led6,
	led7
}led_t;

typedef enum 
{
  green   = 1,
  red     = 2,
  orange  = 3
}color_t;


typedef enum
{
	led_on,
	led_off
}
led_status_t;

typedef enum 
{
  freq_half_hz,
  freq_1_hz,
  freq_2_hz
}led_freq_t;


void led_init(led_t led);

void led_ctl_on(led_t led);
void led_ctl_off(led_t led);
void led_toggle(led_t led);

void led_blink_init(led_t led, led_freq_t freq);
void led_blink_deinit(led_t led);

#endif /* __LED_H_ */
/*********************************** END OF FILE *******************************/

















