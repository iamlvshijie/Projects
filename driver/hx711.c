/**
  ******************************************************************************
  * @file    hx711.c 
  * @author  I2T Team
  * @version V1.0.1
  * @date    27-May-2016
  * @brief   Main program body
  ******************************************************************************
  * @history 
	*         V1.0.6 add hx711 calibration
	*					V1.0.5 fix bug: HX711_Get_NetWeight error return
	*					V1.0.4 add HX711 Debug definition
	*					V1.0.3 change AllWeight GrossWeight NetWeight to Arrays to store multi HX711 device's info
	*					V1.0.2 add HX711_x to support multi HX711 Devices
	* 				V1.0.1 add working mode;
	* 				V1.0.0 basic function;
	*/
#include "hx711.h"
#include <stm32f0xx.h>
#include "hal_io.h"
extern uint8_t MyDeviceID;
static uint32_t AllWeight[HX711_N];
static uint32_t GrossWeight[HX711_N];
static int32_t 	NetWeight[HX711_N];
static const float GapValue[6] = {740, 746.0, 761.2, 722.1, 759.9, 700};	//depends on sensor, needed to adjust according to your sensor

/**
  * @brief  HX711 Delay program
  * @param  us: us for delay
  * @retval None
	* @note 	only used for hx711 driver
  */
static void HX711_Delay(uint8_t us)
{
	uint8_t index, piece = 48;
	for(index =0;index<piece*us;index++);
}

//HX711_Devic
const uint16_t HX711_SCK_IO[]={	HX711_0_SCK_IO//,
																//HX711_1_SCK_IO,
																//HX711_2_SCK_IO
																

};
const uint16_t HX711_DOUT_IO[]={HX711_0_DOUT_IO//,
																//HX711_1_DOUT_IO,
																//HX711_2_DOUT_IO
																
};	
/**
  * @brief  HX711 Init program
  * @param  None
  * @retval None
	* @note 	Only used for hx711 driver
  */
void HX711_Init(HX711_TypeDef HX711_x)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);	

	//HX711 SCK Pin
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN[HX711_SCK_IO[HX711_x]];//GPIO_Pin_6;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_Init(GPIO_PORT[HX711_SCK_IO[HX711_x]], &GPIO_InitStructure);					 
	
	//HX711 DOUT Pin
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN[HX711_DOUT_IO[HX711_x]];//GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(GPIO_PORT[HX711_DOUT_IO[HX711_x]], &GPIO_InitStructure);  
	
	GPIO_SetBits(GPIO_PORT[HX711_SCK_IO[HX711_x]],GPIO_PIN[HX711_SCK_IO[HX711_x]]);			

	HX711_Get_GrossWeight(HX711_x);
}


void HX711_Calibration(HX711_TypeDef HX711_x, uint32_t inWeight)
{
	float GapValueMin, GapValueMax, tempGapValue = GapValue[MyDeviceID];
	AllWeight[HX711_x] = HX711_Read_Data(HX711_x, MODE_A128);
	
	NetWeight[HX711_x] = AllWeight[HX711_x] - GrossWeight[HX711_x];			
	NetWeight[HX711_x] = (int32_t)((float)NetWeight[HX711_x]/tempGapValue); 	
	printf("HX711>0, %d, %f\r\n", NetWeight[HX711_x], tempGapValue);

	if(NetWeight[HX711_x]<inWeight){
		do
		{
			tempGapValue-=1;
			NetWeight[HX711_x] = AllWeight[HX711_x] - GrossWeight[HX711_x];			
			NetWeight[HX711_x] = (int32_t)((float)NetWeight[HX711_x]/tempGapValue); 	
			printf("HX711>1, %d, %f\r\n", NetWeight[HX711_x], tempGapValue);
		}
		while(NetWeight[HX711_x]<inWeight);
	}
	else if(NetWeight[HX711_x]>inWeight)
	{
		do{
			tempGapValue+=1;
			NetWeight[HX711_x] = AllWeight[HX711_x] - GrossWeight[HX711_x];			
			NetWeight[HX711_x] = (int32_t)((float)NetWeight[HX711_x]/tempGapValue); 	
			printf("HX711>2, %d, %f\r\n", NetWeight[HX711_x], tempGapValue);
		}
		while(NetWeight[HX711_x]>inWeight);
	
	}
	
	GapValueMin = tempGapValue+0.9;
	GapValueMax = tempGapValue-0.1;
	{
		do
		{
			GapValueMin-=0.1;
			NetWeight[HX711_x] = AllWeight[HX711_x] - GrossWeight[HX711_x];			
			NetWeight[HX711_x] = (int32_t)((float)NetWeight[HX711_x]/GapValueMin); 	
			printf("HX711>3, %d, %f\r\n", NetWeight[HX711_x], GapValueMin);
		}
		while(NetWeight[HX711_x]==inWeight);
		GapValueMin+=0.1;
	
		do
		{
			GapValueMax+=0.1;
			NetWeight[HX711_x] = AllWeight[HX711_x] - GrossWeight[HX711_x];			
			NetWeight[HX711_x] = (int32_t)((float)NetWeight[HX711_x]/GapValueMax); 	
			printf("HX711>4, %d, %f\r\n", NetWeight[HX711_x], GapValueMax);
		}
		while(NetWeight[HX711_x]==inWeight);	
		GapValueMax-=0.1;
	
	}
	
	printf("HX711>Calibration factor (Min : %f, Max: %f)\r\n", GapValueMin, GapValueMax);
}
/**
  * @brief  HX711 Read Data program
  * @param  mode: work mode for next ADC cycle
  * @retval HX711 ADC result
	* @note 	None
  */
static uint32_t HX711_Read_Data(HX711_TypeDef HX711_x, HX711_ModeDef mode)	
{
	uint32_t HX711_Data; 
	uint8_t clockIndex;
  uint8_t t3 = 1, t4=1;	
	
	HAL_IO_High(HX711_DOUT_IO[HX711_x]);
	HX711_Delay(1);
  HAL_IO_Low(HX711_SCK_IO[HX711_x]);
	
  HX711_Data=0; 
  while(HAL_IO_Read(HX711_DOUT_IO[HX711_x])); 
  for(clockIndex=0;clockIndex<24;clockIndex++)
	{ 
		HAL_IO_High(HX711_SCK_IO[HX711_x]);
	  HX711_Data=HX711_Data<<1; 
		HX711_Delay(t3);
		HAL_IO_Low(HX711_SCK_IO[HX711_x]);
		
	  if(HAL_IO_Read(HX711_DOUT_IO[HX711_x]))
			HX711_Data++; 
		HX711_Delay(t4);
	} 
	
	//SCK 25 26 27 clock for MODE_A128 MODE_B32 MODE_A64 
	for(clockIndex=0;clockIndex<mode+1;clockIndex++)
	{
		
		HAL_IO_High(HX711_SCK_IO[HX711_x]);
		HX711_Delay(t3);
		HAL_IO_Low(HX711_SCK_IO[HX711_x]);  
	}
#ifdef HX711_DEBUG	
	printf("HX711>Original data is 0x%x",HX711_Data);

	HX711_Data=HX711_Data^HX711_DATA_MASK;
	printf(", Masked data is 0x%x\r\n", HX711_Data);
#endif	
	return HX711_Data;
}

static void  HX711_Get_GrossWeight(HX711_TypeDef HX711_x)
{
	uint8_t i;
	for(i=0;i<5;i++){
		GrossWeight[HX711_x] = HX711_Read_Data(HX711_x, MODE_A128);	
#ifdef HX711_DEBUG	
		printf("HX711>GrossWeight is %d\r\n", GrossWeight[HX711_x]);
#endif	
	}
} 

uint32_t  HX711_Get_NetWeight(HX711_TypeDef HX711_x, uint32_t* pWeight)
{
	AllWeight[HX711_x] = HX711_Read_Data(HX711_x, MODE_A128);
#ifdef HX711_DEBUG		
	printf("HX711>All Weight is %d, GrossWeight is %d, difference is %d\r\n", AllWeight[HX711_x], GrossWeight[HX711_x], AllWeight[HX711_x]-GrossWeight[HX711_x]);
#endif	
	if(AllWeight[HX711_x] > GrossWeight[HX711_x])			
	{
		NetWeight[HX711_x] = AllWeight[HX711_x] - GrossWeight[HX711_x];			
		NetWeight[HX711_x] = (int32_t)((float)NetWeight[HX711_x]/GapValue[MyDeviceID]+0.05); 															
	}
	//over weight or ...
	if((NetWeight[HX711_x] > HX711_MAX_WEIGHT) || (AllWeight[HX711_x] < (GrossWeight[HX711_x] - HX711_MAX_DATA_DIFFERENCE)) )
	{
#ifdef HX711_DEBUG		
		printf("HX711>Error:over weight or overlflow\r\n");
#endif	
		return ERROR;
	}
	else
	{
		*pWeight = NetWeight[HX711_x];
		return SUCCESS;
	}
}
