#include "hal_systick.h"

static __IO uint32_t TimingDelay=0;
__IO uint32_t TimingOut=0;


uint32_t HAL_Systick_Init(void)
{
	if (SysTick_Config(SystemCoreClock/100000))
	{ 
		/* Capture error */
		return ERROR;
	
	}
	return SUCCESS;
}

uint32_t HAL_Systick_Delay(uint32_t usTime)
{
	TimingDelay = usTime;

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
void HAL_Systick_Set_Timeout(uint32_t timeout)
{
	TimingOut = timeout;
}


uint32_t* HAL_Systick_Get_Timeout(void)
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

	
	//HAL_Systick_Update();

//	Systick_Cnt++;
//	if( (Systick_Cnt&LED_Blink_Interval) == LED_Blink_Interval )
//		LED_Blink_Int();
	
}
