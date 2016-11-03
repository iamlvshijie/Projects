/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HAL_ADC_H
#define __HAL_ADC_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"

uint32_t HAL_ADC_Init(void);
uint32_t HAL_ADC_Get_ADC(uint16_t* value);

/**
  * @}
  */
  
#ifdef __cplusplus
}
#endif

#endif /* __HAL_ADC_H */
