

#include "oled.h"
#include "stdlib.h"
#include "oledfont.h"  	
 
//#include "delay.h"
//OLED的显存
//存放格式如下.
//[0]0 1 2 3 ... 127	
//[1]0 1 2 3 ... 127	
//[2]0 1 2 3 ... 127	
//[3]0 1 2 3 ... 127	
//[4]0 1 2 3 ... 127	
//[5]0 1 2 3 ... 127	
//[6]0 1 2 3 ... 127	
//[7]0 1 2 3 ... 127 			   




void IIC_Start()
{

	OLED_SCLK_Set() ;
	OLED_SDIN_Set();
	OLED_SDIN_Clr();
	OLED_SCLK_Clr();
}

/**********************************************
//IIC Stop
**********************************************/
void IIC_Stop()
{
OLED_SCLK_Set() ;
//	OLED_SCLK_Clr();
	OLED_SDIN_Clr();
	OLED_SDIN_Set();
	
}

void IIC_Wait_Ack()
{

	//GPIOB->CRH &= 0XFFF0FFFF;	//设置PB12为上拉输入模式
	//GPIOB->CRH |= 0x00080000;
//	OLED_SDA = 1;
//	delay_us(1);
	//OLED_SCL = 1;
	//delay_us(50000);
/*	while(1)
	{
		if(!OLED_SDA)				//判断是否接收到OLED 应答信号
		{
			//GPIOB->CRH &= 0XFFF0FFFF;	//设置PB12为通用推免输出模式
			//GPIOB->CRH |= 0x00030000;
			return;
		}
	}
*/
	OLED_SCLK_Set() ;
	OLED_SCLK_Clr();
}
/**********************************************
// IIC Write byte
**********************************************/

void Write_IIC_Byte(u8_t IIC_Byte)
{
	u8_t i;
	u8_t m,da;
	da=IIC_Byte;
	OLED_SCLK_Clr();
	for(i=0;i<8;i++)		
	{
			m=da;
		//	OLED_SCLK_Clr();
		m=m&0x80;
		if(m==0x80)
		{OLED_SDIN_Set();}
		else OLED_SDIN_Clr();
			da=da<<1;
		OLED_SCLK_Set();
		OLED_SCLK_Clr();
		}


}
/**********************************************
// IIC Write Command
**********************************************/
void Write_IIC_Command(u8_t IIC_Command)
{
   IIC_Start();
   Write_IIC_Byte(0x78);            //Slave address,SA0=0
	IIC_Wait_Ack();	
   Write_IIC_Byte(0x00);			//write command
	IIC_Wait_Ack();	
   Write_IIC_Byte(IIC_Command); 
	IIC_Wait_Ack();	
   IIC_Stop();
}
/**********************************************
// IIC Write Data
**********************************************/
void Write_IIC_Data(u8_t IIC_Data)
{
   IIC_Start();
   Write_IIC_Byte(0x78);			//D/C#=0; R/W#=0
	IIC_Wait_Ack();	
   Write_IIC_Byte(0x40);			//write data
	IIC_Wait_Ack();	
   Write_IIC_Byte(IIC_Data);
	IIC_Wait_Ack();	
   IIC_Stop();
}


//SSD1306	
/**
  * @brief  oled show string
  * @param  (x,y), no
  * @retval None
  */				    
void oled_init(void)
{ 	
	
 	GPIO_InitTypeDef  GPIO_InitStructure;
 	

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);	 //使能A端口时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 		 //推挽输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_1;//速度50MHz
 	GPIO_Init(GPIOA, &GPIO_InitStructure);	  //初始化GPIOD3,6
 	GPIO_SetBits(GPIOA,GPIO_Pin_6|GPIO_Pin_7);	


	Delay_50ms(16);
	oled_send_dc(0xAE,OLED_CMD);//--display off
	oled_send_dc(0x00,OLED_CMD);//---set low column address
	oled_send_dc(0x10,OLED_CMD);//---set high column address
	oled_send_dc(0x40,OLED_CMD);//--set start line address  
	oled_send_dc(0xB0,OLED_CMD);//--set page address
	oled_send_dc(0x81,OLED_CMD); // contract control
	oled_send_dc(0xFF,OLED_CMD);//--128   
	oled_send_dc(0xA1,OLED_CMD);//set segment remap 
	oled_send_dc(0xA6,OLED_CMD);//--normal / reverse
	oled_send_dc(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	oled_send_dc(0x3F,OLED_CMD);//--1/32 duty
	oled_send_dc(0xC8,OLED_CMD);//Com scan direction
	oled_send_dc(0xD3,OLED_CMD);//-set display offset
	oled_send_dc(0x00,OLED_CMD);//
	
	oled_send_dc(0xD5,OLED_CMD);//set osc division
	oled_send_dc(0x80,OLED_CMD);//
	
	oled_send_dc(0xD8,OLED_CMD);//set area color mode off
	oled_send_dc(0x05,OLED_CMD);//
	
	oled_send_dc(0xD9,OLED_CMD);//Set Pre-Charge Period
	oled_send_dc(0xF1,OLED_CMD);//
	
	oled_send_dc(0xDA,OLED_CMD);//set com pin configuartion
	oled_send_dc(0x12,OLED_CMD);//
	
	oled_send_dc(0xDB,OLED_CMD);//set Vcomh
	oled_send_dc(0x30,OLED_CMD);//
	
	oled_send_dc(0x8D,OLED_CMD);//set charge pump enable
	oled_send_dc(0x14,OLED_CMD);//
	
	oled_send_dc(0xAF,OLED_CMD);//--turn on oled panel
}  

/**
  * @brief  oled show string
  * @param  (x,y), no
  * @retval None
  */
void oled_send_dc(unsigned dat,unsigned cmd)
{
	if(cmd)
			{

   Write_IIC_Data(dat);
   
		}
	else {
   Write_IIC_Command(dat);	
	}
}

/**
  * @brief  oled show string
  * @param  (x,y), no
  * @retval None
  */
/********************************************
// fill_Picture
********************************************/
void oled_fill_picture(u8_t fill_Data)
{
	u8_t m,n;
	for(m=0;m<8;m++)
	{
		oled_send_dc(0xb0+m,0);		//page0-page1
		oled_send_dc(0x00,0);		//low column start address
		oled_send_dc(0x10,0);		//high column start address
		for(n=0;n<128;n++)
			{
				oled_send_dc(fill_Data,1);
			}
	}
}


/***********************Delay****************************************/
void Delay_50ms(unsigned int Del_50ms)
{
	unsigned int m;
	for(;Del_50ms>0;Del_50ms--)
		for(m=6245;m>0;m--);
}

void Delay_1ms(unsigned int Del_1ms)
{
	u8_t j;
	while(Del_1ms--)
	{	
		for(j=0;j<123;j++);
	}
}

//坐标设置
/**
  * @brief  oled show string
  * @param  (x,y), no
  * @retval None
  */
void oled_set_pos(u8_t x, u8_t y) 
{ 	oled_send_dc(0xb0+y,OLED_CMD);
	oled_send_dc(((x&0xf0)>>4)|0x10,OLED_CMD);
	oled_send_dc((x&0x0f),OLED_CMD); 
}   	  

/**
  * @brief  oled show string
  * @param  (x,y), no
  * @retval None
  */
//开启OLED显示    
void oled_display_ctl_on(void)
{
	oled_send_dc(0X8D,OLED_CMD);  //SET DCDC命令
	oled_send_dc(0X14,OLED_CMD);  //DCDC ON
	oled_send_dc(0XAF,OLED_CMD);  //DISPLAY ON
}

/**
  * @brief  oled show string
  * @param  (x,y), no
  * @retval None
  */
//关闭OLED显示     
void oled_display_ctl_off(void)
{
	oled_send_dc(0X8D,OLED_CMD);  //SET DCDC命令
	oled_send_dc(0X10,OLED_CMD);  //DCDC OFF
	oled_send_dc(0XAE,OLED_CMD);  //DISPLAY OFF
}		   		
	 
/**
  * @brief  oled show string
  * @param  (x,y), no
  * @retval None
  */	 
//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!	  
void oled_display_ctl_clear(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		oled_send_dc (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		oled_send_dc (0x00,OLED_CMD);      //设置显示位置—列低地址
		oled_send_dc (0x10,OLED_CMD);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)oled_send_dc(0,OLED_DATA); 
	} //更新显示
}

/**
  * @brief  oled show string
  * @param  (x,y), no
  * @retval None
  */
void oled_on(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		oled_send_dc (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		oled_send_dc (0x00,OLED_CMD);      //设置显示位置—列低地址
		oled_send_dc (0x10,OLED_CMD);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)oled_send_dc(1,OLED_DATA); 
	} //更新显示
}

/**
  * @brief  oled show string
  * @param  (x,y), no
  * @retval None
  */
//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示				 
//size:选择字体 16/12 
void oled_show_char(u8 x,u8 y,u8 chr,u8 Char_Size)
{      	
	u8_t c=0,i=0;	
		c=chr-' ';//得到偏移后的值			
		if(x>Max_Column-1){x=0;y=y+2;}
		if(Char_Size ==16)
			{
			oled_set_pos(x,y);	
			for(i=0;i<8;i++)
			oled_send_dc(F8X16[c*16+i],OLED_DATA);
			oled_set_pos(x,y+1);
			for(i=0;i<8;i++)
			oled_send_dc(F8X16[c*16+i+8],OLED_DATA);
			}
			else {	
				oled_set_pos(x,y);
				for(i=0;i<6;i++)
				oled_send_dc(F6x8[c][i],OLED_DATA);
				
			}
}

/**
  * @brief  oled show string
  * @param  (x,y), no
  * @retval None
  */
//m^n函数
u32 oled_pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}		
		  
/**
  * @brief  oled show num
  * @param  (x,y), no
  * @retval None
  */		  
//显示2个数字
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);	 		  
void oled_show_num(u8 x,u8 y,u32 num,u8 len,u8 size2)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				oled_show_char(x+(size2/2)*t,y,' ',size2);
				continue;
			}else enshow=1; 
		 	 
		}
	 	oled_show_char(x+(size2/2)*t,y,temp+'0',size2); 
	}
} 

/**
  * @brief  oled show string
  * @param  (x,y), no
  * @retval None
  */
void oled_show_string(u8 x,u8 y,u8 *chr,u8 size)
{
	u8_t j=0;
	while (chr[j]!='\0')
	{		
		oled_show_char(x,y,chr[j],size);
		x+=8;
		if(x>120){x=0;y+=2;}
			j++;
	}
}

/**
  * @brief  oled show chinese
  * @param  (x,y), no
  * @retval None
  */
void oled_show_chinese(u8 x,u8 y,u8 no)
{      			    
	u8 t,adder=0;
	oled_set_pos(x,y);	
    for(t=0;t<16;t++)
	{
		oled_send_dc(Hzk[2*no][t],OLED_DATA);
		adder+=1;
     }	
		oled_set_pos(x,y+1);	
    for(t=0;t<16;t++)
	{	
		oled_send_dc(Hzk[2*no+1][t],OLED_DATA);
		adder+=1;
     }					
}


/**
  * @brief  oled show bmp
  * @param  (x0,y0)->(x1,y1), x:0~127, y:0~7
  * @retval None
  */
void oled_show_bmp(u8_t x0, u8_t y0,u8_t x1, u8_t y1,u8_t bmp[])
{ 	
	unsigned int j=0;
	u8_t x,y;
  
	if(y1%8==0) 
		y=y1/8;      
	else 
		y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		oled_set_pos(x0,y);
		for(x=x0;x<x1;x++)
	    {      
	    	oled_send_dc(bmp[j++],OLED_DATA);	    	
	    }
	}
} 































