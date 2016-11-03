/**
  ******************************************************************************
  * @file    bsp.h
  * @author  I2T Team
  * @version V1.0.0
  * @date    16-January-2014
  * @brief   This file contains definitions for hardware resources of PSS Project.
  ******************************************************************************
  * @note      
  *
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_H
#define __BSP_H

#ifdef __cplusplus
 extern "C" {
#endif

#ifdef STM32F030	 
	#define STM32F0
#endif	 
/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"


/** @addtogroup Utilities
  * @{
  */



	 
/* Exported types ------------------------------------------------------------*/
#define IOn                              	7
	 
typedef enum 
{
  IO0 = 0,
  IO1 = 1,
	IO2 = 2,
	IO3 = 3,
	IO4 = 4,
	IO5 = 5,
	IO6 = 6,
	IO7 = 7
} IO_TypeDef;




typedef enum 
{
  BUTTON_TAMPER = 0,
  BUTTON_RIGHT = 1,
  BUTTON_LEFT = 2,
  BUTTON_UP = 3,
  BUTTON_DOWN = 4,
  BUTTON_SEL = 5
} Button_TypeDef;

typedef enum 
{  
  BUTTON_MODE_GPIO = 0,
  BUTTON_MODE_EXTI = 1
} ButtonMode_TypeDef;

typedef enum 
{ 
  JOY_NONE = 0,
  JOY_SEL = 1,
  JOY_DOWN = 2,
  JOY_LEFT = 3,
  JOY_RIGHT = 4,
  JOY_UP = 5
} JOYState_TypeDef
;

typedef enum 
{
  COM1 = 0,
  COM2 = 1
} COM_TypeDef;   

/* Exported constants --------------------------------------------------------*/
/** @defgroup STM32072B_EVAL_LOW_LEVEL_Exported_Constants
  * @{
  */ 



/** @addtogroup IO
  * @{
  */


#define LEDG_IO														IO0
#define IO0_PIN                         	GPIO_Pin_4
#define IO0_GPIO_PORT                   	GPIOA
#define IO0_GPIO_CLK                    	RCC_AHBPeriph_GPIOA

#define LEDR_IO

#define HX711_0_SCK_IO									  IO1
#define IO1_PIN                         	GPIO_Pin_9
#define IO1_GPIO_PORT                   	GPIOA
#define IO1_GPIO_CLK                    	RCC_AHBPeriph_GPIOA

#define HX711_0_DOUT_IO                   IO2
#define IO2_PIN                   				GPIO_Pin_10
#define IO2_GPIO_PORT            					GPIOA
#define IO2_GPIO_CLK               	 			RCC_AHBPeriph_GPIOA


#define IO3_PIN														GPIO_Pin_4
#define IO3_GPIO_PORT											GPIOA
#define IO3_GPIO_CLK											RCC_AHBPeriph_GPIOA

#define IO4_PIN											 			GPIO_Pin_1
#define IO4_GPIO_PORT											GPIOB
#define IO4_GPIO_CLK							  			RCC_AHBPeriph_GPIOB


#define IO5_PIN                  					GPIO_Pin_0
#define IO5_GPIO_PORT            					GPIOA
#define IO5_GPIO_CLK             					RCC_AHBPeriph_GPIOA


#define IO6_PIN                  					GPIO_Pin_3
#define IO6_GPIO_PORT            					GPIOA
#define IO6_GPIO_CLK             					RCC_AHBPeriph_GPIOA


#define IO7_PIN                  					GPIO_Pin_5
#define IO7_GPIO_PORT            					GPIOA
#define IO7_GPIO_CLK             					RCC_AHBPeriph_GPIOA

/**
  * @}
  */ 

/** @addtogroup STM32072B_EVAL_LOW_LEVEL_BUTTON
  * @{
  */  
#define BUTTONn                          6

/**
 * @brief Tamper push-button
 */
#define TAMPER_BUTTON_PIN                GPIO_Pin_13
#define TAMPER_BUTTON_GPIO_PORT          GPIOC
#define TAMPER_BUTTON_GPIO_CLK           RCC_AHBPeriph_GPIOC
#define TAMPER_BUTTON_EXTI_LINE          EXTI_Line13
#define TAMPER_BUTTON_EXTI_PORT_SOURCE   EXTI_PortSourceGPIOC
#define TAMPER_BUTTON_EXTI_PIN_SOURCE    EXTI_PinSource13
#define TAMPER_BUTTON_EXTI_IRQn          EXTI4_15_IRQn 

/**
 * @brief Joystick Right push-button
 */
#define RIGHT_BUTTON_PIN                 GPIO_Pin_3
#define RIGHT_BUTTON_GPIO_PORT           GPIOE
#define RIGHT_BUTTON_GPIO_CLK            RCC_AHBPeriph_GPIOE
#define RIGHT_BUTTON_EXTI_LINE           EXTI_Line3
#define RIGHT_BUTTON_EXTI_PORT_SOURCE    EXTI_PortSourceGPIOE
#define RIGHT_BUTTON_EXTI_PIN_SOURCE     EXTI_PinSource3
#define RIGHT_BUTTON_EXTI_IRQn           EXTI2_3_IRQn

/**
 * @brief Joystick Left push-button
 */
#define LEFT_BUTTON_PIN                  GPIO_Pin_2
#define LEFT_BUTTON_GPIO_PORT            GPIOE
#define LEFT_BUTTON_GPIO_CLK             RCC_AHBPeriph_GPIOE
#define LEFT_BUTTON_EXTI_LINE            EXTI_Line2
#define LEFT_BUTTON_EXTI_PORT_SOURCE     EXTI_PortSourceGPIOE
#define LEFT_BUTTON_EXTI_PIN_SOURCE      EXTI_PinSource2
#define LEFT_BUTTON_EXTI_IRQn            EXTI2_3_IRQn  

/**
 * @brief Joystick Up push-button
 */
#define UP_BUTTON_PIN                    GPIO_Pin_9
#define UP_BUTTON_GPIO_PORT              GPIOF
#define UP_BUTTON_GPIO_CLK               RCC_AHBPeriph_GPIOF
#define UP_BUTTON_EXTI_LINE              EXTI_Line9
#define UP_BUTTON_EXTI_PORT_SOURCE       EXTI_PortSourceGPIOF
#define UP_BUTTON_EXTI_PIN_SOURCE        EXTI_PinSource9
#define UP_BUTTON_EXTI_IRQn              EXTI4_15_IRQn  

/**
 * @brief Joystick Down push-button
 */  
#define DOWN_BUTTON_PIN                  GPIO_Pin_10
#define DOWN_BUTTON_GPIO_PORT            GPIOF
#define DOWN_BUTTON_GPIO_CLK             RCC_AHBPeriph_GPIOF
#define DOWN_BUTTON_EXTI_LINE            EXTI_Line10
#define DOWN_BUTTON_EXTI_PORT_SOURCE     EXTI_PortSourceGPIOF
#define DOWN_BUTTON_EXTI_PIN_SOURCE      EXTI_PinSource10
#define DOWN_BUTTON_EXTI_IRQn            EXTI4_15_IRQn  

/**
 * @brief Joystick Sel push-button
 */
#define SEL_BUTTON_PIN                   GPIO_Pin_0
#define SEL_BUTTON_GPIO_PORT             GPIOA
#define SEL_BUTTON_GPIO_CLK              RCC_AHBPeriph_GPIOA
#define SEL_BUTTON_EXTI_LINE             EXTI_Line0
#define SEL_BUTTON_EXTI_PORT_SOURCE      EXTI_PortSourceGPIOA
#define SEL_BUTTON_EXTI_PIN_SOURCE       EXTI_PinSource0
#define SEL_BUTTON_EXTI_IRQn             EXTI0_1_IRQn 

/**
  * @}
  */ 


/** @addtogroup STM32072B_EVAL_LOW_LEVEL_COM
  * @{
  */
#define COMn                             2

/**
 * @brief Definition  COM for EVAL
 */ 

#define COM1_USART                  USART2
#define COM1_CLK                    RCC_APB1Periph_USART2
                                    
#define COM1_TX_PIN                 GPIO_Pin_2
#define COM1_TX_GPIO_PORT           GPIOA
#define COM1_TX_GPIO_CLK            RCC_AHBPeriph_GPIOA
#define COM1_TX_SOURCE              GPIO_PinSource2
#define COM1_TX_AF                  GPIO_AF_1
                                    
#define COM1_RX_PIN                 GPIO_Pin_3
#define COM1_RX_GPIO_PORT           GPIOA
#define COM1_RX_GPIO_CLK            RCC_AHBPeriph_GPIOA
#define COM1_RX_SOURCE              GPIO_PinSource3
#define COM1_RX_AF                 	GPIO_AF_1
                                 
// #define COM1_CTS_PIN                GPIO_Pin_11
// #define COM1_CTS_GPIO_PORT          GPIOA
// #define COM1_CTS_GPIO_CLK           RCC_AHBPeriph_GPIOA
// #define COM1_CTS_SOURCE             GPIO_PinSource11
// #define COM1_CTS_AF                 GPIO_AF_1
                                    
// #define COM1_RTS_PIN                GPIO_Pin_12
// #define COM1_RTS_GPIO_PORT          GPIOA
// #define COM1_RTS_GPIO_CLK           RCC_AHBPeriph_GPIOA
// #define COM1_RTS_SOURCE             GPIO_PinSource12
// #define COM1_RTS_AF                 GPIO_AF_1

#define WIFI_COM_IRQn 									USART2_IRQn                                            



/**
 * @brief Definition  COM port for DEBUG
 */ 
#define DEBUG_COM										COM2
#define COM2_USART                  USART1
#define COM2_CLK                    RCC_APB2Periph_USART1
                                   
#define COM2_TX_PIN                 GPIO_Pin_2
#define COM2_TX_GPIO_PORT           GPIOA
#define COM2_TX_GPIO_CLK            RCC_AHBPeriph_GPIOA
#define COM2_TX_SOURCE              GPIO_PinSource2
#define COM2_TX_AF                  GPIO_AF_1
                                   
#define COM2_RX_PIN                 GPIO_Pin_3
#define COM2_RX_GPIO_PORT           GPIOA
#define COM2_RX_GPIO_CLK            RCC_AHBPeriph_GPIOA
#define COM2_RX_SOURCE              GPIO_PinSource3
#define COM2_RX_AF                  GPIO_AF_1
                                         
// #define COM2_CTS_PIN                GPIO_Pin_11
// #define COM2_CTS_GPIO_PORT          GPIOA
// #define COM2_CTS_GPIO_CLK           RCC_AHBPeriph_GPIOA
// #define COM2_CTS_SOURCE             GPIO_PinSource11
// #define COM2_CTS_AF                 GPIO_AF_1
                                   
// #define COM2_RTS_PIN                GPIO_Pin_12
// #define COM2_RTS_GPIO_PORT          GPIOA
// #define COM2_RTS_GPIO_CLK           RCC_AHBPeriph_GPIOA
// #define COM2_RTS_SOURCE             GPIO_PinSource12
// #define COM2_RTS_AF                 GPIO_AF_1
                                         
#define COM2_IRQn                   USART1_IRQn


/**
  * @}
  */
	
	
	
void BSP_IO_Init(IO_TypeDef io);
void BSP_IO_High(IO_TypeDef io);
void BSP_IO_Low(IO_TypeDef io);
void BSP_IO_Toggle(IO_TypeDef io);

void BSP_IO_All_DeInit(void);


void IO_Init(IO_TypeDef io);
void IO_On(IO_TypeDef io);
void IO_Off(IO_TypeDef io);
void IO_Toggle(IO_TypeDef io);

void IO_All_DeInit(void);


void PB_Init(Button_TypeDef Button, ButtonMode_TypeDef Button_Mode);
uint32_t PB_GetState(Button_TypeDef Button);

uint32_t BSP_UART_DeInit(COM_TypeDef COM);
uint32_t BSP_UART_Config(COM_TypeDef COM);

void COM_DeInit(COM_TypeDef COM);
void COM_Init(COM_TypeDef COM, USART_InitTypeDef* USART_InitStruct);

void COM_SendChar(COM_TypeDef COM, uint8_t ch);
void COM_SendString(COM_TypeDef COM, uint8_t *str);
void COM_ITConfig(COM_TypeDef COM);
//void Send_Hex(unsigned char dat);

/**
  * @brief  I2C EEPROM Interface pins
  */  
#define BSP_I2C                          I2C1
#define BSP_I2C_CLK                      RCC_APB1Periph_I2C1
#define BSP_I2C_TIMING            			 0x00210507

#define BSP_I2C_SCL_PIN                  GPIO_Pin_8                  
#define BSP_I2C_SCL_GPIO_PORT            GPIOB                      
#define BSP_I2C_SCL_GPIO_CLK             RCC_AHBPeriph_GPIOB
#define BSP_I2C_SCL_SOURCE               GPIO_PinSource8
#define BSP_I2C_SCL_AF                   GPIO_AF_1

#define BSP_I2C_SDA_PIN                  GPIO_Pin_9                  
#define BSP_I2C_SDA_GPIO_PORT            GPIOB                       
#define BSP_I2C_SDA_GPIO_CLK             RCC_AHBPeriph_GPIOB
#define BSP_I2C_SDA_SOURCE               GPIO_PinSource9
#define BSP_I2C_SDA_AF                   GPIO_AF_1

//void I2C_Deconfig(void);
void BSP_I2C_Config(void); 


#define NRF24L01_SPI                     SPI1
#define SPIx_CLK                         RCC_APB2Periph_SPI1
#define SPIx_IRQn                        SPI1_IRQn
#define SPIx_IRQHandler                  SPI1_IRQHandler

#define SPIx_SCK_PIN                     GPIO_Pin_5
#define SPIx_SCK_GPIO_PORT               GPIOA
#define SPIx_SCK_GPIO_CLK                RCC_AHBPeriph_GPIOA
#define SPIx_SCK_SOURCE                  GPIO_PinSource5
#define SPIx_SCK_AF                      GPIO_AF_0

#define SPIx_MISO_PIN                    GPIO_Pin_6
#define SPIx_MISO_GPIO_PORT              GPIOA
#define SPIx_MISO_GPIO_CLK               RCC_AHBPeriph_GPIOA
#define SPIx_MISO_SOURCE                 GPIO_PinSource6
#define SPIx_MISO_AF                     GPIO_AF_0

#define SPIx_MOSI_PIN                    GPIO_Pin_7
#define SPIx_MOSI_GPIO_PORT              GPIOA
#define SPIx_MOSI_GPIO_CLK               RCC_AHBPeriph_GPIOA
#define SPIx_MOSI_SOURCE                 GPIO_PinSource7
#define SPIx_MOSI_AF                     GPIO_AF_0

#define SPI_DATASIZE                     SPI_DataSize_8b
#define SPI_DATAMASK                     (uint8_t)0xFF

// CE Pin & CSN Pin & IRQ Pin


#define NRF24L01_CSN_IO									IO3
#define NRF24L01_CSN_PIN 								GPIO_Pin_4
#define NRF24L01_CSN_GPIO_PORT     			GPIOA
#define NRF24L01_CSN_GPIO_CLK						RCC_AHBPeriph_GPIOA
#define NRF24L01_CSN_SOURCE							GPIO_PinSource4

#define NRF24L01_CE_IO 									IO4
#define NRF24L01_CE_PIN 								GPIO_Pin_1
#define NRF24L01_CE_GPIO_PORT     			GPIOB
#define NRF24L01_CE_GPIO_CLK						RCC_AHBPeriph_GPIOB
#define NRF24L01_CE_SOURCE							GPIO_PinSource1


#define NRF24L01_IRQ_IO 								IO5
#define NRF24L01_IRQ_PIN 								GPIO_Pin_0
#define NRF24L01_IRQ_GPIO_PORT     			GPIOA
#define NRF24L01_IRQ_GPIO_CLK						RCC_AHBPeriph_GPIOA
#define NRF24L01_IRQ_SOURCE							GPIO_PinSource0


#define CE(x)					x ? GPIO_SetBits(GPIOB,GPIO_Pin_1) : GPIO_ResetBits(GPIOB,GPIO_Pin_1);
#define CSN(x)					x ? GPIO_SetBits(GPIOA,GPIO_Pin_4) : GPIO_ResetBits(GPIOA,GPIO_Pin_4);
#define IRQ					GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)

/**
  * @}
  */
  
#ifdef __cplusplus
}
#endif

#endif /* __STM32072B_EVAL_H */
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
