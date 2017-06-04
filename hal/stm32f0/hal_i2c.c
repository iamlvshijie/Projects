

/* Includes ------------------------------------------------------------------*/
#include "hal_i2c.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
u32_t I2C_Timeout;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  
  *         
  *         
  * @param  
  * @retval 
  */
void hal_i2c_init(i2c_cfg_t cfg)
{
	
}




#ifdef I2C_IO_SIMULATION
/*
void IIC_Start()
{
	SCLK_Set() ;
	SDIN_Set();
	SDIN_Clr();
	SCLK_Clr();
}

//IIC Stop

void IIC_Stop()
{
OLED_SCLK_Set() ;
//	OLED_SCLK_Clr();
	OLED_SDIN_Clr();
	OLED_SDIN_Set();
	
}

void IIC_Wait_Ack()
{

	//GPIOB->CRH &= 0XFFF0FFFF;	//设置PB12为上拉输入模式
	//GPIOB->CRH |= 0x00080000;
//	OLED_SDA = 1;
//	delay_us(1);
	//OLED_SCL = 1;
	//delay_us(50000);
	while(1)
	{
		if(!OLED_SDA)				//判断是否接收到OLED 应答信号
		{
			//GPIOB->CRH &= 0XFFF0FFFF;	//设置PB12为通用推免输出模式
			//GPIOB->CRH |= 0x00030000;
			return;
		}
	}

	OLED_SCLK_Set() ;
	OLED_SCLK_Clr();
}

// IIC Write byte


void Write_IIC_Byte(unsigned char IIC_Byte)
{
	unsigned char i;
	unsigned char m,da;
	da=IIC_Byte;
	OLED_SCLK_Clr();
	for(i=0;i<8;i++)		
	{
			m=da;
		//	OLED_SCLK_Clr();
		m=m&0x80;
		if(m==0x80)
		{OLED_SDIN_Set();}
		else OLED_SDIN_Clr();
			da=da<<1;
		OLED_SCLK_Set();
		OLED_SCLK_Clr();
		}


}

#endif
*/

/**
  * @brief  从I2C1总线上的某一器件的某一起始地址中读取一定字节的数据到数组中
  * @param  driver_Addr：I2C器件地址
  * @param  start_Addr：起始字节地址
  * @param  number_Bytes：要读取的字节数量（小于一页）
  * @param  read_Buffer：存放读取数据的数组指针
  * @retval 是否读取成功
  */
hal_status hal_read_bytes(i2c_num i2cx, u8_t driver_addr, u8_t start_addr, u8_t bytes_len, u8_t *bytes)
{
  u8_t read_Num;
  
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
hal_status hal_write_bytes(i2c_num i2cx, u8_t driver_addr, u8_t start_addr, u8_t bytes_len, u8_t *bytes)
{
  u8_t write_Num;
  
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
