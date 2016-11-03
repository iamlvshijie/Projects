

/* Includes ------------------------------------------------------------------*/
#include "hal_i2c.h"
#include "bsp.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t I2C_Timeout;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  
  *         
  *         
  * @param  
  * @retval 
  */
void HAL_I2C_Init(void)
{
	BSP_I2C_Config();
}

/**
  * @brief  从I2C1总线上的某一器件的某一起始地址中读取一定字节的数据到数组中
  * @param  driver_Addr：I2C器件地址
  * @param  start_Addr：起始字节地址
  * @param  number_Bytes：要读取的字节数量（小于一页）
  * @param  read_Buffer：存放读取数据的数组指针
  * @retval 是否读取成功
  */
I2C_Status HAL_I2C_ReadBytes(I2C_TypeDef* I2Cx,uint8_t driver_Addr, uint8_t start_Addr, uint8_t number_Bytes, uint8_t *read_Buffer)
{
  uint8_t read_Num;
  
  I2C_Timeout = I2C_TIMEOUT;
  while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY) != RESET)
  {
    if((I2C_Timeout--) == 0)
    {
      return I2C_FAIL;
    }
  }
  
  I2C_TransferHandling(I2Cx, driver_Addr, 1, I2C_SoftEnd_Mode, I2C_Generate_Start_Write);
  
  I2C_Timeout = I2C_TIMEOUT;
  while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_TXIS) == RESET)
  {
    if((I2C_Timeout--) == 0)
    {
      return I2C_FAIL;
    }
  }
  
  I2C_SendData(I2Cx, start_Addr);
  
  I2C_Timeout = I2C_TIMEOUT;
  while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_TC) == RESET)
  {
    if((I2C_Timeout--) == 0)
    {
      return I2C_FAIL;
    }
  }
  
  I2C_TransferHandling(I2Cx, driver_Addr, number_Bytes,  I2C_AutoEnd_Mode, I2C_Generate_Start_Read);
  
  for(read_Num = 0; read_Num < number_Bytes; read_Num++)
  {
    I2C_Timeout = I2C_TIMEOUT;
    while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_RXNE) == RESET)
    {
      if((I2C_Timeout--) == 0)
      {
        return I2C_FAIL;
      }
    }
    
    read_Buffer[read_Num] = I2C_ReceiveData(I2Cx);
  }
  
  I2C_Timeout = I2C_TIMEOUT;
  while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_STOPF) == RESET)
  {
    if((I2C_Timeout--) == 0)
    {
      return I2C_FAIL;
    }
  }

  return I2C_OK;
}

/**
  * @brief  从I2C1的总线上的某一器件的某一起始地址中读取一定字节的数据到数组中
  * @param  driver_Addr：I2C器件地址
  * @param  start_Addr：起始字节地址
  * @param  number_Bytes：要读取的字节数量（小于一页）
  * @param  write_Buffer：存放读取数据的数组指针
  * @retval 是否读取成功
  */
I2C_Status HAL_I2C_WriteBytes(I2C_TypeDef* I2Cx, uint8_t driver_Addr, uint8_t start_Addr, uint8_t number_Bytes, uint8_t *write_Buffer)
{
  uint8_t write_Num;
  
  I2C_Timeout = I2C_TIMEOUT;
  while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY) != RESET)
  {
    if((I2C_Timeout--) == 0)
    {
      return I2C_FAIL;
    }
  }
  
  I2C_TransferHandling(I2Cx, driver_Addr, 1, I2C_Reload_Mode, I2C_Generate_Start_Write);
  
  I2C_Timeout = I2C_TIMEOUT;
  while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_TXIS) == RESET)
  {
    if((I2C_Timeout--) == 0)
    {
      return I2C_FAIL;
    }
  }
  
  I2C_SendData(I2Cx, start_Addr);
  
  I2C_Timeout = I2C_TIMEOUT;
  while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_TCR) == RESET)
  {
    if((I2C_Timeout--) == 0)
    {
      return I2C_FAIL;
    }
  }

  I2C_TransferHandling(I2Cx, driver_Addr, number_Bytes, I2C_AutoEnd_Mode, I2C_No_StartStop);
  
  for(write_Num = 0; write_Num < number_Bytes; write_Num++)
  {
    I2C_Timeout = I2C_TIMEOUT;
    while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_TXIS) == RESET)
    {
      if((I2C_Timeout--) == 0)
      {
        return I2C_FAIL;
      }
    }
    
    I2C_SendData(I2Cx, write_Buffer[write_Num]);
  }

  I2C_Timeout = I2C_TIMEOUT;
  while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_STOPF) == RESET)
  {
    if((I2C_Timeout--) == 0)
    {
      return I2C_FAIL;
    }
  }
  
  
  return I2C_OK;
}

/************************ (C) COPYRIGHT LKL0305 ****************END OF FILE****/
