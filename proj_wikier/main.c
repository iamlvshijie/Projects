/**
  ******************************************************************************
  * @file    PSS.c 
  * @author  I2T Team
  * @version V1.4.0
  * @date    24-July-2014
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stdio.h"
#include <string.h>
#include "math.h"
#include "hal_uart.h"
#include "hal_systick.h"
#include "led.h"
#include "hx711.h"
#include "nrf24l01.h"
#include "hal_spi.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/																																

/* Private function prototypes -----------------------------------------------*/
static void 		COM_Config(void);
																										
static void 		IO_Config(void);

//static void 		TIM_Config(void);

//static void     EXIT_Config(void);
//static void 		NVIC_Config(void);

//static void 		RTC_Config(void);

//static uint32_t GetLSIFrequency(void);

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

uint8_t txBuffer[TX_PLOAD_WIDTH]={0};
uint8_t rxBuffer[RX_PLOAD_WIDTH];
volatile uint8_t rxFlag=0;		
const uint8_t MyDeviceID = 0x03;
/* Private functions ---------------------------------------------------------*/


/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
#define	PSS_N (3)
#define CONST_G (9.780327)

int main(void)
{
	uint32_t TempWeight[PSS_N], MyWeight;//1[HX711_N], MyWeight2[HX711_N], PSS_W[3], PSS_avW;
	float PSS_L, PSS_H, PSS_a, PSS_b, PSS_PH1, PSS_PH2, PSS_G;
	float SinBeta1, CosBeta1, SinBeta2, CosBeta2;
	uint8_t PSS_Index, HX711_x;
	uint8_t cmd;
	uint32_t pipe;
	uint32_t inputWeight;
	uint8_t PSS_Status = SUCCESS;
	
	COM_Config();
	printf("@Project: PSS Collector by I2T Team\r\n@Revision: v1.0.1\r\n@Email:lvshijie@pku.edu.cn\r\n@Copyright:Skyrover Tech Inc.\r\n");
	
	printf(">Systick timer initializing ...");
	if(HAL_Systick_Init()==ERROR)
	{
		printf("Failed\r\n");
		while(1);
	}
	else
		printf("Succeed\r\n");

	HX711_Init(HX711_0);		
	
	printf(">Please input 'c' to get into the calibration mode\r\n");
	HAL_Systick_Delay(300000);	
	if(USART_GetFlagStatus(USART1,USART_FLAG_RXNE) == SET)
	{
		USART_ReceiveData(USART1);
		printf("Please to be placed on the PSS collector\r\n");
		do
		{
			printf("input the calibration weight(g),if '0' is input, collector returns to normal work mode\r\n");
			scanf("%d",&inputWeight);
			printf("\r\n");
			if(inputWeight>0)
				HX711_Calibration(HX711_0, inputWeight);
		}
		while(inputWeight>0);
		
	}
	else
		printf("No calibration, contiue starting\r\n");
	printf(">nRF24L01 is Initlizing ... ");
	nRF24L01_Init();
	nRF24L01_Set_Mode(TerminalMode, MyDeviceID);	
  while(1)
  {	

//		uint8_t status,revale=0;
//		CE(0);
//		
//		if((rxFlag&0x80)==0x80)//|| HAL_IO_Read(NRF24L01_IRQ_IO)== 0x00)
//		{	printf("rx a packet from channel %d\r\n", rxFlag&0x7F);
//			if(nRF24L01_RxPacket((uint8_t*)rxBuffer)==1)
//			{
//				printf("rxBuffer is %s\r\n", rxBuffer);
//			}
//			rxFlag = 0;		
//		
//		}
//		while(HAL_IO_Read(NRF24L01_IRQ_IO));
//		printf("irq is low\r\n");

//		}		

//		printf("please type a char\r\n");
//		scanf("%c",&cmd);

//		txBuffer[0]=2;
//		txBuffer[1]=cmd;
//		txBuffer[2]=cmd+1;

//		printf("txBuffer is %s\r\n", txBuffer);
//		nRF24L01_TxPacket((uint8_t*)txBuffer);
		HAL_Systick_Delay(100000);	
		PSS_Status = SUCCESS;
		printf(">Device %d: Weight Measured(g): ", MyDeviceID);
		for(PSS_Index=0;PSS_Index<PSS_N;PSS_Index++)
		{
			if(HX711_Get_NetWeight(HX711_0, (TempWeight+PSS_Index))==SUCCESS)
				printf(" %d ", *(TempWeight+PSS_Index));
			else
			{
				PSS_Status = ERROR;
				break;
				printf(" error, ");
			}
		}
		if(PSS_Status == ERROR)
		{
			printf("Current HX711 data is error, skipped ... \r\n");
			continue;
		}
		MyWeight = (TempWeight[0]+TempWeight[1]+TempWeight[2])/3;
		printf(", average weight: %d\r\n",MyWeight);			
	
		//MyWeight = 16;
		txBuffer[0] = 5;
		txBuffer[1] = MyDeviceID;
		txBuffer[2] = (uint8_t)MyWeight;
		txBuffer[3] = (uint8_t)(MyWeight>>8);
		txBuffer[4] = (uint8_t)(MyWeight>>16);
		txBuffer[5] = (uint8_t)(MyWeight>>24);
		txBuffer[6] = txBuffer[1]^txBuffer[2]^txBuffer[3]^txBuffer[4]^txBuffer[5];
		
		nRF24L01_TxPacket((uint8_t*)txBuffer);
		printf("txBuffer[1](MyDeviceID) txBuffer[2] txBuffer[3] txBuffer[4] txBuffer[5] txBuffer(checksum) is %d %d %d %d %d %d\r\n", txBuffer[1], txBuffer[2], txBuffer[3], txBuffer[4], txBuffer[5], txBuffer[6]);		
	
		//printf("Process 1/3: please place the model, input PSS_H to start measuring\r\n>");
//		scanf("%f",&PSS_H);	
//		printf("PSS_H is %.2f(mm), 1st measuring started!", PSS_H);	
//		
//		SinBeta1 = PSS_H/PSS_L;
//		CosBeta1 = sqrt(PSS_L*PSS_L-PSS_H*PSS_H)/PSS_L;
//		
//		LED_Toggle(LEDG_IO);
//		
//		for(HX711_x = HX711_0;HX711_x<HX711_N;HX711_x++)
//		{
//			printf("\r\nSensor%d: Weight Measured(g): ", HX711_x);
//			for(PSS_Index=0;PSS_Index<PSS_N;PSS_Index++)
//			{
//				if(HX711_Get_NetWeight(HX711_x, (TempWeight+PSS_Index))==SUCCESS)
//					printf(" %d ", *(TempWeight+PSS_Index));
//			}
//			MyWeight1[HX711_x] = (TempWeight[0]+TempWeight[1]+TempWeight[2])/3;
//			printf(", average weight: %d ",MyWeight1[HX711_x]);
//		}
//		PSS_W[1] = MyWeight1[0]+MyWeight1[1] + MyWeight1[2];
//		printf("\r\nPSS_W is %d\r\n",PSS_W[1]);
//		
//		printf("Process 2/3: please input PSS_H, enter to start measuring\r\n>");
//		scanf("%f",&PSS_H);	
//		printf("PSS_H is %.2f(mm), 2nd measuring started!", PSS_H);	
//		
//		SinBeta2 = PSS_H/PSS_L;
//		CosBeta2 = sqrt(PSS_L*PSS_L-PSS_H*PSS_H)/PSS_L;
//		
//		LED_Toggle(LEDG_IO); 
//		
//		for(HX711_x = HX711_0;HX711_x<HX711_N;HX711_x++)
//		{
//			printf("\r\nSensor%d: Weight Measured(g): ", HX711_x);
//			for(PSS_Index=0;PSS_Index<PSS_N;PSS_Index++)
//			{
//				if(HX711_Get_NetWeight(HX711_x, (TempWeight+PSS_Index))==SUCCESS)
//					printf(" %d ", *(TempWeight+PSS_Index));
//			}
//			MyWeight2[HX711_x] = (TempWeight[0]+TempWeight[1]+TempWeight[2])/3;
//			printf(", average weight: %d ",MyWeight2[HX711_x]);
//		}
//		PSS_W[2] = MyWeight2[0]+MyWeight2[1]+MyWeight2[2];
//		printf("\r\nPSS_W is %d\r\n",PSS_W[2]);
//		
//		printf("Process 3/3: calculating ...\r\n");
//		PSS_avW = (PSS_W[0]+PSS_W[1]+PSS_W[2])/3;
//		PSS_G = PSS_avW*CONST_G/1000;
//		PSS_PH1 = MyWeight1[0]*CONST_G/1000;
//		PSS_PH2 = MyWeight2[0]*CONST_G/1000;
//		
//		PSS_a = PSS_L*(PSS_PH1*CosBeta1*SinBeta2-PSS_PH2*CosBeta2*SinBeta1)/(PSS_G*(SinBeta2*CosBeta1-SinBeta1*CosBeta2));
//		PSS_b = PSS_L*CosBeta1*CosBeta2*(PSS_PH1-PSS_PH2)/(PSS_G*(SinBeta2*CosBeta1-SinBeta1*CosBeta2));
//		printf("PSS_PH1 is %f, PSS_PH2 is %f, SinBeta1 is %f, CosBeta1 is %f, SinBeta2 is %f, CosBeta2 is %f, PSS_avW is %d\r\nPSS_a is %f, PSS_b is %f \r\n", PSS_PH1, PSS_PH2, SinBeta1, CosBeta1, SinBeta2, CosBeta2, PSS_avW, PSS_a, PSS_b);
  }
}

/**
  * @brief Configure the USART Device
  * @param  None
  * @retval None
  */
static void COM_Config(void)
{ 
	USART_InitTypeDef USART_InitStructure;
	/* USARTx configured as follow:
  - BaudRate = 115200 baud  
  - Word Length = 8 Bits
  - Stop Bit = 1 Stop Bit
  - Parity = No Parity
  - Hardware flow control disabled (RTS and CTS signals)
  - Receive and transmit enabled
  */
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  
	HAL_UART_Init(DEBUG_COM, &USART_InitStructure);
}


void EXTI0_1_IRQHandler(void)
{
	uint8_t status;
  if(EXTI_GetITStatus(EXTI_Line0) != RESET)
  {
		CE(0);
		status=HAL_SPI_Read_Reg(STATUS);
		printf(">INT0 status is 0x%x\r\n",status);
		
		if(status&RX_DR)
		{
			HAL_SPI_Read_Buf(RD_RX_PLOAD,rxBuffer,TX_PLOAD_WIDTH);// read receive payload from RX_FIFO buffer
			printf("rx a packet from channel %d, data is %s\r\n", (status & RX_P_NO)>>2, rxBuffer);
			HAL_SPI_Write_Reg(NRF24L01_WRITE_REG + STATUS,status); 
			//rxFlag = 0x80 + (status & RX_P_NO)>>2;
		}
		CE(1);
    /* Clear the EXTI line 0 pending bit */
    EXTI_ClearITPendingBit(EXTI_Line0);
  }
}


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
