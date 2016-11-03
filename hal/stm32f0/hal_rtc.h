#ifndef __HAL_RTC_H_
#define __HAL_RTC_H_

#include "stm32f0xx.h"
#include "time.h"

#define BKP0_VALUE    0x03FF 


uint32_t HAL_RTC_Init(Time_TypeDef* time, uint32_t* timeout);
uint32_t HAL_RTC_Get_Time(Time_TypeDef* time);
uint32_t HAL_RTC_Set_Time(Time_TypeDef* time);
uint32_t HAL_RTC_Set_Alarm(void);


#endif /* __HAL_RTC_H_ */
