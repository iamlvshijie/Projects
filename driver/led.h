#ifndef __LED_H_
#define __LED_H_	 

#include "hal_io.h"

//#define LED_COLOR

typedef enum 
{
	NoColor = 0,
  Green   = 1,
  Red     = 2,
  Orange  = 3
}Color_TypeDef;

typedef enum 
{
  Fast_Speed 		= 0x082,
  Medium_Speed 	= 0x1F4,
  Slow_Speed 		= 0x3E8
}Speed_TypeDef;

void LED_Init(IO_TypeDef io);	
void LED_On(IO_TypeDef io);
void LED_Off(IO_TypeDef io);
void LED_Toggle(IO_TypeDef io);
void LED_Blink_Int(void);

void LED_Color_On(Color_TypeDef color);
void LED_Color_Off(void);
void LED_Color_Blink(Color_TypeDef color, Speed_TypeDef speed);
void LED_Color_Toggle(void);
#endif /* __LED_H_ */

















