/**
  ******************************************************************************
  * @file    Project/STM32F0xx_StdPeriph_Templates/stm32f0xx_it.c 
  * @author  MCD Application Team
  * @version V1.5.0
  * @date    05-December-2014
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_it.h"
#include "hal_systick.h"
#include "led.h"

/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* For WIFI RX TX */
//extern uint8_t ATBuffer[];
//extern uint8_t RxBuffer[];
//extern __IO uint8_t CounterUpdateFlag; 
//extern __IO uint8_t WifiTransparencyFlag;
//extern uint16_t RxPacketBeginIndex;
//extern uint16_t RxPacketEndIndex;

///* For LED */
//extern uint32_t LED_Blink_Interval;


//extern __IO uint8_t RTC_ALARM_Flag;

//extern __IO uint32_t TIM3_Clock;
//extern __IO uint8_t Fixed_Water_Flag;

//extern uint16_t Flow_Cnt;



//__IO uint8_t rxChar;
//__IO uint16_t pktLength;
//__IO uint8_t 	RxCounter= 0, ReceiveState = 0;

//__IO uint16_t IC3ReadValue1 = 0, IC3ReadValue2 = 0;
//__IO uint16_t TDS_In_Capture_Cnt = 0;
//__IO uint32_t Capture = 0;
//__IO uint32_t TDS_In_Freg = 0;
//__IO uint8_t  Flow_Cnt_OF = RESET;
//__IO uint16_t Systick_Cnt=0;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M0 Processor Exceptions Handlers                         */
/******************************************************************************/

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


/**
  * @brief  This function handles External lines 4 to 15 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI4_15_IRQHandler(void)
{
// if(EXTI_GetITStatus(EXTI_Line6) != RESET)
//  {
//    /* Clear the EXTI line 8 pending bit */
//		
//		Flow_Cnt++;
//		if(Flow_Cnt==99){
//			Fixed_Water_Flag++;
//			Flow_Cnt =0;
//		}
//		
//    EXTI_ClearITPendingBit(EXTI_Line6);
//  }
	
	
}
/**
  * @brief  This function handles RTC interrupt request.
  * @param  None
  * @retval None
  */
void RTC_IRQHandler(void)
{
//  if(RTC_GetITStatus(RTC_IT_ALRA) != RESET)
//  {
//		RTC_ALARM_Flag = SET;
//    RTC_ClearITPendingBit(RTC_IT_ALRA);
//    EXTI_ClearITPendingBit(EXTI_Line17);
//  } 
}


void TIM1_BRK_UP_TRG_COM_IRQHandler()
{
//	if(TIM_GetITStatus(TIM1, TIM_IT_CC1)==SET)
//	{
//		Flow_Cnt++;
//		Flow_Cnt_OF = SET;
//    /* Clear the EXTI line 7 pending bit */
//	}
//	
//	TIM_ClearFlag(TIM1, TIM_IT_CC1);
}
void TIM3_IRQHandler()
{
//	if(TIM_GetITStatus(TIM3, TIM_IT_CC3)==SET)
//	{
//    /* Clear TIM1 Capture compare interrupt pending bit */
//    TIM_ClearITPendingBit(TIM3, TIM_IT_CC3);
//    if(TDS_In_Capture_Cnt == 0)
//    {
//      /* Get the Input Capture value */
//      IC3ReadValue1 = TIM_GetCapture3(TIM3);
//      TDS_In_Capture_Cnt = 1;
//    }
//    else if(TDS_In_Capture_Cnt == 1)
//    {
//      /* Get the Input Capture value */
//      IC3ReadValue2 = TIM_GetCapture3(TIM3); 
//      
//      /* Capture computation */
//      if (IC3ReadValue2 > IC3ReadValue1)
//      {
//        Capture = (IC3ReadValue2 - IC3ReadValue1); 
//      }
//      else if (IC3ReadValue2 < IC3ReadValue1)
//      {
//        Capture = ((0xFFFF - IC3ReadValue1) + IC3ReadValue2); 
//      }
//      else
//      {
//        Capture = 0;
//      }
//      /* Frequency computation */ 
//      TDS_In_Freg = (uint32_t) TIM3_Clock/ Capture;
//      TDS_In_Capture_Cnt = 0;
//			TIM_ITConfig(TIM3, TIM_IT_CC3, ENABLE);
//    }
//	
//	
//	}
	

}


/******************************************************************************/
/*                 STM32F0xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f0xx.s).                                            */
/******************************************************************************/
void USART1_IRQHandler(void)
{
//  if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
//		;
//	if((USART1->ISR & USART_ISR_RXNE)==USART_ISR_RXNE)
//  {
	
//		 
//    /* Read one byte from the receive data register */
//    uint8_t rxChar = (USART_ReceiveData(USART1)& 0x7F);
//		if(CounterUpdateFlag==1)
//		{
//			CounterUpdateFlag =0;
//		  RxCounter = 0;	
//		}
//		ATBuffer[RxCounter++] = rxChar;
		
//	}		
}
/**
  * @brief  This function handles USARTy global interrupt request.
  * @param  None
  * @retval None
  */
void USART2_IRQHandler(void)
{
//	if(CounterUpdateFlag==1)
//	{
//		CounterUpdateFlag =0;
//		RxCounter = 0;	
//		pktLength =0;
//	}  
//	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
//  {
//		
//		if(WifiTransparencyFlag==RESET){
//			/* Read one byte from the receive data register */
//			rxChar = (USART_ReceiveData(USART2)& 0x7F);
//			ATBuffer[RxCounter++] = rxChar;
//		}
//		else 
//		{

//			if(ReceiveState==0){
//				pktLength = (USART_ReceiveData(USART2)& 0x7F);   
//				ReceiveState++;
//			}
//			else if(ReceiveState==1){
//				pktLength = (pktLength<<8) + (USART_ReceiveData(USART2)& 0x7F);
//				pktLength +=2;
//				ReceiveState++;
//			}
//			else if(RxCounter==pktLength-1)
//			{
//				ReceiveState = 0xFF;
//			}
//			
//			RxBuffer[RxCounter++] = (USART_ReceiveData(USART2)& 0x7F);
//			
//		}
//		
//	}		
  
}
/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
