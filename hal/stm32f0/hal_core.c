/*  
	@file:			hal_core.c
	@author:		Jacob Lui
	@description: hal core for stm32f0
	@revision:	1.0
	@history:		2017.3.26
	@copyright: Jacob
*/

#include "hal_core.h"


#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
	
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}


void int_all_disable(void)
{		  
	__ASM volatile("cpsid i");
}

void int_all_enable(void)
{
	__ASM volatile("cpsie i");		  
}

__asm void msr_msp(u32_t addr) 
{
    MSR MSP, r0 			//set Main Stack value
    BX r14
}
/*********************************** END OF FILE *******************************/
