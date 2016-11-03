#include "hal_rtc.h"
#include "hal_systick.h"

RTC_TimeTypeDef RTC_TimeStructure;
RTC_DateTypeDef RTC_DateStructure;

uint32_t HAL_RTC_Init(Time_TypeDef* time, uint32_t* timeout)
{
	RTC_InitTypeDef  RTC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
  RTC_AlarmTypeDef RTC_AlarmStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

	if (RTC_ReadBackupRegister(RTC_BKP_DR0) != BKP0_VALUE){
		printf("  First Config\r\n");
		 /* Enable the PWR clock */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

		/* Allow access to RTC */
		PWR_BackupAccessCmd(ENABLE);
		
		/* Enable the LSE OSC */
		//RCC_LSEDriveConfig(RCC_LSEDrive_Low);
		RCC_LSEConfig(RCC_LSE_ON);
		//while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);
		
		HAL_Systick_Delay(5000);
		
//		while((*timeout)>0)
//		{
			/* Wait till LSE is ready */  
			if(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == SET)
			{
				/* Select the RTC Clock Source */
				RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

				/* Configure the RTC data register and RTC prescaler */
				/* ck_spre(1Hz) = RTCCLK(LSI) /(AsynchPrediv + 1)*(SynchPrediv + 1)*/
				RTC_InitStructure.RTC_AsynchPrediv = 0x7F;
				RTC_InitStructure.RTC_SynchPrediv  = 0xFF;
				RTC_InitStructure.RTC_HourFormat   = RTC_HourFormat_24;
				RTC_Init(&RTC_InitStructure);
				printf("  LSE used ...\r\n");
				
				/* Enable the RTC Clock */
				RCC_RTCCLKCmd(ENABLE);
				
				/* Wait for RTC APB registers synchronisation */
				RTC_WaitForSynchro();	
				//break;
			}
		//}
		//if((*timeout)==0)
		else
		{	
			printf("LSE startup failed! \r\n");
			/* Enable the LSE OSC */
			RCC_LSEConfig(RCC_LSE_OFF);

			HAL_Systick_Delay(50);
			
			/* LSI used as RTC source clock */
			/* The RTC Clock may varies due to LSI frequency dispersion. */   
			/* Enable the LSI OSC */ 
			RCC_LSICmd(ENABLE);

			/* Wait till LSI is ready */  
			while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
			{
			}

			/* Select the RTC Clock Source */
			RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);	
			/* Enable the RTC Clock */
			RCC_RTCCLKCmd(ENABLE);
			
			/* Wait for RTC APB registers synchronisation */
			RTC_WaitForSynchro();	
			
			/* Calendar Configuration */
			RTC_InitStructure.RTC_AsynchPrediv = 99;
			RTC_InitStructure.RTC_SynchPrediv	=  399; /* (40KHz / 100) - 1 = 399*/
			RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
			RTC_Init(&RTC_InitStructure);  
			
			printf("  LSI used ...\r\n");
		}
		
		//HAL_RTC_Set_Time(time);
	
			
		/* Set Time hh:mm:ss */
		RTC_TimeStructure.RTC_Hours   = 0x00;  
		RTC_TimeStructure.RTC_Minutes = 0x00;
		RTC_TimeStructure.RTC_Seconds = 0x00;
		RTC_SetTime(RTC_Format_BCD, &RTC_TimeStructure);

		/* Set Date Week/Date/Month/Year */
		RTC_DateStructure.RTC_WeekDay = 0x01;
		RTC_DateStructure.RTC_Date = 0x31;
		RTC_DateStructure.RTC_Month = 0x08;
		RTC_DateStructure.RTC_Year = 0x15;
		RTC_SetDate(RTC_Format_BCD, &RTC_DateStructure);
		
		/* Write BkUp DR0 */
		RTC_WriteBackupRegister(RTC_BKP_DR0, BKP0_VALUE);
			

		/* Disable the Alarm A */
		RTC_AlarmCmd(RTC_Alarm_A, DISABLE);
		
		/* Set the alarmA Masks */

		RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours = 0x00;
		RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes = 0x00;
		RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds = 0x30;
		
		RTC_AlarmStructure.RTC_AlarmMask = RTC_AlarmMask_All - RTC_AlarmMask_Seconds;
		RTC_AlarmStructure.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_Date;
		RTC_AlarmStructure.RTC_AlarmDateWeekDay = 1;
		RTC_SetAlarm(RTC_Format_BCD, RTC_Alarm_A, &RTC_AlarmStructure);
		
		/* Enable AlarmA interrupt */
		RTC_ITConfig(RTC_IT_ALRA, ENABLE);
		
		/* Enable the alarmA */
		RTC_AlarmCmd(RTC_Alarm_A, ENABLE);
		
	}
	else{
		 /* Check if the Power On Reset flag is set */
    if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
    {
      printf("  Power_On Reset, ");
    }
    /* Check if the Pin Reset flag is set */
    else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
    {
      printf("  External Reset, ");
    }

    printf("No need to configure\r\n");
    
    /* Enable the PWR clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

    /* Allow access to RTC */
    PWR_BackupAccessCmd(ENABLE);
		
		HAL_Systick_Delay(5000);
		
		/* check LSE or LSI */  
		if(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
		{
			RCC_LSICmd(ENABLE);
		}
		RTC_WaitForSynchro();

    /* Clear the RTC Alarm Flag */
    RTC_ClearFlag(RTC_FLAG_ALRAF);

    /* Clear the EXTI Line 17 Pending bit (Connected internally to RTC Alarm) */
    EXTI_ClearITPendingBit(EXTI_Line17);
	}	
	
	if(RTC_GetFlagStatus(RTC_FLAG_ALRAF)==SET)
	{	
		RTC_GetAlarm(RTC_Format_BIN, RTC_Alarm_A, &RTC_AlarmStructure);
		printf("  The current alarm is :  %0.2d:%0.2d:%0.2d \r\n", RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours, RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes, RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds);

		/* EXTI configuration */
		EXTI_ClearITPendingBit(EXTI_Line17);
		EXTI_InitStructure.EXTI_Line = EXTI_Line17;
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		EXTI_Init(&EXTI_InitStructure);
		
		/* Enable the RTC Alarm Interrupt */
		NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
	}
}


uint32_t HAL_RTC_Get_Time(Time_TypeDef* time)
{
	RTC_GetDate(RTC_Format_BCD, &RTC_DateStructure);
	printf("  The current time is :  %0.2d:%0.2d:%0.2d - ", RTC_DateStructure.RTC_Year, RTC_DateStructure.RTC_Month, RTC_DateStructure.RTC_Date);	
	/* Set Date Week/Date/Month/Year */
	
	time->weekday = RTC_DateStructure.RTC_WeekDay;
	time->date 		= RTC_DateStructure.RTC_Date; 
	time->month 	= RTC_DateStructure.RTC_Month; 
	time->year 		= RTC_DateStructure.RTC_Year;
	
	RTC_GetTime(RTC_Format_BCD, &RTC_TimeStructure);
	printf(" %0.2d:%0.2d:%0.2d \r\n", RTC_TimeStructure.RTC_Hours, RTC_TimeStructure.RTC_Minutes, RTC_TimeStructure.RTC_Seconds);
	
	time->hour	 = RTC_TimeStructure.RTC_Hours;
	time->minute = RTC_TimeStructure.RTC_Minutes;
	time->second = RTC_TimeStructure.RTC_Seconds;
}

uint32_t HAL_RTC_Set_Time(Time_TypeDef* time)
{

	/* Set Time hh:mm:ss */
	
	RTC_TimeStructure.RTC_Hours   = time->hour;  
	RTC_TimeStructure.RTC_Minutes = time->minute;
	RTC_TimeStructure.RTC_Seconds = time->second;
	RTC_SetTime(RTC_Format_BCD, &RTC_TimeStructure);

	/* Set Date Week/Date/Month/Year */
	RTC_DateStructure.RTC_WeekDay = time->weekday;
	RTC_DateStructure.RTC_Date = time->date;
	RTC_DateStructure.RTC_Month = time->month;
	RTC_DateStructure.RTC_Year = time->year;
	
	RTC_SetDate(RTC_Format_BCD, &RTC_DateStructure);

}

uint32_t HAL_RTC_Set_Alarm(void)
{
	RTC_AlarmTypeDef  RTC_AlarmStructure;
	/* Disable the Alarm A */
	RTC_AlarmCmd(RTC_Alarm_A, DISABLE);
	
	/* Set the alarmA Masks */

	RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours = 0x00;
	RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes = 0x00;
	RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds = 0x30;
	
	RTC_AlarmStructure.RTC_AlarmMask = RTC_AlarmMask_All - RTC_AlarmMask_Seconds;
	RTC_AlarmStructure.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_Date;
	RTC_AlarmStructure.RTC_AlarmDateWeekDay = 1;
	RTC_SetAlarm(RTC_Format_BCD, RTC_Alarm_A, &RTC_AlarmStructure);
	
	/* Enable AlarmA interrupt */
	RTC_ITConfig(RTC_IT_ALRA, ENABLE);
	
	/* Enable the alarmA */
	RTC_AlarmCmd(RTC_Alarm_A, ENABLE);	
	
}