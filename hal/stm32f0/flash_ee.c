/**
  ******************************************************************************
  * @file    STM32F0xx_EEPROM_Emulation/src/eeprom.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    29-May-2012
  * @brief   This file provides all the EEPROM emulation firmware functions.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
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

/** @addtogroup STM32F0xx_EEPROM_Emulation
  * @{
  */ 

/* Includes ------------------------------------------------------------------*/
#include "eeprom.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Global variable used to store variable value in read sequence */
uint16_t DataVar = 0;

/* Virtual address defined by the user: 0xFFFF value is prohibited */
extern uint16_t VirtAddVarTab[NB_OF_VAR];

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static FLASH_Status EE_Format(void);
//static uint16_t EE_VerifyPageFullWriteVariable(uint16_t VirtAddress, uint16_t Data);
//static uint16_t EE_PageTransfer(uint16_t VirtAddress, uint16_t Data);
//static uint16_t EE_FindValidPage(uint8_t Operation);

/**
  * @brief  Restore the pages to a known good state in case of page's status
  *   corruption after a power loss.
  * @param  None.
  * @retval - Flash error code: on write Flash error
  *         - FLASH_COMPLETE: on success
  */
uint16_t EE_Init(void)
{
  uint16_t PageStatus0 = 6;


  uint16_t  FlashStatus;

  /* Get Page0 status */
  PageStatus0 = (*(__IO uint16_t*)PAGE0_BASE_ADDRESS);


  /* Check for invalid header states and repair if necessary */
  switch (PageStatus0)
  {
    case ERASED:
      
      break;

    case RECEIVE_DATA:
      
      break;

    case VALID_PAGE:
      
      break;

    default:  /* Any other state -> format eeprom */
      /* Erase both Page0 and Page1 and set Page0 as valid page */
      FlashStatus = EE_Format();
      /* If erase/program operation was failed, a Flash error code is returned */
      if (FlashStatus != FLASH_COMPLETE)
      {
        return FlashStatus;
      }
      break;
  }

  return FLASH_COMPLETE;
}

/**
  * @brief  Returns the last stored variable data, if found, which correspond to
  *   the passed virtual address
  * @param  VirtAddress: Variable virtual address
  * @param  Data: Global variable contains the read variable value
  * @retval Success or error status:
  *           - 0: if variable was found
  *           - 1: if the variable was not found
  *           - NO_VALID_PAGE: if no valid page was found.
  */
uint16_t EE_ReadVariable(uint16_t Address, uint16_t* Data)
{
  uint16_t ValidPage = PAGE0;
  uint16_t ReadStatus = 1;

  /* Get the valid Page start Address */
  uint32_t PageStartAddress = (uint32_t)(EEPROM_START_ADDRESS + (uint32_t)(ValidPage * PAGE_SIZE));

  /* Get the valid Page end Address */
  uint32_t PageEndAddress = (uint32_t)((EEPROM_START_ADDRESS - 2) + (uint32_t)((1 + ValidPage) * PAGE_SIZE));

	uint32_t ReadAddress = Address + EEPROM_START_ADDRESS;
  /* Check each active page address starting from end */
  if ( ReadAddress > (PageEndAddress + 2) || ReadAddress < PageStartAddress)
  {
		 ReadStatus = 1;     
  }
	else
	{
		*Data = (*(__IO uint16_t*)(ReadAddress));
		 ReadStatus = 0;
	}

  /* Return ReadStatus value: (0: variable exist, 1: variable doesn't exist) */
  return ReadStatus;
}

/**
  * @brief  Writes/upadtes variable data in EEPROM.
  * @param  VirtAddress: Variable virtual address
  * @param  Data: 16 bit data to be written
  * @retval Success or error status:
  *           - FLASH_COMPLETE: on success
  *           - PAGE_FULL: if valid page is full
  *           - NO_VALID_PAGE: if no valid page was found
  *           - Flash error code: on write Flash error
  */
uint16_t EE_WriteVariable(uint16_t Address, uint16_t Data)
{
  uint16_t Status = 0;

  /* Write the variable virtual address and value in the EEPROM */
  FLASH_Status FlashStatus = FLASH_COMPLETE;
  uint16_t ValidPage = PAGE0;

  /* Get the valid Page start Address */
  uint32_t PageStartAddress = (uint32_t)(EEPROM_START_ADDRESS + (uint32_t)(ValidPage * PAGE_SIZE));

  /* Get the valid Page end Address */
  uint32_t PageEndAddress = (uint32_t)((EEPROM_START_ADDRESS - 2) + (uint32_t)((1 + ValidPage) * PAGE_SIZE));

	uint32_t WriteAddress = Address + EEPROM_START_ADDRESS;
  /* Check each active page address starting from begining */
  if ( WriteAddress > (PageEndAddress + 2) || WriteAddress < PageStartAddress)
  {
		Status = 1;  
  }
	else
	{
		  /* Set variable data */
      FlashStatus = FLASH_ProgramHalfWord(WriteAddress, Data);
      /* If program operation was failed, a Flash error code is returned */
      if (FlashStatus != FLASH_COMPLETE)
      {
        return FlashStatus;
      }
			
     
	}

  return Status;
}

/**
  * @brief  Erases PAGE0 and PAGE1 and writes VALID_PAGE header to PAGE0
  * @param  None
  * @retval Status of the last operation (Flash write or erase) done during
  *         EEPROM formating
  */
static FLASH_Status EE_Format(void)
{
  FLASH_Status FlashStatus = FLASH_COMPLETE;

  /* Erase Page0 */
  FlashStatus = FLASH_ErasePage(PAGE0_BASE_ADDRESS);

  /* If erase operation was failed, a Flash error code is returned */
  if (FlashStatus != FLASH_COMPLETE)
  {
    return FlashStatus;
  }

  /* Set Page0 as valid page: Write VALID_PAGE at Page0 base address */
  FlashStatus = FLASH_ProgramHalfWord(PAGE0_BASE_ADDRESS, VALID_PAGE);

  /* If program operation was failed, a Flash error code is returned */
  if (FlashStatus != FLASH_COMPLETE)
  {
    return FlashStatus;
  }

  /* Erase Page1 */
  FlashStatus = FLASH_ErasePage(PAGE1_BASE_ADDRESS);

  /* Return Page1 erase operation status */
  return FlashStatus;
}


/**
  * @brief  Transfers last updated variables data from the full Page to
  *   an empty one.
  * @param  VirtAddress: 16 bit virtual address of the variable
  * @param  Data: 16 bit data to be written as variable value
  * @retval Success or error status:
  *           - FLASH_COMPLETE: on success
  *           - PAGE_FULL: if valid page is full
  *           - NO_VALID_PAGE: if no valid page was found
  *           - Flash error code: on write Flash error
  */
//static uint16_t EE_PageTransfer(uint16_t VirtAddress, uint16_t Data)
//{
//  FLASH_Status FlashStatus = FLASH_COMPLETE;
//  uint32_t NewPageAddress = 0x080103FF, OldPageAddress = 0x08010000;
//  uint16_t ValidPage = PAGE0, VarIdx = 0;
//  uint16_t EepromStatus = 0, ReadStatus = 0;

//  /* Get active Page for read operation */
//  ValidPage = EE_FindValidPage(READ_FROM_VALID_PAGE);

//  if (ValidPage == PAGE1)       /* Page1 valid */
//  {
//    /* New page address where variable will be moved to */
//    NewPageAddress = PAGE0_BASE_ADDRESS;

//    /* Old page address where variable will be taken from */
//    OldPageAddress = PAGE1_BASE_ADDRESS;
//  }
//  else if (ValidPage == PAGE0)  /* Page0 valid */
//  {
//    /* New page address where variable will be moved to */
//    NewPageAddress = PAGE1_BASE_ADDRESS;

//    /* Old page address where variable will be taken from */
//    OldPageAddress = PAGE0_BASE_ADDRESS;
//  }
//  else
//  {
//    return NO_VALID_PAGE;       /* No valid Page */
//  }

//  /* Set the new Page status to RECEIVE_DATA status */
//  FlashStatus = FLASH_ProgramHalfWord(NewPageAddress, RECEIVE_DATA);
//  /* If program operation was failed, a Flash error code is returned */
//  if (FlashStatus != FLASH_COMPLETE)
//  {
//    return FlashStatus;
//  }

//  /* Write the variable passed as parameter in the new active page */
//  EepromStatus = EE_VerifyPageFullWriteVariable(VirtAddress, Data);
//  /* If program operation was failed, a Flash error code is returned */
//  if (EepromStatus != FLASH_COMPLETE)
//  {
//    return EepromStatus;
//  }

//  /* Transfer process: transfer variables from old to the new active page */
//  for (VarIdx = 0; VarIdx < NB_OF_VAR; VarIdx++)
//  {
//    if (VirtAddVarTab[VarIdx] != VirtAddress)  /* Check each variable except the one passed as parameter */
//    {
//      /* Read the other last variable updates */
//      ReadStatus = EE_ReadVariable(VirtAddVarTab[VarIdx], &DataVar);
//      /* In case variable corresponding to the virtual address was found */
//      if (ReadStatus != 0x1)
//      {
//        /* Transfer the variable to the new active page */
//        EepromStatus = EE_VerifyPageFullWriteVariable(VirtAddVarTab[VarIdx], DataVar);
//        /* If program operation was failed, a Flash error code is returned */
//        if (EepromStatus != FLASH_COMPLETE)
//        {
//          return EepromStatus;
//        }
//      }
//    }
//  }

//  /* Erase the old Page: Set old Page status to ERASED status */
//  FlashStatus = FLASH_ErasePage(OldPageAddress);
//  /* If erase operation was failed, a Flash error code is returned */
//  if (FlashStatus != FLASH_COMPLETE)
//  {
//    return FlashStatus;
//  }

//  /* Set new Page status to VALID_PAGE status */
//  FlashStatus = FLASH_ProgramHalfWord(NewPageAddress, VALID_PAGE);
//  /* If program operation was failed, a Flash error code is returned */
//  if (FlashStatus != FLASH_COMPLETE)
//  {
//    return FlashStatus;
//  }

//  /* Return last operation flash status */
//  return FlashStatus;
//}

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
