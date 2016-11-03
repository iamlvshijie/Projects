/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __COMMON_H
#define __COMMON_H

#include <stm32f0xx.h>

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Constants used by Serial Command Line Mode */
#define CMD_STRING_SIZE       128

/* Exported macro ------------------------------------------------------------*/
/* Common routines */
#define IS_AF(c)             ((c >= 'A') && (c <= 'F'))
#define IS_af(c)             ((c >= 'a') && (c <= 'f'))
#define IS_09(c)             ((c >= '0') && (c <= '9'))
#define ISVALIDHEX(c)        IS_AF(c) || IS_af(c) || IS_09(c)
#define ISVALIDDEC(c)        IS_09(c)
#define CONVERTDEC(c)        (c - '0')

#define CONVERTHEX_alpha(c)  (IS_AF(c) ? (c - 'A'+10) : (c - 'a'+10))
#define CONVERTHEX(c)        (IS_09(c) ? (c - '0') : CONVERTHEX_alpha(c))

/* Exported functions ------------------------------------------------------- */
void 			Int2Str(uint8_t* str,int32_t intnum);
uint32_t 	Str2Int(uint8_t *inputstr,int32_t *intnum);
uint16_t 	BufferCmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);  
void 			BufferCpy(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);
void 			BufferReset(uint8_t* pBuffer, uint16_t BufferLength);
void 			BufferOffsetCpy(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t Offset1, uint16_t Offset2, uint16_t CpyLength);

#endif  /* __COMMON_H */
