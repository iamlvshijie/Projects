/*  
	@file:			hal_systick.c
	@author:		Jacob Lui
	@description: hal systick for yd_stm32f0
	@revision:	1.0
	@history:		2017.3.26
	@copyright: Jacob
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HAL_SYSTICK_H
#define __HAL_SYSTICK_H

#include "hal_core.h"

#ifdef __cplusplus
 extern "C" {
#endif
	 
u32_t 	hal_systick_init(u32_t freq);
u32_t 	hal_systick_delay(u32_t ms_time);
	 
void 			HAL_Systick_Update(void);
void 			HAL_Systick_Set_Timeout(u32_t timeout);
u32_t* HAL_Systick_Get_Timeout(void);	 

#ifdef __cplusplus
}
#endif

#endif /* __HAL_SYSTICK_H */	 	 
