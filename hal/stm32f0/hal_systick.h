
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HAL_SYSTICK_H
#define __HAL_SYSTICK_H

#include "bsp.h"

#ifdef __cplusplus
 extern "C" {
#endif
uint32_t 	HAL_Systick_Init(void);
uint32_t 	HAL_Systick_Delay(uint32_t msTime);
void 			HAL_Systick_Update(void);
void 			HAL_Systick_Set_Timeout(uint32_t timeout);
uint32_t* HAL_Systick_Get_Timeout(void);	 
/**
  * @}
  */
  
#ifdef __cplusplus
}
#endif

#endif /* __HAL_SYSTICK_H */	 	 
