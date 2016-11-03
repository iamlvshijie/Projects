/**
  ******************************************************************************
  * @file    hx711.c 
  * @author  I2T Team
  * @version V1.0.1
  * @date    27-May-2016
  * @brief   Main program body
  ******************************************************************************
  * @history V1.0.4 add HX711 Debug
	*					 V1.0.3 add HX711_N for the number of HX711 devices, change GapValue from definitions to Const Array 
	*					 V1.0.2 add HX711_TypeDef_0 to support multi HX711 Devices
	*					 V1.0.1 add HX711_MAX_WEIGHT, adjust GapValue;
	* 				 V1.0.0 basic function;
	*/
#ifndef __HX711_H
#define __HX711_H

#include "hal_io.h"
//#define HX711_DEBUG

#define HX711_N (3)

typedef enum 
{
  HX711_0 = 0,
  HX711_1 = 1,
	HX711_2 = 2
} HX711_TypeDef;

#define HX711_DATA_MASK 					(0x40000000)  //adjusted by the max weight
#define HX711_MAX_WEIGHT 					(2010) 			//unit:g
#define HX711_MAX_DATA_DIFFERENCE (3000) 			//overflow for weight

typedef enum 
{
  MODE_A128 = 0,
  MODE_B32 = 1,
	MODE_A64 = 2
} HX711_ModeDef;

static void 			HX711_Delay(uint8_t us);
void 							HX711_Init(HX711_TypeDef HX711_x);
void HX711_Calibration(HX711_TypeDef HX711_x, uint32_t inWeight);
static uint32_t 	HX711_Read_Data(HX711_TypeDef HX711_x, HX711_ModeDef mode);
static void 			HX711_Get_GrossWeight(HX711_TypeDef HX711_x);
uint32_t  				HX711_Get_NetWeight(HX711_TypeDef HX711_x,uint32_t* pWeight);

#endif
