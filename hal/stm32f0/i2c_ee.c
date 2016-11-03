#include "i2c_ee.h"
#include "BSP.h"



/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint16_t  EEAddress = 0; 
__IO uint16_t  sRTCAaddress = 0; 
__IO uint32_t  EETimeout = EE_LONG_TIMEOUT;
__IO uint16_t  EEDataNum;
I2C_TypeDef* EE_I2C;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/** @defgroup STM320518_EVAL_I2C_EE_Private_Functions
  * @{
  */ 

/**
  * @brief  DeInitializes peripherals used by the I2C EEPROM driver.
  * @param  None
  * @retval None
  */
void EE_DeInit(void)
{
  
}

/**
  * @brief  Initializes peripherals used by the I2C EEPROM driver.
  * @param  None
  * @retval None
  */
void EE_Init(I2C_TypeDef* I2Cx)
{ 
  EE_I2C = I2Cx;
  /* Select the EEPROM address */
  EEAddress = EE_HW_ADDRESS; 
}

/**
  * @brief  Reads a block of data from the EEPROM.
  * @param  pBuffer: pointer to the buffer that receives the data read from 
  *         the EEPROM.
  * @param  ReadAddr: EEPROM's internal address to start reading from.
  * @param  NumByteToRead: pointer to the variable holding number of bytes to 
  *         be read from the EEPROM.
  *
  * @retval EE_OK (0) if operation is correctly performed, else return value 
  *         different from EE_OK (0) or the timeout user callback.
  */
uint32_t EE_ReadBuffer(uint8_t* pBuffer, uint16_t ReadAddr, uint16_t* NumByteToRead)
{  
  uint32_t NumbOfSingle = 0, Count = 0, DataNum = 0, StartCom = 0;
  
  /* Get number of reload cycles */
  Count = (*NumByteToRead) / 255;  
  NumbOfSingle = (*NumByteToRead) % 255;
  
#ifdef EE_M24C08
  
  /* Configure slave address, nbytes, reload and generate start */
  I2C_TransferHandling(EE_I2C, EEAddress, 1, I2C_SoftEnd_Mode, I2C_Generate_Start_Write);
  
  /* Wait until TXIS flag is set */
  EETimeout = EE_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(EE_I2C, I2C_ISR_TXIS) == RESET)
  {
    if((EETimeout--) == 0) return EE_TIMEOUT_UserCallback();
  }
  
  /* Send memory address */
  I2C_SendData(EE_I2C, (uint8_t)ReadAddr);
  
#elif defined(EE_M24M01) || defined(EE_M24C64_32) || defined (EE_M24LR64) 
  
  /* Configure slave address, nbytes, reload and generate start */
  I2C_TransferHandling(EE_I2C, EEAddress, 2, I2C_SoftEnd_Mode, I2C_Generate_Start_Write);
  
  /* Wait until TXIS flag is set */
  EETimeout = EE_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(EE_I2C, I2C_ISR_TXIS) == RESET)
  {
    if((EETimeout--) == 0) return EE_TIMEOUT_UserCallback();
  }
  
  /* Send MSB of memory address */
  I2C_SendData(EE_I2C, (uint8_t)((ReadAddr & 0xFF00) >> 8));
  
  /* Wait until TXIS flag is set */
  EETimeout = EE_LONG_TIMEOUT;  
  while(I2C_GetFlagStatus(EE_I2C, I2C_ISR_TXIS) == RESET)
  {
    if((EETimeout--) == 0) return EE_TIMEOUT_UserCallback();
  }
  
  /* Send LSB of memory address  */
  I2C_SendData(EE_I2C, (uint8_t)(ReadAddr & 0x00FF));
  
#endif /* EE_M24C08 */ 
  
  /* Wait until TC flag is set */
  EETimeout = EE_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(EE_I2C, I2C_ISR_TC) == RESET)
  {
    if((EETimeout--) == 0) return EE_TIMEOUT_UserCallback();
  }  
  
  /* If number of Reload cycles is not equal to 0 */
  if (Count != 0)
  {
    /* Starting communication */
    StartCom = 1;
    
    /* Wait until all reload cycles are performed */
    while( Count != 0)
    { 
      /* If a read transfer is performed */
      if (StartCom == 0)      
      {
        /* Wait until TCR flag is set */
        EETimeout = EE_LONG_TIMEOUT; 
        while(I2C_GetFlagStatus(EE_I2C, I2C_ISR_TCR) == RESET)
        {
          if((EETimeout--) == 0) return EE_TIMEOUT_UserCallback();
        }
      }      
      
      /* if remains one read cycle */
      if ((Count == 1) && (NumbOfSingle == 0))
      {
        /* if starting communication */
        if (StartCom != 0)
        {
          /* Configure slave address, end mode and start condition */
          I2C_TransferHandling(EE_I2C, EEAddress, 255, I2C_AutoEnd_Mode, I2C_Generate_Start_Read);
        }
        else
        {
          /* Configure slave address, end mode */
          I2C_TransferHandling(EE_I2C, EEAddress, 255, I2C_AutoEnd_Mode, I2C_No_StartStop);          
        }
      }
      else 
      {
        /* if starting communication */
        if (StartCom != 0)
        {
          /* Configure slave address, end mode and start condition */
          I2C_TransferHandling(EE_I2C, EEAddress, 255, I2C_Reload_Mode, I2C_Generate_Start_Read);
        }
        else
        {
          /* Configure slave address, end mode */
          I2C_TransferHandling(EE_I2C, EEAddress, 255, I2C_Reload_Mode, I2C_No_StartStop);          
        } 
      }
      
      /* Update local variable */
      StartCom = 0;      
      DataNum = 0;
      
      /* Wait until all data are received */
      while (DataNum != 255)
      {        
        /* Wait until RXNE flag is set */
        EETimeout = EE_LONG_TIMEOUT;
        while(I2C_GetFlagStatus(EE_I2C, I2C_ISR_RXNE) == RESET)
        {
          if((EETimeout--) == 0) return EE_TIMEOUT_UserCallback();
        }
        
        /* Read data from RXDR */
        pBuffer[DataNum]= I2C_ReceiveData(EE_I2C);
        
        /* Update number of received data */
        DataNum++;
        (*NumByteToRead)--;
      }      
      /* Update Pointer of received buffer */ 
      pBuffer += DataNum;  
      
      /* update number of reload cycle */
      Count--;
    }
    
    /* If number of single data is not equal to 0 */
    if (NumbOfSingle != 0)
    {            
      /* Wait until TCR flag is set */
      EETimeout = EE_LONG_TIMEOUT;   
      while(I2C_GetFlagStatus(EE_I2C, I2C_ISR_TCR) == RESET)
      {
        if((EETimeout--) == 0) return EE_TIMEOUT_UserCallback();
      }
      
      /* Update CR2 : set Nbytes and end mode */
      I2C_TransferHandling(EE_I2C, EEAddress, (uint8_t)(NumbOfSingle), I2C_AutoEnd_Mode, I2C_No_StartStop);
      
      /* Reset local variable */
      DataNum = 0;
      
      /* Wait until all data are received */
      while (DataNum != NumbOfSingle)
      {        
        /* Wait until RXNE flag is set */
        EETimeout = EE_LONG_TIMEOUT;
        while(I2C_GetFlagStatus(EE_I2C, I2C_ISR_RXNE) == RESET)
        {
          if((EETimeout--) == 0) return EE_TIMEOUT_UserCallback();
        }
        
        /* Read data from RXDR */
        pBuffer[DataNum]= I2C_ReceiveData(EE_I2C);
        
        /* Update number of received data */
        DataNum++;
        (*NumByteToRead)--;
      } 
    }
  }
  else
  {
    /* Update CR2 : set Slave Address , set read request, generate Start and set end mode */
    I2C_TransferHandling(EE_I2C, EEAddress, (uint32_t)(NumbOfSingle), I2C_AutoEnd_Mode, I2C_Generate_Start_Read);
    
    /* Reset local variable */
    DataNum = 0;
    
    /* Wait until all data are received */
    while (DataNum != NumbOfSingle)
    {
      /* Wait until RXNE flag is set */
      EETimeout = EE_LONG_TIMEOUT; 
      while(I2C_GetFlagStatus(EE_I2C, I2C_ISR_RXNE) == RESET)
      {
        if((EETimeout--) == 0) return EE_TIMEOUT_UserCallback();
      }
      
      /* Read data from RXDR */
      pBuffer[DataNum]= I2C_ReceiveData(EE_I2C);
      
      /* Update number of received data */
      DataNum++;
      (*NumByteToRead)--;
    }    
  }  
  
  /* Wait until STOPF flag is set */
  EETimeout = EE_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(EE_I2C, I2C_ISR_STOPF) == RESET)
  {
    if((EETimeout--) == 0) return EE_TIMEOUT_UserCallback();
  }
  
  /* Clear STOPF flag */
  I2C_ClearFlag(EE_I2C, I2C_ICR_STOPCF);
  
  /* If all operations OK, return EE_OK (0) */
  return EE_OK;
}

/**
  * @brief  Writes more than one byte to the EEPROM with a single WRITE cycle.
  *
  * @note   The number of bytes (combined to write start address) must not 
  *         cross the EEPROM page boundary. This function can only write into
  *         the boundaries of an EEPROM page.
  * @note   This function doesn't check on boundaries condition (in this driver 
  *         the function EE_WriteBuffer() which calls EE_WritePage() is 
  *         responsible of checking on Page boundaries).
  * 
  * @param  pBuffer: pointer to the buffer containing the data to be written to 
  *         the EEPROM.
  * @param  WriteAddr: EEPROM's internal address to write to.
  * @param  NumByteToWrite: pointer to the variable holding number of bytes to 
  *         be written into the EEPROM.
  *
  * @retval EE_OK (0) if operation is correctly performed, else return value 
  *         different from EE_OK (0) or the timeout user callback.
  */
uint32_t EE_WritePage(uint8_t* pBuffer, uint16_t WriteAddr, uint8_t* NumByteToWrite)
{   
  uint32_t DataNum = 0;
  
#ifdef EE_M24C08
  
  /* Configure slave address, nbytes, reload and generate start */
  I2C_TransferHandling(EE_I2C, EEAddress, 1, I2C_Reload_Mode, I2C_Generate_Start_Write);
  
  /* Wait until TXIS flag is set */
  EETimeout = EE_LONG_TIMEOUT;  
  while(I2C_GetFlagStatus(EE_I2C, I2C_ISR_TXIS) == RESET)
  {
    if((EETimeout--) == 0) return EE_TIMEOUT_UserCallback();
  }
  
  /* Send memory address */
  I2C_SendData(EE_I2C, (uint8_t)WriteAddr);
  
#elif defined(EE_M24M01) || defined(EE_M24C64_32) || defined (EE_M24LR64)
  
  /* Configure slave address, nbytes, reload and generate start */
  I2C_TransferHandling(EE_I2C, EEAddress, 2, I2C_Reload_Mode, I2C_Generate_Start_Write);
  
  /* Wait until TXIS flag is set */
  EETimeout = EE_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(EE_I2C, I2C_ISR_TXIS) == RESET)
  {
    if((EETimeout--) == 0) return EE_TIMEOUT_UserCallback();
  }
  
  /* Send MSB of memory address */
  I2C_SendData(EE_I2C, (uint8_t)((WriteAddr & 0xFF00) >> 8));  
  
  /* Wait until TXIS flag is set */
  EETimeout = EE_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(EE_I2C, I2C_ISR_TXIS) == RESET)
  {
    if((EETimeout--) == 0) return EE_TIMEOUT_UserCallback();
  }
  
  /* Send LSB of memory address  */
  I2C_SendData(EE_I2C, (uint8_t)(WriteAddr & 0x00FF));
  
#endif /* EE_M24C08 */
  
  /* Wait until TCR flag is set */
  EETimeout = EE_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(EE_I2C, I2C_ISR_TCR) == RESET)
  {
    if((EETimeout--) == 0) return EE_TIMEOUT_UserCallback();
  }
  
  /* Update CR2 : set Slave Address , set write request, generate Start and set end mode */
  I2C_TransferHandling(EE_I2C, EEAddress, (uint8_t)(*NumByteToWrite), I2C_AutoEnd_Mode, I2C_No_StartStop);
  
  while (DataNum != (*NumByteToWrite))
  {      
    /* Wait until TXIS flag is set */
    EETimeout = EE_LONG_TIMEOUT;
    while(I2C_GetFlagStatus(EE_I2C, I2C_ISR_TXIS) == RESET)
    {
      if((EETimeout--) == 0) return EE_TIMEOUT_UserCallback();
    }  
    
    /* Write data to TXDR */
    I2C_SendData(EE_I2C, (uint8_t)(pBuffer[DataNum]));
    
    /* Update number of transmitted data */
    DataNum++;   
  }  
  
  /* Wait until STOPF flag is set */
  EETimeout = EE_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(EE_I2C, I2C_ISR_STOPF) == RESET)
  {
    if((EETimeout--) == 0) return EE_TIMEOUT_UserCallback();
  }   
  
  /* Clear STOPF flag */
  I2C_ClearFlag(EE_I2C, I2C_ICR_STOPCF);
  
  /* If all operations OK, return EE_OK (0) */
  return EE_OK;
}

/**
  * @brief  Writes buffer of data to the I2C EEPROM.
  * @param  pBuffer: pointer to the buffer  containing the data to be written 
  *         to the EEPROM.
  * @param  WriteAddr: EEPROM's internal address to write to.
  * @param  NumByteToWrite: number of bytes to write to the EEPROM.
  * @retval None
  */
void EE_WriteBuffer(uint8_t* pBuffer, uint16_t WriteAddr, uint16_t NumByteToWrite)
{
  uint16_t NumOfPage = 0, NumOfSingle = 0, count = 0;
  uint16_t Addr = 0;
  
  Addr = WriteAddr % EE_PAGESIZE;
  count = EE_PAGESIZE - Addr;
  NumOfPage =  NumByteToWrite / EE_PAGESIZE;
  NumOfSingle = NumByteToWrite % EE_PAGESIZE;
  
  /* If WriteAddr is EE_PAGESIZE aligned  */
  if(Addr == 0) 
  {
    /* If NumByteToWrite < EE_PAGESIZE */
    if(NumOfPage == 0) 
    {
      /* Store the number of data to be written */
      EEDataNum = NumOfSingle;
      /* Start writing data */
      EE_WritePage(pBuffer, WriteAddr, (uint8_t*)(&EEDataNum));
      EE_WaitEepromStandbyState();
    }
    /* If NumByteToWrite > EE_PAGESIZE */
    else  
    {
      while(NumOfPage--)
      {
        /* Store the number of data to be written */
        EEDataNum = EE_PAGESIZE;        
        EE_WritePage(pBuffer, WriteAddr, (uint8_t*)(&EEDataNum)); 
        EE_WaitEepromStandbyState();
        WriteAddr +=  EE_PAGESIZE;
        pBuffer += EE_PAGESIZE;
      }
      
      if(NumOfSingle!=0)
      {
        /* Store the number of data to be written */
        EEDataNum = NumOfSingle;          
        EE_WritePage(pBuffer, WriteAddr, (uint8_t*)(&EEDataNum));
        EE_WaitEepromStandbyState();
      }
    }
  }
  /* If WriteAddr is not EE_PAGESIZE aligned  */
  else 
  {
    /* If NumByteToWrite < EE_PAGESIZE */
    if(NumOfPage== 0) 
    {
      /* If the number of data to be written is more than the remaining space 
      in the current page: */
      if (NumByteToWrite > count)
      {
        /* Store the number of data to be written */
        EEDataNum = count;        
        /* Write the data contained in same page */
        EE_WritePage(pBuffer, WriteAddr, (uint8_t*)(&EEDataNum));
        EE_WaitEepromStandbyState();      
        
        /* Store the number of data to be written */
        EEDataNum = (NumByteToWrite - count);          
        /* Write the remaining data in the following page */
        EE_WritePage((uint8_t*)(pBuffer + count), (WriteAddr + count), (uint8_t*)(&EEDataNum));
        EE_WaitEepromStandbyState();        
      }      
      else      
      {
        /* Store the number of data to be written */
        EEDataNum = NumOfSingle;         
        EE_WritePage(pBuffer, WriteAddr, (uint8_t*)(&EEDataNum));
        EE_WaitEepromStandbyState();        
      }     
    }
    /* If NumByteToWrite > EE_PAGESIZE */
    else
    {
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / EE_PAGESIZE;
      NumOfSingle = NumByteToWrite % EE_PAGESIZE;
      
      if(count != 0)
      {  
        /* Store the number of data to be written */
        EEDataNum = count;         
        EE_WritePage(pBuffer, WriteAddr, (uint8_t*)(&EEDataNum));
        EE_WaitEepromStandbyState();
        WriteAddr += count;
        pBuffer += count;
      } 
      
      while(NumOfPage--)
      {
        /* Store the number of data to be written */
        EEDataNum = EE_PAGESIZE;          
        EE_WritePage(pBuffer, WriteAddr, (uint8_t*)(&EEDataNum));
        EETimeout = EE_LONG_TIMEOUT;
        EE_WaitEepromStandbyState();
        WriteAddr +=  EE_PAGESIZE;
        pBuffer += EE_PAGESIZE;  
      }
      if(NumOfSingle != 0)
      {
        /* Store the number of data to be written */
        EEDataNum = NumOfSingle;           
        EE_WritePage(pBuffer, WriteAddr, (uint8_t*)(&EEDataNum)); 
        EE_WaitEepromStandbyState();
      }
    }
  }  
}

/**
  * @brief  Wait for EEPROM Standby state.
  * 
  * @note  This function allows to wait and check that EEPROM has finished the 
  *        last operation. It is mostly used after Write operation: after receiving
  *        the buffer to be written, the EEPROM may need additional time to actually
  *        perform the write operation. During this time, it doesn't answer to
  *        I2C packets addressed to it. Once the write operation is complete
  *        the EEPROM responds to its address.
  * 
  * @param  None
  *
  * @retval EE_OK (0) if operation is correctly performed, else return value 
  *         different from EE_OK (0) or the timeout user callback.
  */
uint32_t EE_WaitEepromStandbyState(void)      
{
  __IO uint32_t EETrials = 0;
  
  /* Keep looping till the slave acknowledge his address or maximum number 
  of trials is reached (this number is defined by EE_MAX_TRIALS_NUMBER define
  in stm32373c_eval_i2c_ee.h file) */
  
  /* Configure CR2 register : set Slave Address and end mode */
  I2C_TransferHandling(EE_I2C, EEAddress, 0, I2C_AutoEnd_Mode, I2C_No_StartStop);  
  
  do
  { 
    /* Initialize EETimeout */
    EETimeout = EE_FLAG_TIMEOUT;
    
    /* Clear NACKF */
    I2C_ClearFlag(EE_I2C, I2C_ICR_NACKCF | I2C_ICR_STOPCF);
    
    /* Generate start */
    I2C_GenerateSTART(EE_I2C, ENABLE);
    
    /* Wait until timeout elapsed */
    while (EETimeout-- != 0); 
    
    /* Check if the maximum allowed number of trials has bee reached */
    if (EETrials++ == EE_MAX_TRIALS_NUMBER)
    {
      /* If the maximum number of trials has been reached, exit the function */
      return EE_TIMEOUT_UserCallback();
    }
  }
  while(I2C_GetFlagStatus(EE_I2C, I2C_ISR_NACKF) != RESET);
  
  /* Clear STOPF */
  I2C_ClearFlag(EE_I2C, I2C_ICR_STOPCF);
  
  /* Return EE_OK if device is ready */
  return EE_OK;
}

#define USE_DEFAULT_TIMEOUT_CALLBACK

#ifdef USE_DEFAULT_TIMEOUT_CALLBACK
/**
  * @brief  Basic management of the timeout situation.
  * @param  None.
  * @retval 0.
  */
uint32_t EE_TIMEOUT_UserCallback(void)
{
  /* The following code allows I2C error recovery and return to normal communication
     if the error source doesn? still exist (ie. hardware issue..) */
  
  /* Reinitialize all resources */
//  EE_DeInit();
//  I2C_Config();

  /* At this stage the I2C error should be recovered and device can communicate
     again (except if the error source still exist).
     User can implement mechanism (ex. test on max trial number) to manage situation
     when the I2C can't recover from current error. */
  
  return 0;
}
#endif /* USE_DEFAULT_TIMEOUT_CALLBACK */
