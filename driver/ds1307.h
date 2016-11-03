/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DS1307_H
#define __DS1307_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"


/** @addtogroup Utilities
  * @{
  */



/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

	#define DS1307_I2C_Address 	0xD0	 
	 
	#define DS1307_SEC 0
	#define DS1307_MIN 1
	#define DS1307_HR 2
	#define DS1307_DOW 3
	#define DS1307_DATE 4
	#define DS1307_MTH 5
	#define DS1307_YR 6

	#define DS1307_BASE_YR 2000

	#define DS1307_SQW1HZ  7
	#define DS1307_SQW4KHZ 8
	#define DS1307_SQW8KHZ 9
	#define DS1307_SQW32KHZ 10

	#define DS1307_CTRL_ID B1101000  //DS1307

	 // Define register bit masks
	#define DS1307_CLOCKHALT B10000000

	#define DS1307_LO_BCD  B00001111
	#define DS1307_HI_BCD  B11110000

	#define DS1307_HI_SEC  B01110000
	#define DS1307_HI_MIN  B01110000
	#define DS1307_HI_HR   B00110000
	#define DS1307_LO_DOW  B00000111
	#define DS1307_HI_DATE B00110000
	#define DS1307_HI_MTH  B00110000
	#define DS1307_HI_YR   B11110000


	// Prescaler
	#define DS1307_OUTLOW_BIT		B00000000
	#define DS1307_OUTHIGH_BIT		B10000000
	#define DS1307_SQW1HZ_BIT	B00010000
	#define DS1307_SQW4KHZ_BIT	B00010001
	#define DS1307_SQW8KHZ_BIT	B00010010
	#define DS1307_SQW32KHZ_BIT	B00010011		



/** @defgroup 
  * @{
  */ 
uint32_t DS1307_Init(I2C_TypeDef* I2Cx);
uint32_t DS1307_ReadTime(uint8_t* pBuffer);
uint32_t DS1307_SetTime(uint8_t* pBuffer);
uint32_t DS1307_TIMEOUT_UserCallback(void);
	 
/**
  * @}
  */
  
#ifdef __cplusplus
}
#endif

#endif /* __DS1307_H */
/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */

/**
  * @}
  */  

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
