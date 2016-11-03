#include "esp8266.h"

#include "string.h"

#include "common.h"

#include "ee_memory.h"

#include "uart_debug.h"

#include "hal_systick.h"
#include "hal_uart.h"

/*For uart interrupt*/

extern __IO uint8_t ReceiveState;
extern __IO uint16_t pktLength;	

uint8_t CounterUpdateFlag =0; 

/* Buffer used for transmission */
uint8_t ATBuffer[PKT_MAX_LENGTH]={""};

extern Packet_TypeDef ReceivePacket;
extern Packet_TypeDef TransmitPacket;

uint8_t RxBuffer[BUFFER_SIZE]={0};
uint8_t TxBuffer[PKT_MAX_LENGTH]={0};

__IO uint8_t WifiTransparencyFlag = RESET; 

/* For Device info */
extern uint8_t Device_SN[];
extern uint8_t Device_PID[];
extern uint8_t Device_HW_REV[];
extern uint8_t Device_FW_REV[];


extern uint8_t Device_Date_Time[];

/* For WIFI Settings  */
uint8_t Device_AT_Version[8] ={0};
uint8_t Device_IP[12]={0};

uint8_t Device_WIFI_SSID[32]={0};
uint8_t Device_WIFI_PSWD[32]={0};
uint8_t Device_SERVER_IP[SERVER_IP_SIZE];
uint8_t Device_SERVER_PORT[SERVER_PORT_SIZE];


/* fuction check status 
*  
*/
uint8_t WIFI_Check_Status(void)
{
	uint8_t status, *pos;
	/* send wifi configure cmds */
#ifdef USING_UNVARNISHED	
	WifiTransparencyFlag =SET;
	
	BufferReset(RxBuffer, BUFFER_SIZE);
	CounterUpdateFlag =1;
	/* Send echo packet to server to check server connection unvarished mode */

	WIFI_Send_Packet(TEST_PKT, NULL, NEED_ACK);
	
	HAL_Systick_Delay(6000);
	HAL_Systick_Set_Timeout(300);
	if(WIFI_Receive_Packet(&ReceivePacket,HAL_Systick_Get_Timeout()))
	{
		if(ReceivePacket.type == RESPONSE_PKT){
			WifiTransparencyFlag =SET;
			return CONNECTED;
		}	
	}
	WIFI_Quit_Unvarnished();
#endif	

	//check new wifi module
	BufferReset(ATBuffer, BUFFER_SIZE);
	CounterUpdateFlag =1;
	HAL_UART_Send_String(WIFI_COM,"AT+CWMODE?\r\n");
	HAL_Systick_Delay(10);

	if(strstr(ATBuffer,"OK"))
	{
		pos = strstr(ATBuffer,"CWMODE:");
		
		status = *(pos+7);
		if(strstr(ATBuffer,"CWMODE?")){
			HAL_UART_Send_String(WIFI_COM,"ATE0\r\n");
			HAL_Systick_Delay(10);
		}
			
		CounterUpdateFlag =1;
		BufferReset(ATBuffer, BUFFER_SIZE);
    
		if(status==0x32)
			return FIRST_CONFIG;
	}

	/* check connection status */
	HAL_UART_Send_String(WIFI_COM,"AT+CIPSTATUS\r\n");
	HAL_Systick_Delay(10);
	
	if(strstr(ATBuffer,"OK"))
	{
		pos = strstr(ATBuffer,"STATUS:");
		
		status = *(pos+7);
		CounterUpdateFlag =1;
		BufferReset(ATBuffer, BUFFER_SIZE);
		if(status==CONNECTED || status == AP_JOINED || status == NO_AP)	
			return status;
	}

	/* check connection status */
	HAL_UART_Send_String(WIFI_COM,"AT+CWJAP?\r\n");
	HAL_Systick_Delay(10);
	
	if(strstr(ATBuffer,"OK"))
	{		
		if(strstr(ATBuffer,"CWJAP:"))
		{
			CounterUpdateFlag =1;
			BufferReset(ATBuffer, BUFFER_SIZE);
			return AP_JOINED;
		}
		else if(strstr(ATBuffer,"No AP"))
		{
			CounterUpdateFlag =1;
			BufferReset(ATBuffer, BUFFER_SIZE);
			return NO_AP;
		}	
	}
	
	return OTHER;
}
/**
  * @brief  Configures the wifi module.
  * @param  None
  * @retval index: which indicates the line number of TxBuffer cmd strings
	*								 0-4:	fail  5: success				
  */
uint8_t WIFI_Config(void)
{
	uint8_t index =0, *pos;
	BufferReset(ATBuffer, BUFFER_SIZE);
	CounterUpdateFlag =1;
																							
	/* get AT version*/																								
	HAL_UART_Send_String(WIFI_COM,"AT+GMR\r\n");
	HAL_Systick_Delay(30);
	if(strstr(ATBuffer,"OK")){
		pos=strstr(ATBuffer,"AT version");
		BufferCpy(Device_AT_Version,  pos+11, 8);
		CounterUpdateFlag =1;
		BufferReset(ATBuffer, BUFFER_SIZE);
	}
	else if(strstr(ATBuffer,"ERROR"))
	{
		CounterUpdateFlag =1;
		BufferReset(ATBuffer, BUFFER_SIZE);		
		return ERROR;
	}
	else
		return ERROR;
		
		
	/* Disable AT CMD Echo*/	
	HAL_UART_Send_String(WIFI_COM,"ATE0\r\n");
	HAL_Systick_Delay(10);
	if(strstr(ATBuffer,"OK"))
	{
		CounterUpdateFlag =1;
		BufferReset(ATBuffer, BUFFER_SIZE);
	}
	else if(strstr(ATBuffer,"ERROR"))
	{
		CounterUpdateFlag =1;
		BufferReset(ATBuffer, BUFFER_SIZE);		
		return ERROR;
	}	
	else
		return ERROR;
	
	/* set CWMODE */
	HAL_UART_Send_String(WIFI_COM,"AT+CWMODE=1\r\n");
	HAL_Systick_Delay(10);
	if(strstr(ATBuffer,"OK"))
	{
		CounterUpdateFlag =1;
		BufferReset(ATBuffer, BUFFER_SIZE);	
	}
	else if(strstr(ATBuffer,"ERROR"))
	{
		CounterUpdateFlag =1;
		BufferReset(ATBuffer, BUFFER_SIZE);		
		return ERROR;
	}
	else
		return ERROR;
		
  return SUCCESS;			
}

/* join a AP or router(airkiss protocol), then get an IP address	*/
uint8_t WIFI_Join_AP(uint32_t* timeout)
{
	uint8_t *pos, i=0;
	uint8_t string[128] = "AT+CWJAP_DEF=\"";
	
	/* Read from EEROM SSID&PSWD */
	if(Memory_Read_Contrl_Reg(CTRL_REG1, SSID_BIT0))
	{
		BufferOffsetCpy(string,Device_WIFI_SSID,14,1,Device_WIFI_SSID[0]);
		BufferOffsetCpy(string,"\",\"",14+Device_WIFI_SSID[0],0,3);
		BufferOffsetCpy(string,Device_WIFI_PSWD,17+Device_WIFI_SSID[0],1,Device_WIFI_PSWD[0]);
		BufferOffsetCpy(string,"\"\r\n",17+Device_WIFI_SSID[0]+Device_WIFI_PSWD[0],0,3);
		printf("Read SSID&PSWD from EEPROM, ");
		/* conect AP directly */ 
		HAL_UART_Send_String(WIFI_COM, string);
		HAL_Systick_Delay(4000);
		if(strstr(ATBuffer,"CONNECTED"))
		{
			CounterUpdateFlag =1;
			BufferReset(ATBuffer, BUFFER_SIZE);	
			printf(" and Join AP successfully!\r\n ");
			return SUCCESS;
		}
		else //if(strstr(ATBuffer,"ERROR"))
		{
			CounterUpdateFlag =1;
			BufferReset(ATBuffer, BUFFER_SIZE);
			printf(" and Failed to Join AP!\r\n ");
		}	
	}
	printf("Starting Airkiss Mode...");
	/* Start AirKiss Mode */
	HAL_UART_Send_String(WIFI_COM, "AT+CWSMARTSTART=2\r\n");
	
	HAL_Systick_Delay(10000);
	
	if(strstr(ATBuffer,"SMART SUCCESS"))
	{
		HAL_Systick_Delay(20);
		pos=strstr(ATBuffer,"SSID")+5;
		printf("success\r\n");
		while(*(pos+i)!=0x0D)
		{
			Device_WIFI_SSID[i+1]=*(pos+i);
			i++;
		}
		Device_WIFI_SSID[0]=i;
		
			
		i=0;
		pos=strstr(ATBuffer,"PASSWORD")+9;
		while(*(pos+i)!=0x0D)
		{
			Device_WIFI_PSWD[i+1]=*(pos+i);
			i++;
		}
		Device_WIFI_PSWD[0]=i;
		
		Memory_Write_Buffer(Device_WIFI_SSID, SSID_ADDRESS, SSID_SIZE);	
		Memory_Write_Buffer(Device_WIFI_PSWD, PSWD_ADDRESS, PSWD_SIZE);
		
		Memory_Write_Contrl_Reg(CTRL_REG1, SSID_BIT0, SET);
		
		#ifdef DEBUG_PRINT
			printf(ATBuffer);
		#endif
		printf("  AP CONNECTED ");
		
		CounterUpdateFlag =1;
		BufferReset(ATBuffer, BUFFER_SIZE);	
	
		return SUCCESS;
	}
	else
		return ERROR;

//	/* Get IP addr*/
//	HAL_UART_Send_String(WIFI_COM, "AT+CIPSTA?\r\n");
//	HAL_Systick_Delay(10);
//	if(strstr(ATBuffer,"OK"))
//	{
//		pos = strstr(ATBuffer,"CIPSTA")+8;
//#ifdef DEBUG_PRINT
//	printf(ATBuffer);
//#endif				
//		CounterUpdateFlag =1;
//		BufferReset(ATBuffer, BUFFER_SIZE);	
//		return SUCCESS;
//	}
//	else //if(strstr(ATBuffer,"ERROR"))
//	{
//		CounterUpdateFlag =1;
//		BufferReset(ATBuffer, BUFFER_SIZE);
//		return ERROR;
//	}	
	
}

uint8_t WIFI_Connect_Server(void)
{
	uint8_t status, *pos;
	//try to connect server
#ifdef USING_UDP
	HAL_UART_Send_String(WIFI_COM,"AT+CIPSTART=\"UDP\",\"203.195.207.140\",8080,8000\r\n");
#elif defined	USING_TCP
	#ifdef USING_DNS
		HAL_UART_Send_String(WIFI_COM,"AT+CIPSTART=\"TCP\",\"water.900315.com\",2348\r\n");
	#else
		HAL_UART_Send_String(WIFI_COM,"AT+CIPSTART=\"TCP\",\"203.195.206.133\",2348\r\n");
	#endif
#endif	
  HAL_Systick_Delay(10000);
	if(strstr(ATBuffer,"ERROR"))
	{
		CounterUpdateFlag =1;
		BufferReset(ATBuffer, BUFFER_SIZE);
		return ERROR;
	}
	else if(strstr(ATBuffer,"CONNECT"))
	{
		CounterUpdateFlag =1;
		BufferReset(ATBuffer, BUFFER_SIZE);
	}
	else
		return ERROR;
	
	/* check connection status */
//	HAL_UART_Send_String(WIFI_COM,"AT+CIPSTATUS\r\n");
//	HAL_Systick_Delay(10);
//	
//	if(strstr(ATBuffer,"OK"))
//	{
//		pos = strstr(ATBuffer,"STATUS:");
//		status = *(pos+7);
//		CounterUpdateFlag =1;
//		BufferReset(ATBuffer, BUFFER_SIZE);
//		if(status!=CONNECTED)	
//			return ERROR;
//	}

	return SUCCESS;
} 	

uint8_t WIFI_Quit_Server(void)
{
	HAL_UART_Send_String(WIFI_COM, "AT+CIPCLOSE\r\n");
  HAL_Systick_Delay(10);
	if(strstr(ATBuffer,"CLOSED"))
	{

		CounterUpdateFlag =1;
		BufferReset(ATBuffer, BUFFER_SIZE);
	}
	else if(strstr(ATBuffer,"ERROR"))
	{
		CounterUpdateFlag =1;
		BufferReset(ATBuffer, BUFFER_SIZE);
		return ERROR;
	}

  return SUCCESS;
}


uint8_t  WIFI_Set_Unvarnished(void)
{

	HAL_UART_Send_String(WIFI_COM,"AT+CIPMODE=1\r\n");
	HAL_Systick_Delay(100);
	if(strstr(ATBuffer,"OK"))
	{
		CounterUpdateFlag =1;
		BufferReset(ATBuffer, BUFFER_SIZE);
	}
	else if(strstr(ATBuffer,"ERROR"))
	{
		CounterUpdateFlag =1;
		BufferReset(ATBuffer, BUFFER_SIZE);
		return ERROR;
	}
	
	/* Save unvarnished settings to flash */
#ifdef USING_UDP	
//	HAL_UART_Send_String(WIFI_COM,"AT+SAVETRANSLINK=1,\"203.195.207.140\",8080,\"UDP\"\r\n");
//	HAL_Systick_Delay(100);
	
#elif defined USING_TCP 
 	HAL_UART_Send_String(WIFI_COM,"AT+SAVETRANSLINK=1,\"203.195.207.140\",2348,\"TCP\"\r\n");
	HAL_Systick_Delay(100);
#endif	
	
	if(strstr(ATBuffer,"OK"))
	{
		CounterUpdateFlag =1;
		BufferReset(ATBuffer, BUFFER_SIZE);
	}

	
	HAL_UART_Send_String(WIFI_COM,"AT+CIPSEND\r\n");
	HAL_Systick_Delay(10);
	if(strstr(ATBuffer,">"))
	{
		WifiTransparencyFlag= SET;
		CounterUpdateFlag =1;
		BufferReset(ATBuffer, BUFFER_SIZE);
	}
	return SUCCESS;
}

uint8_t WIFI_Reset_Unvarnished(void)
{
	HAL_UART_Send_String(WIFI_COM,"AT+CIPMODE=0\r\n");
	HAL_Systick_Delay(10);
	//"CLOSED"
}


uint8_t WIFI_Restore(void)
{
	HAL_UART_Send_String(WIFI_COM,"AT+RESTORE\r\n");
	HAL_Systick_Delay(20);
	if(strstr(ATBuffer,"OK"))
		return SUCCESS;
	else
		return ERROR;
}


uint8_t WIFI_Quit_Unvarnished(void)
{
	WifiTransparencyFlag = RESET;
	HAL_UART_Send_String(WIFI_COM,"+++");
  HAL_Systick_Delay(50);
	if(strstr(ATBuffer,"CLOSED"))
		;
	else
		return ERROR;
	
	BufferReset(ATBuffer, BUFFER_SIZE);
	CounterUpdateFlag =1;

	HAL_UART_Send_String(WIFI_COM,"AT+CIPMODE=0\r\n");
  HAL_Systick_Delay(10);
	if(strstr(ATBuffer,"OK"))
		return SUCCESS;
	else
		return ERROR;
}


uint8_t WIFI_Send_Packet(uint8_t type, uint8_t* data, uint8_t ack)
{
	uint16_t TxPacketOffset = 0;
	switch(type)
	{
		case RESPONSE_PKT:
			TxBuffer[TxPacketOffset++] = (uint8_t)RESPONSE_PKT_LEN>>8;
		  TxBuffer[TxPacketOffset++] = (uint8_t)RESPONSE_PKT_LEN;
			TxBuffer[TxPacketOffset++] = RESPONSE_PKT;
			BufferOffsetCpy(TxBuffer, Device_SN, TxPacketOffset,0,4);
			TxPacketOffset+=4;
			TxBuffer[TxPacketOffset++] = *data;
		  TxBuffer[TxPacketOffset++] = ack;
		 	TxBuffer[TxPacketOffset++] = CHECK_SUM;
			break;
		case TEST_PKT:
			TxBuffer[TxPacketOffset++] = (uint8_t)TEST_PKT_LEN>>8;
		  TxBuffer[TxPacketOffset++] = (uint8_t)TEST_PKT_LEN;
			TxBuffer[TxPacketOffset++] = TEST_PKT;
			BufferOffsetCpy(TxBuffer, Device_SN, TxPacketOffset,0,4);
			TxPacketOffset+=4;
			TxBuffer[TxPacketOffset++] = ack;
			TxBuffer[TxPacketOffset++] = CHECK_SUM;
			break;
		case LOGIN_REQUEST_PKT:
			break;
		case SENSOR_INC_DATA_PKT:
			TxBuffer[TxPacketOffset++]= (uint8_t)SENSOR_INC_DATA_PKT_LEN>>8;
			TxBuffer[TxPacketOffset++]= (uint8_t)SENSOR_INC_DATA_PKT_LEN;
			TxBuffer[TxPacketOffset++]= SENSOR_INC_DATA_PKT;
			BufferOffsetCpy(TxBuffer, Device_SN, TxPacketOffset,0,4);
			TxPacketOffset+=4;
		
			BufferOffsetCpy(TxBuffer, data, TxPacketOffset,0,SENSOR_INC_DATA_SIZE);
			TxPacketOffset += SENSOR_INC_DATA_SIZE;
			
			TxBuffer[TxPacketOffset++]=	ack;
			TxBuffer[TxPacketOffset++]= 0x0D;
			break;
		case SENSOR_ACC_DATA_PKT:
//			TxBuffer[TxPacketOffset++]= (uint8_t)SENSOR_DATA_PKT_LENGTH>>8;
//			TxBuffer[TxPacketOffset++]= (uint8_t)SENSOR_DATA_PKT_LENGTH;
//			TxBuffer[TxPacketOffset++]= SENSOR_INC_DATA_PKT;
//			BufferOffsetCpy(TxBuffer, Device_SN, TxPacketOffset,0,4);
//			TxPacketOffset+=4;
//			BufferOffsetCpy(TxBuffer, Device_Date_Time, TxPacketOffset,0,7);
//			TxPacketOffset+=7;
//			
//			TxBuffer[TxPacketOffset++]=	(uint8_t)TDS_In_Value>>8;
//			TxBuffer[TxPacketOffset++]=	(uint8_t)TDS_In_Value;
//			TxBuffer[TxPacketOffset++]=	(uint8_t)TDS_Out_Value>>8;
//			TxBuffer[TxPacketOffset++]=	(uint8_t)TDS_Out_Value;
//			
//			TxBuffer[TxPacketOffset++]=	(uint8_t)Flow_In_Value>>24;
//			TxBuffer[TxPacketOffset++]=	(uint8_t)Flow_Value>>16;
//			TxBuffer[TxPacketOffset++]=	(uint8_t)Flow_Value>>8;
//			TxBuffer[TxPacketOffset++]=	(uint8_t)Flow_Value;		

//			TxBuffer[TxPacketOffset++]=	Temper_Value;
//			
//			TxBuffer[TxPacketOffset++]=	NO_ACK;
//			TxBuffer[TxPacketOffset++]= 0x0D;
			break;			
					
		case TIME_REQUEST_PKT:
			TxBuffer[TxPacketOffset++] = (uint8_t)TIME_REQUEST_PKT_LEN>>8;
		  TxBuffer[TxPacketOffset++] = (uint8_t)TIME_REQUEST_PKT_LEN;
			TxBuffer[TxPacketOffset++] = TIME_REQUEST_PKT;
			BufferOffsetCpy(TxBuffer, Device_SN, TxPacketOffset,0,4);
			TxPacketOffset+=4;
			TxBuffer[TxPacketOffset++] = ack;
			TxBuffer[TxPacketOffset++] = CHECK_SUM;
		
			break;
			
		case FW_REQUEST_PKT:
			break;
		
		default:
			printf("Unknown Packet to be sent, Dropped!\r\n");
			break;	
	}
	
	return WIFI_Send_Bytes();
}

uint8_t WIFI_Send_Bytes(void)
{
	uint8_t index =0, LengthToSend = (TxBuffer[0]<<8) + TxBuffer[1];
	
#ifdef USING_UNVARNISHED

#else	
//	if(len>9)
//		cLen[index++]=len-0x30;
//		
//	cLen[index++]=len%10-0x30;
//	cLen[index++]='\r';
//	cLen[index++]='\n';
//	
//	// send data lenth
//	HAL_UART_Send_String(WIFI_COM,strsta("AT+CIPSEND=",cLen));
//	

//  HAL_Systick_Delay(10);
//	if(strstr(ATBuffer,"OK"))
//	{
//		CounterUpdateFlag =1;
//		BufferReset(ATBuffer, BUFFER_SIZE);
//	}
//	else if(strstr(ATBuffer,"ERROR"))
//	{
//		CounterUpdateFlag =1;
//		BufferReset(ATBuffer, BUFFER_SIZE);
//		return ERROR;
//	}
#endif	
	while(LengthToSend--){
		HAL_UART_Send_Byte(WIFI_COM, TxBuffer[index++]);
	}
	return SUCCESS;
}



uint8_t WIFI_Receive_Packet(Packet_TypeDef* pkt, uint32_t*timeout)
{
	if(timeout==NULL)
	{
		HAL_Systick_Delay(10);

		if(pktLength==0 ||ReceiveState != 0xFF)
			return ERROR;
		else{
			pkt->type = RxBuffer[2]; 
			BufferOffsetCpy(pkt->data,RxBuffer,0,3,(pktLength-2));
			
			CounterUpdateFlag = 1;
			ReceiveState=0;
			return SUCCESS;
		}
	}
	else
	{
	 while(*timeout>0)
	 {
	  if(ReceiveState == 0xFF)
		{
			printf("ReceivePacket timeout value is %d\r\n", *timeout);
			pkt->length = (RxBuffer[0]<<8)+RxBuffer[1];
			pkt->type = RxBuffer[2]; 
			BufferOffsetCpy(pkt->data,RxBuffer,0,3,(pkt->length-3));
			
			CounterUpdateFlag = 1;
			ReceiveState=0;
			return SUCCESS;
		}
		
	 }
	printf("ReceivePacket timeout\r\n");
	return ERROR;
	}

}
