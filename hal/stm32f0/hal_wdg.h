#ifndef __HAL_WDG_
#define __HAL_WDG_

#include "stm32f0xx.h"

void HAL_WDG_Init(void);
void HAL_WDG_Reload(void);
uint8_t HAL_WDG_Check(void);
#endif /* __HAL_WDG_ */
