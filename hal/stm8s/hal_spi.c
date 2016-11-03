#include "hal_spi.h"
#include "stm8s_spi.h"

static void HAL_SPI_DelayUS(u16 t)
{
  u16 d;
  while(--t)
    for(d=0;d<1024;d++);
} 

void HAL_SPI_Init(void)
{
   SPI_Init(SPI_FIRSTBIT_MSB, SPI_BAUDRATEPRESCALER_8, SPI_MODE_MASTER,\
            SPI_CLOCKPOLARITY_LOW, SPI_CLOCKPHASE_1EDGE, \
            SPI_DATADIRECTION_2LINES_FULLDUPLEX, SPI_NSS_SOFT, 0x07);
   SPI_Cmd(ENABLE);
   GPIO_Init(SPI_CS_PORT , SPI_CS_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
   HAL_SPI_CS_HIGH();
}
/*******************************************************************************
* Function Name  : HAL_SPI_Write_Byte
* Description    : Sends a byte through the SPI interface and return the byte
*                  received from the SPI bus.
* Input          : byte : byte to send.
* Output         : None
* Return         : The value of the received byte.
*******************************************************************************/
u8 HAL_SPI_Send_Byte(u8 byte)
{
  /* Loop while DR register in not emplty */
  while (SPI_GetFlagStatus( SPI_FLAG_TXE) == RESET);

  /* Send byte through the SPI1 peripheral */
  SPI_SendData(byte);

  /* Wait to receive a byte */
  while (SPI_GetFlagStatus(SPI_FLAG_RXNE) == RESET);

  /* Return the byte read from the SPI bus */
  return SPI_ReceiveData();	 
 }
 
/*******************************************************************************
* Function Name  : HAL_SPI_Read_Byte
* Description    : Reads a byte from the SPI Flash.
*                  This function must be used only if the Start_Read_Sequence
*                  function has been previously called.
* Input          : None
* Output         : None
* Return         : Byte Read from the SPI Flash.
*******************************************************************************/
u8 HAL_SPI_Receive_Byte(void)
{
  return (HAL_SPI_Send_Byte(0x00));
}


u8 HAL_SPI_Write_Reg(u8 regAddr, u8 data)
{
  u8 status;
  HAL_SPI_CS_LOW();
  status = HAL_SPI_Send_Byte(regAddr);
  HAL_SPI_Send_Byte(data);
  HAL_SPI_CS_HIGH();
  return status;
}

u8 HAL_SPI_Read_Reg(u8 regAddr)
{
  uint8_t status;
  HAL_SPI_CS_LOW();
  HAL_SPI_Send_Byte(regAddr);
  status=HAL_SPI_Receive_Byte();   //select register  and write value to it
  HAL_SPI_CS_HIGH();
  return status;
}
/*******************************************************************************
* Function Name  : HAL_SPI_Write_Buffer
* Description    : Writes block of data to the FLASH. In this function, the
*                  number of WRITE cycles are reduced, using Page WRITE sequence.
* Input          : - pBuffer : pointer to the buffer  containing the data to be
*                    written to the FLASH.
*                  - WriteAddr : FLASH's internal address to write to.
*                  - NumByteToWrite : number of bytes to write to the FLASH.
* Output         : None
* Return         : None
*******************************************************************************/
void HAL_SPI_Write_Buffer(u8 WriteAddr, u8* pBuffer, u16 NumByteToWrite)
{
  u8 status;
  u16 byte_ctr;
  HAL_SPI_CS_LOW();
  status = HAL_SPI_Send_Byte(WriteAddr);

  for(byte_ctr=0;byte_ctr<NumByteToWrite;byte_ctr++)
    HAL_SPI_Send_Byte(*pBuffer++);
  HAL_SPI_CS_HIGH();
}

/*******************************************************************************
* Function Name  : HAL_SPI_Read_Buffer
* Description    : Reads a block of data from the FLASH.
* Input          : - pBuffer : pointer to the buffer that receives the data read
*                    from the FLASH.
*                  - ReadAddr : FLASH's internal address to read from.
*                  - NumByteToRead : number of bytes to read from the FLASH.
* Output         : None
* Return         : None
*******************************************************************************/
void HAL_SPI_Read_Buffer(u8 ReadAddr, u8* pBuffer, u16 NumByteToRead)
{
  u8 status;
  u16 byte_ctr;
  /* Select the FLASH: Chip Select low */
  HAL_SPI_CS_LOW();

  /* Send "Read from Memory " instruction */
  status = HAL_SPI_Send_Byte(ReadAddr);
  for(byte_ctr=0;byte_ctr<NumByteToRead;byte_ctr++)
    pBuffer[byte_ctr] = HAL_SPI_Receive_Byte();
        
//  while (NumByteToRead--) /* while there is data to be read */
//  {
//    /* Read a byte from the FLASH */
//    *pBuffer = HAL_SPI_Send_Byte(Dummy_Byte);
//    /* Point to the next location where the byte read will be saved */
//    pBuffer++;
//  }

  /* Deselect the FLASH: Chip Select high */
  HAL_SPI_CS_HIGH();
}


