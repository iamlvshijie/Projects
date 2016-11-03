#include "bsp.h"
#include "nrf24l01.h"
#include <stdio.h>
#include <string.h>
#include "common.h"
#include "hal_spi.h"
#include "hal_io.h"


//const uint8_t TX_ADDRESS[TX_ADR_WIDTH] = {0x31, 0x36, 0x32, 0x34, 0x30};
//const uint8_t RX_ADDRESS[RX_ADR_WIDTH] = {0x31, 0x36, 0x32, 0x34, 0x30};

const uint8_t RX_ADDRESS0[RX_ADR_WIDTH] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF};	
const uint8_t RX_ADDRESS1[RX_ADR_WIDTH] = {0x31, 0x36, 0x32, 0x34, 0x30};	
const uint8_t RX_ADDRESS2[1]= {0x32};									
const uint8_t RX_ADDRESS3[1]= {0x33};									
const uint8_t RX_ADDRESS4[1]= {0x34};									
const uint8_t RX_ADDRESS5[1]= {0x35};	



uint8_t RX_BUF[TX_PLOAD_WIDTH];
uint8_t TX_BUF[TX_PLOAD_WIDTH];

static void nRF24L01_DelayUS(uint8_t num)
{
	uint8_t i,j; 
	for(i=0;i>num;i++)
		for(j=100;j>0;j--);
}

uint32_t nRF24L01_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	EXTI_InitTypeDef   EXTI_InitStruct;
	NVIC_InitTypeDef   NVIC_InitStruct;
	
	HAL_SPI_Init(NRF24L01_SPI);
	
  RCC_AHBPeriphClockCmd(NRF24L01_CE_GPIO_CLK | NRF24L01_CSN_GPIO_CLK | NRF24L01_IRQ_GPIO_CLK, ENABLE);

  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_Level_3;
	
	GPIO_InitStruct.GPIO_Pin = NRF24L01_CE_PIN;
  GPIO_Init(NRF24L01_CE_GPIO_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = NRF24L01_CSN_PIN;
  GPIO_Init(NRF24L01_CSN_GPIO_PORT, &GPIO_InitStruct);
	
	//	CE = 0, chip enable
	HAL_IO_Low(NRF24L01_CE_IO);
	//	CSN = 1, SPI disable
	HAL_IO_High(NRF24L01_CSN_IO);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_Level_3;
	
	GPIO_InitStruct.GPIO_Pin = NRF24L01_IRQ_PIN;
  GPIO_Init(NRF24L01_IRQ_GPIO_PORT, &GPIO_InitStruct);
	
	/* Enable SYSCFG clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
  /* Connect EXTI0 Line to PA0 pin */
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);

  /* Configure EXTI0 line */
  EXTI_InitStruct.EXTI_Line = EXTI_Line0;
  EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStruct.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStruct);

  /* Enable and set EXTI0 Interrupt */
  NVIC_InitStruct.NVIC_IRQChannel = EXTI0_1_IRQn;
  NVIC_InitStruct.NVIC_IRQChannelPriority = 0x00;
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStruct);
	
	if(nRF24L01_Check()==SUCCESS)
	{
		printf("	nRF24L01 check successfully, and started\r\n");
	
//		CE(0);	  
//		HAL_SPI_Write_Buf(NRF24L01_WRITE_REG+TX_ADDR,(uint8_t*)TX_ADDRESS,RX_ADR_WIDTH);
//		HAL_SPI_Write_Buf(NRF24L01_WRITE_REG+RX_ADDR_P0,(uint8_t*)RX_ADDRESS,RX_ADR_WIDTH);
//		HAL_SPI_Write_Reg(NRF24L01_WRITE_REG+EN_AA,0x3f);    	// all channels auto ack   
//		HAL_SPI_Write_Reg(NRF24L01_WRITE_REG+EN_RXADDR,0x3f);	// all channels enabled
//		HAL_SPI_Write_Reg(NRF24L01_WRITE_REG+RF_CH,0);	     	// 2.4G  
//		HAL_SPI_Write_Reg(NRF24L01_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);// valid rx data width    
//		HAL_SPI_Write_Reg(NRF24L01_WRITE_REG+RF_SETUP,0x0f);	// Tx 0db,2Mbps, low noise amp
//		HAL_SPI_Write_Reg(NRF24L01_WRITE_REG+CONFIG, 0x0f);		// PWR_UP,EN_CRC,16BIT_CRC,
//		CE(1);
		return SUCCESS;
	}
	else
	{
		printf("nRF24L01 check failed\r\n");
		return ERROR;
	}

}

uint32_t nRF24L01_Check(void)
{
	uint8_t check_in_buf[5]={0x11,0x22,0x33,0x44,0x55};
	uint8_t check_out_buf[5]={0x00};

	HAL_SPI_Write_Buf(NRF24L01_WRITE_REG+TX_ADDR,check_in_buf,5);
	HAL_SPI_Read_Buf(NRF24L01_READ_REG+TX_ADDR,check_out_buf,5);
	
	if(BufferCmp(check_in_buf, check_out_buf, 5)==0)
		return SUCCESS;		 
	else
		return ERROR;
}	

 /* 
void nRF24L01_RX_Mode(void)
{
	CE(0);	  
	HAL_SPI_Write_Buf(NRF24L01_WRITE_REG+TX_ADDR,(uint8_t*)RX_ADDRESS0,RX_ADR_WIDTH);
	HAL_SPI_Write_Buf(NRF24L01_WRITE_REG+RX_ADDR_P0,(uint8_t*)RX_ADDRESS0,RX_ADR_WIDTH);
	HAL_SPI_Write_Buf(NRF24L01_WRITE_REG+RX_ADDR_P1,(uint8_t*)RX_ADDRESS1,RX_ADR_WIDTH);	
	HAL_SPI_Write_Buf(NRF24L01_WRITE_REG+RX_ADDR_P2,(uint8_t*)RX_ADDRESS2,1);	
	HAL_SPI_Write_Buf(NRF24L01_WRITE_REG+RX_ADDR_P3,(uint8_t*)RX_ADDRESS3,1);	
	HAL_SPI_Write_Buf(NRF24L01_WRITE_REG+RX_ADDR_P4,(uint8_t*)RX_ADDRESS4,1);	
	HAL_SPI_Write_Buf(NRF24L01_WRITE_REG+RX_ADDR_P5,(uint8_t*)RX_ADDRESS5,1);		
	
	HAL_SPI_Write_Reg(NRF24L01_WRITE_REG+EN_AA,0x3f);    	// all channels auto ack   
	HAL_SPI_Write_Reg(NRF24L01_WRITE_REG+EN_RXADDR,0x3f);	// all channels enabled
	HAL_SPI_Write_Reg(NRF24L01_WRITE_REG+RF_CH,0);	     	// 2.4G+		  
	HAL_SPI_Write_Reg(NRF24L01_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);// valid rx data width    
	HAL_SPI_Write_Reg(NRF24L01_WRITE_REG+RF_SETUP,0x0f);	// Tx 0db,2Mbps, low noise amp
	HAL_SPI_Write_Reg(NRF24L01_WRITE_REG+CONFIG, 0x0f);		// PWR_UP,EN_CRC,16BIT_CRC,
	CE(1); 
}						 
 
void nRF24L01_TX_Mode(void)
{														 
	CE(0);	    
	HAL_SPI_Write_Buf(NRF24L01_WRITE_REG+TX_ADDR,(uint8_t*)TX_ADDRESS,TX_ADR_WIDTH);//?TX???? 
	HAL_SPI_Write_Buf(NRF24L01_WRITE_REG+RX_ADDR_P0,(uint8_t*)RX_ADDRESS,RX_ADR_WIDTH); //??TX????,??????ACK	  
	HAL_SPI_Write_Reg(NRF24L01_WRITE_REG + RX_PW_P0, TX_PLOAD_WIDTH);
	
	HAL_SPI_Write_Reg(NRF24L01_WRITE_REG+EN_AA,0x01);     
	HAL_SPI_Write_Reg(NRF24L01_WRITE_REG+EN_RXADDR,0x01); 
	HAL_SPI_Write_Reg(NRF24L01_WRITE_REG+SETUP_RETR,0x1a);
	HAL_SPI_Write_Reg(NRF24L01_WRITE_REG+RF_CH,0);       
	HAL_SPI_Write_Reg(NRF24L01_WRITE_REG+RF_SETUP,0x0f);  
	HAL_SPI_Write_Reg(NRF24L01_WRITE_REG+CONFIG,0x0e);    
	CE(1);
}
*/

uint32_t nRF24L01_Set_Mode(nRF24L01_ModeDef mode, uint8_t pipe)	
{
	uint8_t temp_RX_ADDRESS[] = {0x30, 0x36, 0x32, 0x34, 0x30};
	if(mode == HubMode)
	{
		printf("work as Hub\r\n");
		printf("Rx address is 0x%x 0x%x 0x%x 0x%x 0x%x\r\n", RX_ADDRESS0[0],RX_ADDRESS0[1],RX_ADDRESS0[2],RX_ADDRESS0[3],RX_ADDRESS0[4]);
		printf("Rx address is 0x%x 0x%x 0x%x 0x%x 0x%x\r\n", RX_ADDRESS1[0],RX_ADDRESS1[1],RX_ADDRESS1[2],RX_ADDRESS1[3],RX_ADDRESS1[4]);
		printf("Rx address is 0x%x\r\n", RX_ADDRESS2[0]);
		printf("Rx address is 0x%x\r\n", RX_ADDRESS3[0]);
		printf("Rx address is 0x%x\r\n", RX_ADDRESS4[0]);
		printf("Rx address is 0x%x\r\n", RX_ADDRESS5[0]);
		
		CE(0);
		HAL_SPI_Write_Buf(NRF24L01_WRITE_REG+TX_ADDR,(uint8_t*)RX_ADDRESS0,RX_ADR_WIDTH);
		HAL_SPI_Write_Buf(NRF24L01_WRITE_REG+RX_ADDR_P0,(uint8_t*)RX_ADDRESS0,RX_ADR_WIDTH);
		HAL_SPI_Write_Buf(NRF24L01_WRITE_REG+RX_ADDR_P1,(uint8_t*)RX_ADDRESS1,RX_ADR_WIDTH);	
		HAL_SPI_Write_Buf(NRF24L01_WRITE_REG+RX_ADDR_P2,(uint8_t*)RX_ADDRESS2,1);	
		HAL_SPI_Write_Buf(NRF24L01_WRITE_REG+RX_ADDR_P3,(uint8_t*)RX_ADDRESS3,1);	
		HAL_SPI_Write_Buf(NRF24L01_WRITE_REG+RX_ADDR_P4,(uint8_t*)RX_ADDRESS4,1);	
		HAL_SPI_Write_Buf(NRF24L01_WRITE_REG+RX_ADDR_P5,(uint8_t*)RX_ADDRESS5,1);		
		
		HAL_SPI_Write_Reg(NRF24L01_WRITE_REG+EN_AA,0x3f);    	// all channels auto ack   
		HAL_SPI_Write_Reg(NRF24L01_WRITE_REG+EN_RXADDR,0x3f);	// all channels enabled
		HAL_SPI_Write_Reg(NRF24L01_WRITE_REG+RF_CH,0);	     	// 2.4G+	
	  
		HAL_SPI_Write_Reg(NRF24L01_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);// valid rx data width    
		HAL_SPI_Write_Reg(NRF24L01_WRITE_REG+RX_PW_P1,RX_PLOAD_WIDTH);// valid rx data width 
		HAL_SPI_Write_Reg(NRF24L01_WRITE_REG+RX_PW_P2,RX_PLOAD_WIDTH);// valid rx data width   
		HAL_SPI_Write_Reg(NRF24L01_WRITE_REG+RX_PW_P3,RX_PLOAD_WIDTH);// valid rx data width   
		HAL_SPI_Write_Reg(NRF24L01_WRITE_REG+RX_PW_P4,RX_PLOAD_WIDTH);// valid rx data width   
		HAL_SPI_Write_Reg(NRF24L01_WRITE_REG+RX_PW_P5,RX_PLOAD_WIDTH);// valid rx data width   
		
		HAL_SPI_Write_Reg(NRF24L01_WRITE_REG+RF_SETUP,0x0f);	// Tx 0db,2Mbps, low noise amp
		HAL_SPI_Write_Reg(NRF24L01_WRITE_REG+CONFIG, 0x0f);		// PWR_UP,EN_CRC,16BIT_CRC, RX
  	CE(1);
		nRF24L01_DelayUS(10);
		return SUCCESS;
	}
  else if (mode == TerminalMode)
	{
		printf("work as terminal, pipe is %d\r\n", pipe);
		CE(0);
		if(pipe==0)
		{
			HAL_SPI_Write_Buf(NRF24L01_WRITE_REG+TX_ADDR,(uint8_t*)RX_ADDRESS0,RX_ADR_WIDTH);
			HAL_SPI_Write_Buf(NRF24L01_WRITE_REG+RX_ADDR_P0,(uint8_t*)RX_ADDRESS0,RX_ADR_WIDTH);
		}
		else if(pipe<6)
		{
			temp_RX_ADDRESS[0] = 0x30 + pipe;
			printf("Tx address is 0x%x 0x%x 0x%x 0x%x 0x%x\r\n", temp_RX_ADDRESS[0],temp_RX_ADDRESS[1],temp_RX_ADDRESS[2],temp_RX_ADDRESS[3],temp_RX_ADDRESS[4]);
			HAL_SPI_Write_Buf(NRF24L01_WRITE_REG+TX_ADDR,(uint8_t*)temp_RX_ADDRESS,RX_ADR_WIDTH);
			HAL_SPI_Write_Buf(NRF24L01_WRITE_REG+RX_ADDR_P0,(uint8_t*)temp_RX_ADDRESS,RX_ADR_WIDTH);
		}
		else 
			return ERROR;
		
		HAL_SPI_Write_Reg(NRF24L01_WRITE_REG+EN_AA,0x01);    	// 0 channel auto ack   
		HAL_SPI_Write_Reg(NRF24L01_WRITE_REG+EN_RXADDR,0x01);	// 0 channel enabled
		HAL_SPI_Write_Reg(NRF24L01_WRITE_REG+RF_CH,0);	     	// 2.4G  
		HAL_SPI_Write_Reg(NRF24L01_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);// valid rx data width 		
		HAL_SPI_Write_Reg(NRF24L01_WRITE_REG+RF_SETUP,0x0f);	// Tx 0db,2Mbps, low noise amp
		HAL_SPI_Write_Reg(NRF24L01_WRITE_REG+CONFIG, 0x0f);		// PWR_UP,EN_CRC,16BIT_CRC, RX
		CE(1);
		nRF24L01_DelayUS(10);
		return SUCCESS;
	}
	else
	{
		return ERROR;
	}
} 

void nRF24L01_Config(void)
{
  //initial io
	//CE(0);          //        CE=0 ;chip enable
	//CSN(1);       //CSN=1   Spi disable
	HAL_SPI_Write_Reg(NRF24L01_WRITE_REG + CONFIG, 0x0e); // Set PWR_UP bit, enable CRC(2 bytes) &Prim:RX. RX_DR enabled..
	HAL_SPI_Write_Reg(NRF24L01_WRITE_REG + EN_AA, 0x3f);
	HAL_SPI_Write_Reg(NRF24L01_WRITE_REG + EN_RXADDR, 0x3f); // Enable Pipe0
//	HAL_SPI_Write_Reg(NRF24L01_WRITE_REG + SETUP_AW, 0x02); // Setup address width=5 bytes
//	HAL_SPI_Write_Reg(NRF24L01_WRITE_REG + SETUP_RETR, 0x1a); // 500us + 86us, 10 retrans...
	HAL_SPI_Write_Reg(NRF24L01_WRITE_REG + RF_CH, 0);
	HAL_SPI_Write_Reg(NRF24L01_WRITE_REG + RF_SETUP,0x07); // TX_PWR:0dBm, Datarate:2Mbps,
}

uint8_t nRF24L01_RxPacket(uint8_t* rx_buf)
{

  uint8_t status,revale=0;
	CE(0);
	//printf("enter rx packet\r\n");
//	nRF24L01_DelayUS(10);
//	status=HAL_SPI_Receive_Byte(SPI1,STATUS);	

//	if(status & RX_DR)	
//	{	
		//printf("Rx a packet: status is 0x%x\r\n",status);
		HAL_SPI_Read_Buf(RD_RX_PLOAD,rx_buf,TX_PLOAD_WIDTH);// read receive payload from RX_FIFO buffer
		revale =1;			//读取数据完成标志
//	}
	HAL_SPI_Write_Reg(NRF24L01_WRITE_REG + STATUS,status);   //接收到数据后RX_DR,TX_DS,MAX_PT都置高为1，通过写1来清楚中断标志
	CE(1);
	return revale;	
}


void nRF24L01_TxPacket(uint8_t* tx_buf)
{
	uint8_t status = 0x00;
	CE(0);
	HAL_SPI_Write_Buf(WR_TX_PLOAD, tx_buf, TX_PLOAD_WIDTH); 			
	HAL_SPI_Write_Reg(NRF24L01_WRITE_REG + CONFIG, 0x0e);   		
	nRF24L01_DelayUS(15);
	CE(1);		
	
	while(HAL_IO_Read(NRF24L01_IRQ_IO)==1);
	status=HAL_SPI_Read_Reg(STATUS);
	HAL_SPI_Write_Reg(NRF24L01_WRITE_REG + STATUS, status);
	if(status&TX_DS)	
	{
		printf("TX_DS\r\n");
	}
	if(status&MAX_RT)
	{
		HAL_SPI_Write_Reg(FLUSH_TX, 0xFF);
		printf("MAX_RT\r\n");
	}
	
	CE(0);			
	HAL_SPI_Write_Reg(NRF24L01_WRITE_REG + CONFIG, 0x0f); 
	nRF24L01_DelayUS(15);
	CE(1);	
}

	

// 
//void nRF24L01_Send(void)
//{
//    uint8_t status=0x00;
////	CSN(0);
//	//TX_Mode(TX_BUF);
//	while(IRQ);

//	CE(0);
//	//nRF24L01_DelayUS(10);
//	status=HAL_SPI_Read_Reg(STATUS);	// 读取状态寄存其来判断数据接收状况
//	if(status&TX_DS)	/*tx_ds == 0x20*/
//	{
//		printf("STATUS接受状态：0x%2x\r\n",status);
//		printf("\r\n发送完数据：%s\r\n",RX_BUF);	
//		HAL_SPI_Write_Reg(NRF24L01_WRITE_REG + STATUS, 0x20);      // 清除TX，让IRQ拉低；
//			
//	}
//	else if(status&MAX_RT)
//		{
//			printf("发送达到最大发送次数");	
//			HAL_SPI_Write_Reg(NRF24L01_WRITE_REG + STATUS, 0x10);      // 清除TX，让IRQ拉低；			
//		}
//	CE(1);
////	status=20;
//}

//void nRF24L01_Receive(void)
//{   
//  uint8_t i,status=0x01;  

//	while(IRQ);
//	printf("Rx a Packet\n");
//	CE(0);
//	nRF24L01_DelayUS(10);
//	//status=SPI_Read_Reg(STATUS);	// 读取状态寄存其来判断数据接收状况
//	printf("STATUS接受状态：0x%2x\r\n",status);
//	if(status & 0x40)			//接受中断标志位
//	{
//		printf("接受成功");
////		CE(1);
//		HAL_SPI_Read_Buf(RD_RX_PLOAD,RX_BUF,TX_PLOAD_WIDTH);// read receive payload from RX_FIFO buffer

//		printf("\r\n i=%d,接收到数据：%x\r\n",i,RX_BUF[0]);
////		for(i=0;i<32;i++)
////		{
////			RX_BUF[1] = 0X06;
////			printf("\r\n i=%d,接收到数据：%x\r\n",i,RX_BUF[i]);
////		}
//		HAL_SPI_Write_Reg(NRF24L01_WRITE_REG + STATUS, 0x40);      // 清除TX，让IRQ拉低
//	}  
//	CE(1);

//}









