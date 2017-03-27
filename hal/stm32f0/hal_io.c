/*  
	@file:			hal_io.c
	@author:		Jacob Lui
	@description: hal for stm32f0 io
	@revision:	1.0
	@history:		2017.3.26
	@copyright: Jacob
*/
#include "hal_io.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/**
  * @brief  hal_io_init
  * @param  none
  * @retval none
  */
void hal_io_init(io_t* io, io_cfg_t* cfg)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  /* Enable the Clock */
  RCC_AHBPeriphClockCmd(io->periph, ENABLE);
  /* Configure the GPIO_LED pin */
	GPIO_InitStructure.GPIO_Pin = io->pin;
	GPIO_InitStructure.GPIO_Mode = cfg->GPIO_Mode;
	GPIO_InitStructure.GPIO_OType = cfg->GPIO_OType;
	GPIO_InitStructure.GPIO_PuPd = cfg->GPIO_PuPd;
	GPIO_InitStructure.GPIO_Speed = cfg->GPIO_Speed;
	
  GPIO_Init(io->port, &GPIO_InitStructure);
}
/**
  * @brief  hal_io_set
  * @param  none
  * @retval none
  */
void hal_io_set(port_t *port, pin_t pin)
{
	port->BSRR = pin;
}

/**
  * @brief  hal_io_reset
  * @param  none
  * @retval none
  */
void hal_io_reset(port_t* port, pin_t pin)
{
	port->BRR = pin;
}

/**
  * @brief  hal_io_toggle
  * @param  none
  * @retval none
  */
void hal_io_toggle(port_t* port, pin_t pin)
{
  port->ODR ^= pin;
}

/**
  * @brief  hal_io_read
  * @param  none
  * @retval none
  */
pin_t hal_io_read(port_t* port, pin_t pin)
{
	return port->IDR & pin;
}
/*********************************** END OF FILE *******************************/
