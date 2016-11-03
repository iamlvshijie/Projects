/**
  ******************************************************************************
  * @file    stm32072b_eval.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    16-January-2014
  * @brief   This file provides firmware functions to manage Leds, push-buttons, 
  *          COM ports, SD card on SPI and temperature sensor (LM75) available on 
  *          STM32072B-EVAL evaluation board from STMicroelectronics.
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
#include "bsp.h"

/** @addtogroup Utilities
  * @{
  */ 

/** @addtogroup STM32_EVAL
  * @{
  */ 

/** @addtogroup STM32072B_EVAL
  * @{
  */   
    
/** @defgroup STM32072B_EVAL_LOW_LEVEL 
  * @brief This file provides firmware functions to manage Leds, push-buttons, 
  *        COM ports, SD card on SPI and temperature sensor (LM75) available on 
  *        STM32072B-EVAL evaluation board from STMicroelectronics.
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/


GPIO_TypeDef* BUTTON_PORT[BUTTONn] = {TAMPER_BUTTON_GPIO_PORT, 
																			RIGHT_BUTTON_GPIO_PORT, 
                                      LEFT_BUTTON_GPIO_PORT, 
																			UP_BUTTON_GPIO_PORT,
                                      DOWN_BUTTON_GPIO_PORT,
																			SEL_BUTTON_GPIO_PORT}; 

const uint16_t BUTTON_PIN[BUTTONn] = {TAMPER_BUTTON_PIN, RIGHT_BUTTON_PIN, 
                                      LEFT_BUTTON_PIN, UP_BUTTON_PIN, 
                                      DOWN_BUTTON_PIN, SEL_BUTTON_PIN}; 

const uint32_t BUTTON_CLK[BUTTONn] = {TAMPER_BUTTON_GPIO_CLK, RIGHT_BUTTON_GPIO_CLK, 
                                      LEFT_BUTTON_GPIO_CLK, UP_BUTTON_GPIO_CLK, 
                                      DOWN_BUTTON_GPIO_CLK, SEL_BUTTON_GPIO_CLK};

const uint16_t BUTTON_EXTI_LINE[BUTTONn] = {TAMPER_BUTTON_EXTI_LINE,
                                            RIGHT_BUTTON_EXTI_LINE,
                                            LEFT_BUTTON_EXTI_LINE,
                                            UP_BUTTON_EXTI_LINE,
                                            DOWN_BUTTON_EXTI_LINE,
                                            SEL_BUTTON_EXTI_LINE};

const uint16_t BUTTON_PORT_SOURCE[BUTTONn] = {TAMPER_BUTTON_EXTI_PORT_SOURCE,
                                              RIGHT_BUTTON_EXTI_PORT_SOURCE,
                                              LEFT_BUTTON_EXTI_PORT_SOURCE,
                                              UP_BUTTON_EXTI_PORT_SOURCE,
                                              DOWN_BUTTON_EXTI_PORT_SOURCE,
                                              SEL_BUTTON_EXTI_PORT_SOURCE};
								 
const uint16_t BUTTON_PIN_SOURCE[BUTTONn] = {TAMPER_BUTTON_EXTI_PIN_SOURCE,
                                             RIGHT_BUTTON_EXTI_PIN_SOURCE,
                                             LEFT_BUTTON_EXTI_PIN_SOURCE,
                                             UP_BUTTON_EXTI_PIN_SOURCE,
                                             DOWN_BUTTON_EXTI_PIN_SOURCE,
                                             SEL_BUTTON_EXTI_PIN_SOURCE}; 

const uint16_t BUTTON_IRQn[BUTTONn] = {TAMPER_BUTTON_EXTI_IRQn, RIGHT_BUTTON_EXTI_IRQn, 
                                       LEFT_BUTTON_EXTI_IRQn, UP_BUTTON_EXTI_IRQn, 
                                       DOWN_BUTTON_EXTI_IRQn, SEL_BUTTON_EXTI_IRQn};

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/ 

/** @defgroup STM32072B_EVAL_LOW_LEVEL_Private_Functions
  * @{
  */ 

///**
//  * @brief  Toggles the selected LED.
//  * @param  Led: Specifies the Led to be toggled. 
//  *          This parameter can be one of following parameters:
//  *            @arg LED1
//  *            @arg LED2
//  *            @arg LED3
//  *            @arg LED4  
//  * @retval None
//  */
//void BSP_IO_Init(IO_TypeDef io)
//{
//  GPIO_InitTypeDef  GPIO_InitStructure;
//  
//  /* Enable the GPIO_LED Clock */
//  RCC_AHBPeriphClockCmd(GPIO_CLK[io], ENABLE);

//  /* Configure the GPIO_LED pin */
//  GPIO_InitStructure.GPIO_Pin = GPIO_PIN[io];
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_Init(GPIO_PORT[io], &GPIO_InitStructure);

//}

///**
//  * @brief  Turns selected LED On.
//  * @param  io: Specifies the Led to be set on. 
//  *          This parameter can be one of following parameters:
//  *            @arg LED1
//  *            @arg LED2
//  *            @arg LED3
//  *            @arg LED4  
//  * @retval None
//  */
//void BSP_IO_High(IO_TypeDef io)
//{
//  GPIO_PORT[io]->BSRR = GPIO_PIN[io];
//}

///**
//  * @brief  Turns selected LED Off.
//  * @param  Led: Specifies the Led to be set off. 
//  *          This parameter can be one of following parameters:
//  *            @arg LED1
//  *            @arg LED2
//  *            @arg LED3
//  *            @arg LED4 
//  * @retval None
//  */
//void BSP_IO_Low(IO_TypeDef io)
//{
//	GPIO_PORT[io]->BRR = GPIO_PIN[io];
//}

///**
//  * @brief  Toggles the selected LED.
//  * @param  Led: Specifies the Led to be toggled. 
//  *          This parameter can be one of following parameters:
//  *            @arg LED1
//  *            @arg LED2
//  *            @arg LED3
//  *            @arg LED4  
//  * @retval None
//  */
//void BSP_IO_Toggle(IO_TypeDef io)
//{
//  GPIO_PORT[io]->ODR ^= GPIO_PIN[io];
//}
/**
  * @brief  Configures Button GPIO and EXTI Line.
  * @param  Button: Specifies the Button to be configured.
  *          This parameter can be one of following parameters:
  *            @arg BUTTON_TAMPER: Tamper Push Button
  *            @arg BUTTON_RIGHT: Joystick Right Push Button 
  *            @arg BUTTON_LEFT: Joystick Left Push Button 
  *            @arg BUTTON_UP: Joystick Up Push Button 
  *            @arg BUTTON_DOWN: Joystick Down Push Button
  *            @arg BUTTON_SEL: Joystick Sel Push Button      
  * @param  Button_Mode: Specifies Button mode.
  *          This parameter can be one of following parameters:   
  *            @arg BUTTON_MODE_GPIO: Button will be used as simple IO 
  *            @arg BUTTON_MODE_EXTI: Button will be connected to EXTI line with interrupt
  *                     generation capability
  * @retval None
  */
void PB_Init(Button_TypeDef Button, ButtonMode_TypeDef Button_Mode)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable the BUTTON Clock */
  RCC_AHBPeriphClockCmd(BUTTON_CLK[Button], ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

  /* Configure Button pin as input */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_InitStructure.GPIO_Pin = BUTTON_PIN[Button];
  GPIO_Init(BUTTON_PORT[Button], &GPIO_InitStructure);

  if (Button_Mode == BUTTON_MODE_EXTI)
  {
    /* Connect Button EXTI Line to Button GPIO Pin */
    SYSCFG_EXTILineConfig(BUTTON_PORT_SOURCE[Button], BUTTON_PIN_SOURCE[Button]);

    /* Configure Button EXTI line */
    EXTI_InitStructure.EXTI_Line = BUTTON_EXTI_LINE[Button];
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    if (Button != BUTTON_TAMPER)
    {
      EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    }
    else
    {
      EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;    
    }
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /* Enable and set Button EXTI Interrupt to the lowest priority */
    NVIC_InitStructure.NVIC_IRQChannel = BUTTON_IRQn[Button];
    NVIC_InitStructure.NVIC_IRQChannelPriority = 0x03;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&NVIC_InitStructure); 
  }
}

/**
  * @brief  Returns the selected Button state.
  * @param  Button: Specifies the Button to be checked.
  *          This parameter can be one of following parameters:
  *            @arg BUTTON_TAMPER: Tamper Push Button
  *            @arg BUTTON_RIGHT: Joystick Right Push Button 
  *            @arg BUTTON_LEFT: Joystick Left Push Button 
  *            @arg BUTTON_UP: Joystick Up Push Button 
  *            @arg BUTTON_DOWN: Joystick Down Push Button
  *            @arg BUTTON_SEL: Joystick Sel Push Button     
  * @retval The Button GPIO pin value.
  */
uint32_t PB_GetState(Button_TypeDef Button)
{
  /* There is no Wakeup button on STM32072B-EVAL. */
  return GPIO_ReadInputDataBit(BUTTON_PORT[Button], BUTTON_PIN[Button]);
}

//void COM_DeInit(COM_TypeDef COM)
//{
//	/* USART configuration */
//  USART_DeInit(COM_USART[COM]);
//    
//  /* Enable USART */
//  USART_Cmd(COM_USART[COM], DISABLE);

//}	
///**
//  * @brief  Configures COM port.
//  * @param  COM: Specifies the COM port to be configured.
//  *          This parameter can be one of following parameters:    
//  *            @arg COM1
//  * @param  USART_InitStruct: pointer to a USART_InitTypeDef structure that
//  *         contains the configuration information for the specified USART peripheral.
//  * @retval None
//  */
//void COM_Init(COM_TypeDef COM, USART_InitTypeDef* USART_InitStruct)
//{
//  GPIO_InitTypeDef GPIO_InitStructure;

//  /* Enable GPIO clock */
//  RCC_AHBPeriphClockCmd(COM_TX_PORT_CLK[COM] | COM_RX_PORT_CLK[COM], ENABLE);

//  /* Enable USART clock */
//	//if(IS_RCC_APB2_PERIPH(COM_USART_CLK[COM]))
//	if(COM == COM2)
//		RCC_APB2PeriphClockCmd(COM_USART_CLK[COM], ENABLE); 
//	else //if(IS_RCC_APB1_PERIPH(COM_USART_CLK[COM]))
//		RCC_APB1PeriphClockCmd(COM_USART_CLK[COM], ENABLE);
//	
//  /* Connect PXx to USARTx_Tx */
//  GPIO_PinAFConfig(COM_TX_PORT[COM], COM_TX_PIN_SOURCE[COM], COM_TX_AF[COM]);

//  /* Connect PXx to USARTx_Rx */
//  GPIO_PinAFConfig(COM_RX_PORT[COM], COM_RX_PIN_SOURCE[COM], COM_RX_AF[COM]);
//  
//  /* Configure USART Tx as alternate function push-pull */
//  GPIO_InitStructure.GPIO_Pin = COM_TX_PIN[COM];
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//  GPIO_Init(COM_TX_PORT[COM], &GPIO_InitStructure);
//    
//  /* Configure USART Rx as alternate function push-pull */
//  GPIO_InitStructure.GPIO_Pin = COM_RX_PIN[COM];
//  GPIO_Init(COM_RX_PORT[COM], &GPIO_InitStructure);

//  /* USART configuration */
//  USART_Init(COM_USART[COM], USART_InitStruct);
//    
//  /* Enable USART */
//  USART_Cmd(COM_USART[COM], ENABLE);
//}


///**
//  * @brief  Retargets the C library printf function to the USART.
//  * @param  None
//  * @retval None
//  */
//void COM_SendChar(COM_TypeDef COM, uint8_t ch)
//{
//		USART_SendData(COM_USART[COM], ch);
//		/* Loop until transmit data register is empty */
//		while (USART_GetFlagStatus(COM_USART[COM], USART_FLAG_TXE) == RESET)
//		{}
//}

//void COM_SendString(COM_TypeDef COM, uint8_t *str)
//{
//  /* Place your implementation of fputc here */
//  /* e.g. write a character to the USART */
//	while(*str!='\0')
//	{
//		COM_SendChar(COM, *str);
//		str++;
//	}
//}

//void COM_ITConfig(COM_TypeDef COM)
//{
//	USART_ITConfig(COM_USART[COM], USART_IT_RXNE, ENABLE);
//}
//void COM_Receive(COM_TypeDef COM, uint8_t* str)
//{

//	*str = USART_ReceiveData(COM_USART[COM]);

//}
////uint8_t HexTable[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
////void Send_Hex(unsigned char dat)
////{
////   COM_SendChar('0');
////   COM_SendChar('x');
////   COM_SendChar(HexTable[dat>>4]);
////   COM_SendChar(HexTable[dat&0x0f]);
////   COM_SendChar(' ');
////}
/**
  * @brief  DeInitializes peripherals used by the I2C EEPROM driver.
  * @param  None
  * @retval None
  */
//void I2C_Deconfig(void)
//{
//  GPIO_InitTypeDef  GPIO_InitStructure; 
//   
//  /* BSP_I2C Peripheral Disable */
//  I2C_Cmd(BSP_I2C, DISABLE);
// 
//  /* BSP_I2C DeInit */
//  I2C_DeInit(BSP_I2C);

//  /* BSP_I2C Periph clock disable */
//  RCC_APB1PeriphClockCmd(BSP_I2C_CLK, DISABLE);
//    
//  /* GPIO configuration */  
//  /* Configure BSP_I2C pins: SCL */
//  GPIO_InitStructure.GPIO_Pin = BSP_I2C_SCL_PIN;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//  GPIO_Init(BSP_I2C_SCL_GPIO_PORT, &GPIO_InitStructure);

//  /* Configure BSP_I2C pins: SDA */
//  GPIO_InitStructure.GPIO_Pin = BSP_I2C_SDA_PIN;
//  GPIO_Init(BSP_I2C_SDA_GPIO_PORT, &GPIO_InitStructure);
////}
//uint32_t BSP_UART_DeInit(COM_TypeDef COM)
//{
//	while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET)
//	{;}
//	
//	/* USART configuration */
//  USART_DeInit(COM_USART[COM]);
//    
//  /* Enable USART */
//  USART_Cmd(COM_USART[COM], DISABLE);
//	
//	return SUCCESS;
//}
//uint32_t BSP_UART_Config(COM_TypeDef COM)
//{
//	USART_InitTypeDef USART_InitStructure;
//	GPIO_InitTypeDef GPIO_InitStructure;
//	
//	/* USART configuration */
//	USART_InitStructure.USART_BaudRate = 115200;
//  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
//  USART_InitStructure.USART_StopBits = USART_StopBits_1;
//  USART_InitStructure.USART_Parity = USART_Parity_No;
//  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
//  
// 
//	
//  /* Enable GPIO clock */
//  RCC_AHBPeriphClockCmd(COM_TX_PORT_CLK[COM] | COM_RX_PORT_CLK[COM], ENABLE);

//  /* Enable USART clock */
//	//if(IS_RCC_APB2_PERIPH(COM_USART_CLK[COM]))
//	if(COM == COM2)
//		RCC_APB2PeriphClockCmd(COM_USART_CLK[COM], ENABLE); 
//	else //if(IS_RCC_APB1_PERIPH(COM_USART_CLK[COM]))
//		RCC_APB1PeriphClockCmd(COM_USART_CLK[COM], ENABLE);
//	
//  /* Connect PXx to USARTx_Tx */
//  GPIO_PinAFConfig(COM_TX_PORT[COM], COM_TX_PIN_SOURCE[COM], COM_TX_AF[COM]);

//  /* Connect PXx to USARTx_Rx */
//  GPIO_PinAFConfig(COM_RX_PORT[COM], COM_RX_PIN_SOURCE[COM], COM_RX_AF[COM]);
//  
//  /* Configure USART Tx as alternate function push-pull */
//  GPIO_InitStructure.GPIO_Pin = COM_TX_PIN[COM];
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//  GPIO_Init(COM_TX_PORT[COM], &GPIO_InitStructure);
//    
//  /* Configure USART Rx as alternate function push-pull */
//  GPIO_InitStructure.GPIO_Pin = COM_RX_PIN[COM];
//  GPIO_Init(COM_RX_PORT[COM], &GPIO_InitStructure);
//  
//	USART_Init(COM_USART[COM], &USART_InitStructure);
//  /* Enable USART */
//  USART_Cmd(COM_USART[COM], ENABLE);
//	
//	
////	USART_ITConfig(EVAL_COM1, USART_IT_RXNE, ENABLE);
////	
////  /* USART1 IRQ Channel configuration */
////  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
////  NVIC_InitStructure.NVIC_IRQChannelPriority = 0x01;
////  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
////  NVIC_Init(&NVIC_InitStructure);
//  return SUCCESS;
//}

/**
  * @brief  Initializes the I2C source clock and IOs used to drive the EEPROM.
  * @param  None
  * @retval None
  */
void BSP_I2C_Config(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  I2C_InitTypeDef  I2C_InitStructure;
	
  /* Configure the I2C clock source. The clock is derived from the HSI */
  RCC_I2CCLKConfig(RCC_I2C1CLK_HSI);
    
  /* BSP_I2C_SCL_GPIO_CLK and BSP_I2C_SDA_GPIO_CLK Periph clock enable */
  RCC_AHBPeriphClockCmd(BSP_I2C_SCL_GPIO_CLK | BSP_I2C_SDA_GPIO_CLK, ENABLE);
  
  /* BSP_I2C Periph clock enable */
  RCC_APB1PeriphClockCmd(BSP_I2C_CLK, ENABLE);
  
  /* Connect PXx to I2C_SCL*/
  GPIO_PinAFConfig(BSP_I2C_SCL_GPIO_PORT, BSP_I2C_SCL_SOURCE, BSP_I2C_SCL_AF);
  
  /* Connect PXx to I2C_SDA*/
  GPIO_PinAFConfig(BSP_I2C_SDA_GPIO_PORT, BSP_I2C_SDA_SOURCE, BSP_I2C_SDA_AF);
  
  /* GPIO configuration */  
  /* Configure BSP_I2C pins: SCL */
  GPIO_InitStructure.GPIO_Pin = BSP_I2C_SCL_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//GPIO_OType_OD
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//GPIO_PuPd_NOPULL;
  GPIO_Init(BSP_I2C_SCL_GPIO_PORT, &GPIO_InitStructure);
  
  /* Configure BSP_I2C pins: SDA */
  GPIO_InitStructure.GPIO_Pin = BSP_I2C_SDA_PIN;
  GPIO_Init(BSP_I2C_SDA_GPIO_PORT, &GPIO_InitStructure);
	
	
	/* I2C configuration */
  /* BSP_I2C configuration */
  I2C_InitStructure.I2C_Mode = I2C_Mode_SMBusHost;//I2C_Mode_I2C;
  I2C_InitStructure.I2C_AnalogFilter = I2C_AnalogFilter_Enable;
  I2C_InitStructure.I2C_DigitalFilter = 0x00;
  I2C_InitStructure.I2C_OwnAddress1 = 0x00;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_Timing = 0x00901D2B;//BSP_I2C_TIMING;
  
  /* Apply EE_I2C configuration after enabling it */
  I2C_Init(BSP_I2C, &I2C_InitStructure);
   
  /* EE_I2C Peripheral Enable */
  I2C_Cmd(BSP_I2C, ENABLE);
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

