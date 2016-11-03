#include "hal_wdg.h"

void HAL_WDG_Init(void)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
  IWDG_SetPrescaler(IWDG_Prescaler_32);
  IWDG_SetReload(800);
  IWDG_ReloadCounter();
  IWDG_Enable();
}
/* Set counter reload value to obtain 250ms IWDG TimeOut.
	 Counter Reload Value = 250ms/IWDG counter clock period
												= 250ms / (LSI/32)
												= 200
*/
void HAL_WDG_Reload(void)
{
	IWDG_ReloadCounter();
}

uint8_t HAL_WDG_Check()
{  
	return RCC_GetFlagStatus(RCC_FLAG_IWDGRST);
}