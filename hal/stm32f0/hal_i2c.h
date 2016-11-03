

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HAL_I2C_H
#define __HAL_I2C_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"

/* Exported types ------------------------------------------------------------*/
typedef enum
{
  I2C_OK                                          = 1,
  I2C_FAIL                                        = 0
}I2C_Status;

/* Exported constants --------------------------------------------------------*/
#define I2C_IO_SIMULATION

#define I2C_TIMEOUT                               0xF000

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void HAL_I2C_Init(void);
I2C_Status HAL_I2C_ReadBytes(I2C_TypeDef* I2Cx, uint8_t driver_Addr, uint8_t start_Addr, uint8_t number_Bytes, uint8_t *read_Buffer); 
I2C_Status HAL_I2C_WriteBytes(I2C_TypeDef* I2Cx, uint8_t driver_Addr, uint8_t start_Addr, uint8_t number_Bytes, uint8_t *write_Buffer); 

#endif

/************************ (C) COPYRIGHT LKL0305 ****************END OF FILE****/
