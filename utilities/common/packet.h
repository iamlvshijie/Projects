/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PACKET_H
#define __PACKET_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"
	 


/** @addtogroup Utilities
  * @{
  */

typedef struct 
{
	uint16_t 	length;
	uint8_t 	type;
	__IO uint8_t 	data[20];
	uint16_t  offset;
	uint8_t  	ack_flag;
	uint8_t  	sum;
}Packet_TypeDef;

/*         */
#define PACKET_HEADER           (2)
#define PACKET_TRAILER          (0)
#define PACKET_OVERHEAD         (PACKET_HEADER + PACKET_TRAILER)

#define APP_PKT_MAX_LENGTH 			10

#define PKT_MAX_LENGTH  				1031
#define PKT_TYPE_TOTAL  				9


#define NEED_ACK 								0xFF
#define NO_ACK 									0x00

#define CHECK_SUM       				0x0D

#define UNKOWN_PKT     					0x00

#define RESPONSE_PKT    				0x01
#define RESPONSE_PKT_LEN		    8
 #define RESPONSE_DATA_SIZE     1
	#define OK 										0x01
	#define LOW_REV 							0x55
	#define INVALID_PKT						0xFD
	#define INVALID_DEV   				0xFE

#define TEST_PKT	      				0x02
#define TEST_PKT_LEN	  				7
	#define TEST_DATA_SIZE        0
	
#define LOGIN_REQUEST_PKT 			0x03

#define QUIT_REQUEST_PKT 				0x04

#define SENSOR_INC_DATA_PKT   	0x05
#define SENSOR_INC_DATA_PKT_LEN 19
#define SENSOR_INC_DATA_SIZE    12
	#define TDS_DATA      				0x10
	#define FLOW_DATA     				0x11
	#define TEMP_DATA 						0x12
	
#define SENSOR_ACC_DATA_PKT 		0x06
	#define SENSOR_ACC_DATA_SIZE  15
	

#define SERVER_CMD_PKT      		0x07
	#define SERVER_CMD_DATA_SIZE  1
	#define CMD_OFFSET            0
	#define CMD_CHECK_STATUS     	0x01
	#define CMD_BUZZER_ALARM  		0x02
	#define CMD_CLEAR_FLOW        0x03

#define DEVICE_STATUS_PKT 			0x08
#define DEVICE_STATUS_PKT_LEN 	13
#define DEVICE_STATUS_DATA_SIZE 6

#define TIME_REQUEST_PKT 				0x09
#define TIME_REQUEST_PKT_LEN 		7
#define TIME_REQUEST_DATA_SIZE  0

#define TIME_DATA_PKT 					0x0A
#define TIME_DATA_PKT_LEN 			10
#define TIME_DATA_SIZE        	3
	#define TIME_YEAR_OFFSET 			0
	#define TIME_MONTH_OFFSET 		1
	#define TIME_DATE_OFFSET  		2
	#define TIME_HOUR_OFFSET  		3
	#define TIME_MINUTE_OFFSET 		4
	#define TIME_SECOND_OFFSET 		5
	#define TIME_WEEKDAY_OFFSET 	6
                                


#define FW_REQUEST_PKT      		0xF0
#define FW_REQUEST_PKT_LEN 			12
	

#define FW_PREAMBLE_PKT     		0xF1
	#define FW_TOTAL_NUM_OFFSET 	0x08
	#define FW_PAD_NUM_OFFSET   	0x0A
	
#define FW_DATA_PKT         		0xF2
	#define FW_DATA_OFFSET    		0x05
	#define FW_DATA_LEN       		1024
	
#define FW_RESPONSE_PKT     		0xF3
#define FW_RESPONSE_PKT_LEN     0x06
	#define RESPONSE_OK       		0x00
	#define HW_REV_ERROR   				0xF0
	#define FW_REV_SAME    				0xF1
	#define FW_PACKET_INDEX_ADDR 	0x03
	
#define FW_SUCCESS_PKT      		0xF4
#define FW_SUCCESS_PKT_LEN      0x03


	
	
uint8_t Packet_Parser(uint8_t* pkt, uint16_t len, uint8_t* data);
uint8_t Packet_Create(uint8_t type, uint8_t* data);


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
	 
	 
/** @defgroup 
  * @{
  */ 

	 
/**
  * @}
  */
  
#ifdef __cplusplus
}
#endif

#endif /* __PACKET_H */


/************************ (C) COPYRIGHT I2T Team *****END OF FILE****/


