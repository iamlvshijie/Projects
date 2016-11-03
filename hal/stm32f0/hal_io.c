
#include "hal_io.h"
#include "bsp.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
const uint16_t GPIO_PIN[IOn] = {IO0_PIN,
																IO1_PIN,
															  IO2_PIN,
																IO3_PIN,
																IO4_PIN,
																IO5_PIN,
																IO6_PIN};

GPIO_TypeDef* GPIO_PORT[IOn] = {IO0_GPIO_PORT, 
																IO1_GPIO_PORT,
																IO2_GPIO_PORT,
																IO3_GPIO_PORT,
	                              IO4_GPIO_PORT,
																IO5_GPIO_PORT,
																IO6_GPIO_PORT};

const uint32_t GPIO_CLK[IOn] = {IO0_GPIO_CLK,
																IO1_GPIO_CLK,
																IO2_GPIO_CLK,
																IO3_GPIO_CLK,
																IO4_GPIO_CLK,
																IO5_GPIO_CLK,
																IO6_GPIO_CLK};

void HAL_IO_Init(IO_TypeDef io)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  /* Enable the GPIO_LED Clock */
  RCC_AHBPeriphClockCmd(GPIO_CLK[io], ENABLE);

  /* Configure the GPIO_LED pin */
  GPIO_InitStructure.GPIO_Pin = GPIO_PIN[io];
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIO_PORT[io], &GPIO_InitStructure);
}

void HAL_IO_High(IO_TypeDef io)
{
	GPIO_PORT[io]->BSRR = GPIO_PIN[io];
}
void HAL_IO_Low(IO_TypeDef io)
{
	GPIO_PORT[io]->BRR = GPIO_PIN[io];
}

void HAL_IO_Toggle(IO_TypeDef io)
{
  GPIO_PORT[io]->ODR ^= GPIO_PIN[io];
}

uint16_t HAL_IO_Read(IO_TypeDef io)
{
	return GPIO_PORT[io]->IDR & GPIO_PIN[io];
}
