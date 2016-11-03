#ifndef __TIME_H_
#define __TIME_H_

#include "stm32f0xx.h"

#define Weekday_Monday             ((uint8_t)0x01)
#define Weekday_Tuesday            ((uint8_t)0x02)
#define Weekday_Wednesday          ((uint8_t)0x03)
#define Weekday_Thursday           ((uint8_t)0x04)
#define Weekday_Friday             ((uint8_t)0x05)
#define Weekday_Saturday           ((uint8_t)0x06)
#define Weekday_Sunday             ((uint8_t)0x07)


typedef struct 
{
	uint8_t year;		//0x15:2015;BCD format;
	uint8_t month;	//0x08:August;BCD format;
	uint8_t date;		//0x30:30;BCD format;
	uint8_t hour;		//0x23:23;24-Hour;BCD format;
	uint8_t minute; //0x59:59;BCD format;
	uint8_t second; //0x59:59;BCD format;
	uint8_t weekday;	//0x01:Monday;
}Time_TypeDef;


#endif /* __TIME_H_ */
