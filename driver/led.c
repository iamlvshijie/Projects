#include "led.h"
#include "hal_io.h"

uint32_t 	LED_Blink_Interval = Medium_Speed;
uint8_t 	LED_Blink_Flag = DISABLE;
Color_TypeDef localColor = NoColor;

void LED_Init(IO_TypeDef io)
{
	HAL_IO_Init(io);
	LED_Off(io);
}

void LED_On(IO_TypeDef io)
{
	HAL_IO_Low(io);
}

void LED_Off(IO_TypeDef io)
{
	HAL_IO_High(io);
}

void LED_Toggle(IO_TypeDef io)
{
	HAL_IO_Toggle(io);
}

#ifdef LED_COLOR
void LED_Color_On(Color_TypeDef color)
{
	LED_Blink_Flag = DISABLE;
	localColor = color;
  if(color == Red){
		LED_Off(LEDG_IO);
		LED_On(LEDR_IO);
	}
	if(color ==Green)
	{
		LED_On(LEDG_IO);
		LED_Off(LEDR_IO);
	}
	if(color == Orange)
	{
		LED_On(LEDG_IO);
		LED_On(LEDR_IO);
	}
}

void LED_Color_Off(void)
{
	localColor = NoColor;
	LED_Blink_Flag = DISABLE;
	LED_Off(LEDG_IO);
	LED_Off(LEDR_IO);
}

/* LED Color Blink Settings */
void LED_Color_Blink(Color_TypeDef color, Speed_TypeDef speed)
{
	LED_Color_Off();
	LED_Blink_Interval = speed;
	LED_Blink_Flag = ENABLE;
	localColor = color;
}

/* Called by Interrupt at stm32f0xx_it.c */
void LED_Blink_Int(void)
{
	if(LED_Blink_Flag==ENABLE) 
	{
		if(localColor==Red)
		{
			HAL_IO_Toggle(LEDG_IO);
		}
		else if(localColor==Green)
		{
			HAL_IO_Low(LEDG_IO);

		}
		else if(localColor==Orange)
		{
			HAL_IO_Toggle(LEDG_IO);
			HAL_IO_Toggle(LEDR_IO);
		}
	}

}

void LED_Color_Toggle(void)
{
	if(localColor == Green){
		HAL_IO_Toggle(LEDR_IO);
	}
	else if(localColor == Red)
	{
		HAL_IO_Toggle(LEDR_IO);
	}
	else if(localColor == Orange)
	{
		HAL_IO_Toggle(LEDG_IO);
		HAL_IO_Toggle(LEDR_IO);
	}
}
#endif
