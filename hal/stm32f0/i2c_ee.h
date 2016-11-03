#ifndef __SEE_H
#define __SEE_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"

#define sRTC_HW_ADDRESS   


/* Select which EEPROM will be used with this driver */
#define EE_M24C64_32

#ifdef EE_M24C64_32
/* For M24C32 and M24C64 devices, E0,E1 and E2 pins are all used for device 
  address selection (ne need for additional address lines). According to the 
  Hardware connection on the board. */

 #define EE_HW_ADDRESS         0xA0   /* E0 = E1 = E2 = 0 */ 

#elif defined (EE_M24C08)
/* The M24C08W contains 4 blocks (128byte each) with the addresses below: E2 = 0 
   EEPROM Addresses defines */
 #define EE_HW_ADDRESS     0xA0   /* E2 = 0 */ 
 /*#define EE_HW_ADDRESS     0xA2*/ /* E2 = 0 */  
 /*#define EE_HW_ADDRESS     0xA4*/ /* E2 = 0 */
 /*#define EE_HW_ADDRESS     0xA6*/ /* E2 = 0 */

#elif defined (EE_M24LR64)
 #define EE_HW_ADDRESS         0xA0

#endif /* EE_M24C64_32 */



#if defined (EE_M24C08)
 #define EE_PAGESIZE           16
#elif defined (EE_M24C64_32)
 #define EE_PAGESIZE           32
#elif defined (EE_M24LR64)
 #define EE_PAGESIZE           4
#endif
   
/* Maximum Timeout values for flags and events waiting loops. These timeouts are
   not based on accurate values, they just guarantee that the application will 
   not remain stuck if the I2C communication is corrupted.
   You may modify these timeout values depending on CPU frequency and application
   conditions (interrupts routines ...). */   
#define EE_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define EE_LONG_TIMEOUT         ((uint32_t)(10 * EE_FLAG_TIMEOUT))

/* Maximum number of trials for EE_WaitEepromStandbyState() function */
#define EE_MAX_TRIALS_NUMBER     300
      
#define EE_OK                    1
#define EE_FAIL                  0   

void     EE_DeInit(void);
void     EE_Init(I2C_TypeDef* I2Cx);
uint32_t EE_ReadBuffer(uint8_t* pBuffer, uint16_t ReadAddr, uint16_t* NumByteToRead);
uint32_t EE_WritePage(uint8_t* pBuffer, uint16_t WriteAddr, uint8_t* NumByteToWrite);
void     EE_WriteBuffer(uint8_t* pBuffer, uint16_t WriteAddr, uint16_t NumByteToWrite);
uint32_t EE_WaitEepromStandbyState(void);

/* USER Callbacks: These are functions for which prototypes only are declared in
   EEPROM driver and that should be implemented into user application. */  
/* EE_TIMEOUT_UserCallback() function is called whenever a timeout condition 
   occurs during communication (waiting on an event that doesn't occur, bus 
   errors, busy devices ...).
   You can use the default timeout callback implementation by uncommenting the 
   define USE_DEFAULT_TIMEOUT_CALLBACK in stm320518_eval_i2c_ee.h file.
   Typically the user implementation of this callback should reset I2C peripheral
   and re-initialize communication or in worst case reset all the application. */
uint32_t EE_TIMEOUT_UserCallback(void);

#endif
