/*  
	@file:			hal_systick.c
	@author:		Jacob Lui
	@description: hal systick for yd_stm32f0
	@revision:	1.0
	@history:		2017.3.26
	@copyright: Jacob
*/

#include "hal_systick.h"

static __IO u32_t TimingDelay=0;
__IO u32_t TimingOut=0;

u32_t hal_systick_init(u32_t freq)
{
	if (SysTick_Config(SystemCoreClock/freq))
	{ 
		/* Capture error */
		return ERROR;
	
	}
	return SUCCESS;
}

u32_t hal_systick_delay(u32_t us_time)
{
	TimingDelay = us_time;

  while(TimingDelay != 0);

  return SUCCESS;
}

/**
  * @brief  update the systick variable called by systick ISR.
  * @param  None
  * @retval None
  */
void HAL_Systick_Update(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
	if(TimingOut != 0x00)
		TimingOut--;
	
	
}
void HAL_Systick_Set_Timeout(u32_t timeout)
{
	TimingOut = timeout;
}


u32_t* HAL_Systick_Get_Timeout(void)
{
	return &TimingOut;
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	if (TimingDelay != 0)
  { 
    TimingDelay--;
  }
}
/*********************************** END OF FILE *******************************/
