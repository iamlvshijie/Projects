/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __EE_MEMORY_H
#define __EE_MEMORY_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"
#include "i2c_ee.h"
	 

/** @addtogroup Utilities
  * @{
  */



/* Exported types ------------------------------------------------------------*/
	 
/* Exported constants --------------------------------------------------------*/
#define INFO_ADDRESS 					0x00
#define INFO_SIZE    					128	 
	 
#define DSN_ADDRESS 					0x00
#define DSN_SIZE    					4

#define PID_ADDRESS 					0x04
#define PID_SIZE	  					2

#define HW_REV_ADDRESS   			0x06
#define HW_REV_SIZE  					1

#define FW_REV_ADDRESS 				0x07
#define FW_REV_SIZE    				2

#define CTL_REG_ADDRESS 			0x09
#define CTL_REG_SIZE   				2

#define CTRL_REG0             0

#define FUF_BIT               0x40
#define ODS_BIT								0x20

#define CTRL_REG1             1
#define TCPS_BIT              0x80
#define UDP_BIT 							0x40
#define SVIP_BIT              0x20
#define SSID_BIT0			        0x10

#define SERVER_IP_ADDRESS 		0x0B
#define SERVER_IP_SIZE    		4

#define SERVER_PORT_ADDRESS 	0x0F
#define SERVER_PORT_SIZE    	2

#define SSID_ADDRESS     			0x11
#define SSID_SIZE 			 			32

#define PSWD_ADDRESS     			0x31
#define PSWD_SIZE        			64

#define WB_INDEX_ADDRESS			0x7E
#define WB_INDEX_SIZE     		1
#define RB_INDEX_ADDRESS			0x7F
#define RB_INDEX_SIZE     		1

#define EE_CAPACITY_SIZE 			64*1024/8
#define BLOCK_START_ADDRESS 	0x80
#define BLOCK_SIZE   					16
#define BLOCK_NUM 	 					10//256//504 //(EE_CAPACITY_SIZE-INFO_SIZE)/16

	 
	 
/** @defgroup 
  * @{
  */ 
uint8_t Memory_Init(void);
uint8_t Memory_ReadInfo(void);
uint8_t Memory_Read_Contrl_Reg(uint8_t regNum, uint8_t bit);
uint8_t Memory_Write_Contrl_Reg(uint8_t regNum, uint8_t bit, FlagStatus s);
uint8_t Memory_Read_Buffer(uint8_t* buf, uint8_t addr, uint8_t len);
uint8_t Memory_Write_Buffer(uint8_t* buf, uint8_t addr, uint8_t len);
uint8_t Memory_Write_Block(uint8_t* bData);
uint8_t Memory_Read_Block(uint8_t* bData);

	 
/**
  * @}
  */
  
#ifdef __cplusplus
}
#endif

#endif /* __EE_MEMORY_H */
/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */

/**
  * @}
  */  

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
