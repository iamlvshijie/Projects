

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HAL_I2C_H
#define __HAL_I2C_H

/* Includes ------------------------------------------------------------------*/
#include "hal_core.h"

/* Exported types ------------------------------------------------------------*/
typedef enum
{
  I2C_OK                                          = 1,
  I2C_FAIL                                        = 0
};
typedef enum
{
	i2c0,
	i2c1,
	i2c2
}i2c_num;

typedef struct
{
	u8_t i2c_Addr;
	
	
}i2c_cfg_t;

/* Exported constants --------------------------------------------------------*/
#define I2C_IO_SIMULATION

#define I2C_TIMEOUT                               0xF000

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
hal_status hal_i2c_init(i2c_num i2cx, i2c_cfg_t cfg);
hal_status hal_i2c_read_bytes(i2c_num i2cx, uint8_t i2c_Addr, uint8_t start_Addr, uint8_t number_Bytes, uint8_t *p_bytes); 
hal_status hal_i2c_read_bytes(i2c_num i2cx, uint8_t i2c_Addr, uint8_t start_Addr, uint8_t number_Bytes, uint8_t *p_bytes); 
hal_status hal_i2c_read_byte(i2c_num i2cx, uint8_t i2c_Addr, uint8_t start_Addr, uint8_t data); 
hal_status hal_i2c_read_byte(i2c_num i2cx, uint8_t i2c_Addr, uint8_t start_Addr, uint8_t data); 

#endif

/************************ (C) COPYRIGHT LKL0305 ****************END OF FILE****/
