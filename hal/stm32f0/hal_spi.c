
#include "hal_spi.h"

void hal_spi_init(spi_t spi_x, spi_cfg_t spi_cfg)
{
	SPI_InitTypeDef SPI_InitStruct;

	 /* Enable the SPI periph */
//  RCC_APB2PeriphClockCmd(SPIx_CLK, ENABLE);
//  
//  /* Enable SCK, MOSI, MISO and NSS GPIO clocks */
//  RCC_AHBPeriphClockCmd(SPIx_SCK_GPIO_CLK | SPIx_MISO_GPIO_CLK | SPIx_MOSI_GPIO_CLK, ENABLE);
//  
//  GPIO_PinAFConfig(SPIx_SCK_GPIO_PORT, SPIx_SCK_SOURCE, SPIx_SCK_AF);
//  GPIO_PinAFConfig(SPIx_MOSI_GPIO_PORT, SPIx_MOSI_SOURCE, SPIx_MOSI_AF);
//  GPIO_PinAFConfig(SPIx_MISO_GPIO_PORT, SPIx_MISO_SOURCE, SPIx_MISO_AF);
//  
//  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
//  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
//  GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_DOWN;
//  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_Level_3;

//  /* SPI SCK pin configuration */
//  GPIO_InitStruct.GPIO_Pin = SPIx_SCK_PIN;
//  GPIO_Init(SPIx_SCK_GPIO_PORT, &GPIO_InitStruct);

//  /* SPI  MOSI pin configuration */
//  GPIO_InitStruct.GPIO_Pin =  SPIx_MOSI_PIN;
//  GPIO_Init(SPIx_MOSI_GPIO_PORT, &GPIO_InitStruct);

//  /* SPI MISO pin configuration */
//  GPIO_InitStruct.GPIO_Pin = SPIx_MISO_PIN;
//  GPIO_Init(SPIx_MISO_GPIO_PORT, &GPIO_InitStruct);
//  
//  /* SPI configuration -------------------------------------------------------*/
//  SPI_I2S_DeInit(SPIx);

		SPI_InitStruct = spi_cfg;
//	SPI_InitStruct.SPI_Direction = SPI_Direction;// SPI_Direction_2Lines_FullDuplex;
//  SPI_InitStruct.SPI_DataSize = SPI_DATASIZE;
//  SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
//  SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
//  SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
//  SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
//  SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
//  SPI_InitStruct.SPI_CRCPolynomial = 7;
//   
//  /* Configure the SPI interrupt priority */
////  NVIC_InitStruct.NVIC_IRQChannel = SPIx_IRQn;
////  NVIC_InitStruct.NVIC_IRQChannelPriority = 1;
////  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
////  NVIC_Init(&NVIC_InitStruct);
////	
//	/* Initializes the SPI communication */
//  SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
//	SPI_Init(SPIx, &SPI_InitStruct);
//  
//  /* Initialize the FIFO threshold */
//  SPI_RxFIFOThresholdConfig(SPIx, SPI_RxFIFOThreshold_QF);
//  
////  /* Enable the Rx buffer not empty interrupt */
////  SPI_I2S_ITConfig(SPIx, SPI_I2S_IT_RXNE, ENABLE);
////  /* Enable the SPI Error interrupt */
////  SPI_I2S_ITConfig(SPIx, SPI_I2S_IT_ERR, ENABLE);
////  /* Data transfer is performed in the SPI interrupt routine */
//  
//  /* Enable the SPI peripheral */
//  SPI_Cmd(SPIx, ENABLE);
//	

//	if(SPIx==SPI1)
//	{
//		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO,ENABLE);
//		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);

//		GPIO_InitStruct.GPIO_Pin 	= GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
//		GPIO_InitStruct.GPIO_Mode 	= GPIO_Mode_AF_PP;
//		GPIO_InitStruct.GPIO_Speed 	= GPIO_Speed_50MHz;
//								 
//		GPIO_Init(GPIOA, &GPIO_InitStruct);
//	}
//	else if(SPIx==SPI2)
//	{
//		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE);
//		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);

//		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
//		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
//		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
//		
//		GPIO_Init(GPIOB, &GPIO_InitStruct);
//	}

//	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
//	SPI_InitStruct.SPI_Direction= SPI_Direction_2Lines_FullDuplex;
//	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
//	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
//	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
//	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
//	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
//	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
//	SPI_InitStruct.SPI_CRCPolynomial = 7;
//	SPI_Init(SPIx, &SPI_InitStruct);
//	SPI_Cmd(SPIx, ENABLE);
	

}


void hal_spi_send_byte(SPI_TypeDef* SPIx,uint8_t data)
{
	while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE)==RESET);
	SPI_SendData8(SPIx,data);

	while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE)==RESET);
	SPI_ReceiveData8(SPIx);
}

uint8_t hal_spi_receive_byte(SPI_TypeDef* SPIx,uint8_t data)
{
	while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE)==RESET);
	SPI_SendData8(SPIx,data);

	while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE)==RESET);
	return SPI_ReceiveData8(SPIx);
}

static void HAL_SPI_DelayUS(uint8_t t)
{
	uint16_t d;
	while(--t)
		for(d=0;d<1024;d++);
} 

uint8_t hal_spi_chip_select(port_t port, pin_t pin)
{
	hal_io_reset(port, pin);
}

uint8_t hal_spi_chip_unselect(port_t port, pin_t pin)
{
		hal_io_set(port, pin);
}

uint8_t hal_spi_write_reg(uint8_t reg,uint8_t value)
{
//	uint8_t status;
//	CSN(0);
//	status=HAL_SPI_Receive_Byte(SPI1,reg);   //select register  and write value to it
//	HAL_SPI_Send_Byte(SPI1,value);   
//	CSN(1);
//	return(status); 
}

uint8_t hal_spi_read_reg(uint8_t reg)
{
//	uint8_t status;
//	CSN(0);
//	HAL_SPI_Send_Byte(SPI1,reg);
//	status=HAL_SPI_Receive_Byte(SPI1,0);   //select register  and write value to it
//	CSN(1);
//	return(status);
}

uint8_t hal_spi_read_bytes(uint8_t reg, uint8_t *pbuf, uint8_t bytes)
{
//	uint8_t status,byte_ctr;
//	CSN(0);
//	HAL_IO_Low(NRF24L01_CSN_IO);
//	status=HAL_SPI_Receive_Byte(SPI1,reg);       
//	for(byte_ctr=0;byte_ctr<bytes;byte_ctr++)
//		pBuf[byte_ctr]=HAL_SPI_Receive_Byte(SPI1,0);
//	CSN(1);
//	HAL_IO_High(NRF24L01_CSN_IO);
//	return(status);
}


uint8_t hal_spi_write_bytes(uint8_t reg,uint8_t *pbuf,uint8_t bytes)
{
//	uint8_t status,byte_ctr;
//	CSN(0);
//	HAL_IO_Low(NRF24L01_CSN_IO);
//	status=HAL_SPI_Receive_Byte(SPI1,reg); 
//	HAL_SPI_DelayUS(10);
//	for(byte_ctr=0;byte_ctr<bytes;byte_ctr++)
//		HAL_SPI_Send_Byte(SPI1,*pBuf++);
//	CSN(1);
//	HAL_IO_High(NRF24L01_CSN_IO);
//	return(status);
}

void sp1_irq_handler(void)
{
//  uint8_t id;
//#if defined (STM32F0)
//  
//  /* SPI in Master Tramitter mode--------------------------------------- */
//  if (SPI_I2S_GetITStatus(SPI1, SPI_I2S_IT_TXE) == SET)
//  {
//		id =0;
////    if (CmdStatus == 0x00)
////    {
////      SPI_SendData8(SPIx, CmdTransmitted);
////      CmdStatus = 0x01;
////    }
////    else
////    {
////      SPI_SendData8(SPIx, TxBuffer[Tx_Idx++]);
////      if (Tx_Idx == GetVar_NbrOfData())
////      {
////        /* Disable the Tx buffer empty interrupt */
////        SPI_I2S_ITConfig(SPIx, SPI_I2S_IT_TXE, DISABLE);
////      }
////    }
//  }
//  
//  /* SPI in Master Receiver mode--------------------------------------- */
//  if (SPI_I2S_GetITStatus(SPI1, SPI_I2S_IT_RXNE) == SET)
//  {
//		id = 1;
////    if (CmdReceived == 0x00)
////    {
////      CmdReceived = SPI_ReceiveData8(SPIx);
////      Rx_Idx = 0x00;
////    }
////    else
////    {
////      RxBuffer[Rx_Idx++] = SPI_ReceiveData8(SPIx);
////    }
//  }
//  
//  /* SPI Error interrupt--------------------------------------- */
//  if (SPI_I2S_GetITStatus(SPI1, SPI_I2S_IT_OVR) == SET)
//  {
//		id = 2;
////    SPI_ReceiveData8(SPIx);
////    SPI_I2S_GetITStatus(SPIx, SPI_I2S_IT_OVR);
//  }
//  
//#endif /* SPI_MASTER*/
}