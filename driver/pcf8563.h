#ifndef _PCF8563_H_
#define _PCF8563_H_

#include <stm32f0xx.h>


#define PCF8563_I2C_Address 		0xA2

#define CTRL_STATUS_REG1_ADDR		0x00
#define CTRL_STATUS_REG2_ADDR		0x01

#define SECOND_REG_ADDR					0x02
#define MINUTE_REG_ADDR					0x03
#define HOUR_REG_ADDR						0x04
#define DAY_REG_ADDR						0x05
#define WEEK_REG_ADDR						0x06
#define MONTH_REG_ADDR					0x07
#define YEAR_REG_ADDR						0x08

#define MINUTE_AE_BUF	0x09
#define HOUR_AE_BUF		0x0A
#define DAY_AE_BUF		0x0B
#define WEEK_AE_BUF		0x0C

#define CLK_FRQ_BUF		0x0D
#define TIMER_CTRL_BUF	0x0E
#define COUNT_VAL_BUF	0x0F


#define changeIntToHex(dec)		( ( ((dec)/10) <<4 ) + ((dec)%10) )

#define changeHexToInt(hex)		( ( ((hex)>>4) *10 ) + ((hex)%16) )

uint32_t PCF8563_Init(I2C_TypeDef* I2Cx);
uint32_t PCF8563_ReadTime(uint8_t* pBuffer);
uint32_t PCF8563_SetTime(uint8_t* pBuffer);
uint32_t PCF8563_TIMEOUT_UserCallback(void);
//void WriteAByte(u8 wordAdr,u8 dat)
//{
//	I2C_Start();
//	I2C_SendDAdr(WD_DADR);
//	I2C_SendDat(wordAdr);
//	I2C_SendDat(dat);
//	I2C_stop();
//}

//void PCF8563_getTime(u8 *buf)
//{  
//	u8 i;
//	I2C_Start();
//	I2C_SendDAdr(WD_DADR);
//	I2C_SendDat(SECOND_DATA_BUF);
//	I2C_Start();
//	I2C_SendDAdr(RD_DADR);
//	for(i=0;i<2;i++)
//	{
//		buf[i]=I2C_RcvDat();
//	}
//	I2C_NAck();
//	buf[i]=I2C_RcvDat();
//	I2C_stop();
//	I2C_Ack();

//	buf[0]=buf[0]&0x7f;		//get second data
//	buf[1]=buf[1]&0x7f;		//get minute data
//	buf[2]=buf[2]&0x3f;		//get hour data
//	
//	buf[0]=changeHexToInt(buf[0]);
//	buf[1]=changeHexToInt(buf[1]);
//	buf[2]=changeHexToInt(buf[2]);
//}

//void PCF8563_setTime(u8 hour,u8 minute,u8 second)
//{
//	hour=changeIntToHex(hour);		//????Dex?????Hex??
//	minute=changeIntToHex(minute);
//	second=changeIntToHex(second);

//	WriteAByte(HOUR_DATA_BUF,hour);
//	WriteAByte(MINUTE_DATA_BUF,minute);
//	WriteAByte(SECOND_DATA_BUF,second);
//}

//void PCF8563_init(u8 hour,u8 mintue,u8 second)
//{
//	I2C_Init();
//	WriteAByte(CTRL_BUF1,0x00);			//basic setting
//	WriteAByte(CTRL_BUF2,0x00);
//	PCF8563_setTime(hour,mintue,second);
//	//WriteAByte(CTRL_BUF2,0x12);			//alarm enable
//	//WriteAByte(HOUR_AE_BUF,0x09);		//set alarm hour at 9:00
//	//WriteAByte(CLK_FRQ_BUF,0xf0);		//set clkout frequency
//}

#endif /*_PCF8563_H_*/
