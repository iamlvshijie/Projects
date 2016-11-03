#include "stm32f0xx.h"
#include "packet.h"
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ESP8266_H
#define __ESP8266_H

#ifdef __cplusplus
 extern "C" {
#endif
#define USING_DNS

#define USING_AIRKISS
//#define USING_UDP
#define USING_TCP
#define USING_UNVARNISHED


#define USART_MAX_INDEX   12
	 
	 
#define BUFFER_SIZE       1035	 
	 
	 
#define WIFI_MAX_TRIES      3
typedef enum {
								CONNECTED 		= 0x33, 
								AP_JOINED			=	0x32, 
								NO_AP					=	0x35,
								OTHER 				= 0x30,
								FIRST_CONFIG  = 0x31
							} WIFIStatus;    

#define WIFI_POWERED_BIT     0x01
#define AP_JOINED_BIT        0x02
#define SERVER_CONNECT_BIT   0x04
#define DATA_SEND_BIT        0x08		
							
	 
uint8_t  	WIFI_Check_Status(void);
uint8_t 	WIFI_Config(void);
uint8_t  	WIFI_Join_AP(uint32_t* timeout);
uint8_t 	WIFI_Quit_Unvarnished(void);
uint8_t 	WIFI_Reset_Unvarnished(void);
uint8_t 	WIFI_Restore(void);
							
uint8_t 	WIFI_Quit_Server(void);
uint8_t 	WIFI_Connect_Server(void);
uint8_t  	WIFI_Set_Unvarnished(void);	
							
uint8_t 	WIFI_Receive_Packet(Packet_TypeDef* pkt, uint32_t*timeout);
uint8_t 	WIFI_Send_Packet(uint8_t type, uint8_t* data, uint8_t ack);
uint8_t 	WIFI_Send_Bytes(void);
							

	 
/**
  * @}
  */
  
#ifdef __cplusplus
}
#endif

#endif /* __ESP8266_H */	 
