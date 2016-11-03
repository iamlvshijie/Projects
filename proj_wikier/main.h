/**
  ******************************************************************************
  * @file    Project/STM32F0xx_StdPeriph_Templates/main.h 
  * @author  MCD Application Team
  * @version V1.5.0
  * @date    05-December-2014
  * @brief   Header for main.c module
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
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"




/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/



/* Device Key Info */
#define IWB_EEPROM_ADDR   				0x0800FC00
#define IWB_EEPROM_SIZE   				0x0400

#define IWB_HW_VER_ADDR_OFFSET   	0x0000
#define IWB_FW_VER_ADDR_OFFSET    0x0002
#define IWB_FW_UPDATE_FLAG_OFFSET 0x0004

#define IWB_WIFI_SSID_OFFSET      0x0006
#define IWB_WIFI_PSWD_PSWD_OFFSET 0x000A

/* System status */
#define HW_INIT_SUCCESS  				0x01
#define LSE_RTC_SUCCESS					0x02
#define INFO_READ_SUCCESS 			0x04
#define WIFI_CONFIG_SUCCESS 		0x08
#define AP_JOIN_SUCCESS 		 	  0x10
#define SERVER_CONNECT_SUCCESS 	0x20
#define UNVARNISH_SET_SUCCESS 	0x40
#define TIME_REQUEST_SUCCESS    0x80

#define DATA_STORE_OFFLINE    0x0100




#define WATER_LEAK_FOUND      0x8000



/* APP State Machine Definitions */
#define SM_INIT_HW    					0x00
#define SM_WIFI_CONFIG					0x01
#define SM_POLL_EVENT						0x02
#define SM_SW_RESET							0x03
#define SM_SET_UPDATEFLAG				0x04
#define SM_COLLECT_DATA					0x05
#define SM_STORE_DATA						0x06
#define SM_SEND_DATA						0x07
#define SM_TEST_SERVER        	0x08
#define SM_CONNECT_SERVER      	0x09
        
#define SM_JOIN_AP							0x10
#define SM_SET_TIME  						0x11
//#define SM_UPDATE_TIME         	0x12
#define SM_SET_UNVARNISHED     	0x13
#define SM_LOGIN_SERVER        	0x14
#define SM_CHECK_WIFI          	0x15
#define SM_RESET_WIFI          	0x16
#define SM_RESTORE_WIFI        	0x17
#define SM_REPOWER_WIFI        	0x18
#define SM_SEND_TEST						0x19
       
#define SM_CHECK_STATUS        	0x20
#define SM_GET_TIME 						0x21
#define SM_READ_INFO           	0x22
#define SM_WRITE_INFO						0x23
#define SM_REPAIR_WIFI         	0x24
#define SM_REQUEST_TIME   			0x25
#define SM_RESEND_DATA         	0x26
#define SM_READ_TEMP           	0x27
        
#define SM_CHECK_UPDATE					0x3E
#define SM_SYSTEM_FAILED       	0x3F


		
							
							
/* Exported functions ------------------------------------------------------- */
						
#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
