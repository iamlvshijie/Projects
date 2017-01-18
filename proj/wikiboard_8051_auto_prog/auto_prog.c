/***************************************************************
        作品：STC免手动烧写控制程序
  单片机：STC15F104E（A版）
    晶振：11.0592M
编译环境：Keil uVision4 V9.00
    
        注意：STC-ISP软件中的“最低波特率” 可选“1200bps/2400bps/4800bps”，
              本程序自动检测适应。
***************************************************************/
     
#include "STC15Fxxxx.H" 


#define uint8  unsigned char
#define uint16 unsigned int   
	
sbit RXB   = P3^4;
sbit Key   = P3^1;   //自动/手动开关（根据需要使用）
sbit Relay = P3^3;         //继电器
sbit LED   = P3^0;

uint8 RBUF;         //接收缓存
uint8 RDAT;         //接收数据暂存
uint8 RCNT;         //接收计数器
uint8 RBIT;         //接收比特数
bit   RING;         //开始接收标志
bit   REND;         //接收完成标志

uint8 cnt;         //重启计数器
bit   START; //重启标志

uint16 cn;          //波特率切换计数器
uint8  Status;//波特率状态
bit    NEW;          //波特率更新标志

void YS100ms(uint8 n)
{
  uint8 a,b,c;
  while(n--)
  {
     for(c=89;c>0;c--)
     for(b=230;b>0;b--)
     for(a=12;a>0;a--);
  }
}

void UART_INIT()
{
   RING = 0;
   REND = 0;
   RCNT = 0;

   cnt = 0;
   cn  = 0;
}

void main()
{
   P3M1 = 0x00;
   P3M0 = 0x09;                 //P3.0、P3.3推挽输出
   Key        = 1;
   Relay= 0;                 //断开继电器
   LED  = 0;                 //关LED

   
   TMOD = 0x00;      //T0处于16位自动重装模式
   AUXR = 0x80;      //T0工作在1T模式
   TL0 = 0x00;                 //初始化Timer0和设定重载值
   TH0 = 0xFA;       //默认 2400bps @ 11.0592MHz
   TR0 = 1;          
   ET0 = 1;          
   PT0 = 1;          //提高Timer0的中断优先级
   EA  = 1;           

   UART_INIT();

   while(1)
   {                                   
      if(START)
          {
                 START = 0;
                 if(Key)
                 {
                    ET0 = 0;
                    Relay = 1;                 //继电器吸合
                    LED = 1;
                    YS100ms(50);         //从『加载HEX』到『提示上电』有5秒
                    Relay = 0;
                    LED = 0;
                    YS100ms(30);
                    UART_INIT();
                        ET0 = 1;
                 }
          }

          if(NEW)
          {
                 NEW = 0;
                 TR0 = 0; 
                 if(++Status > 2)Status=0;
                 switch(Status)
         {
                    case 0:TL0=0x00; TH0=0xFA;           // 2400bps @ 11.0592MHz
                                   //P31=0;P32=1;P35=1;          //状态指示灯（根据需要使用）
                                   break;
                        case 1:TL0=0x00; TH0=0xFD;           // 4800bps @ 11.0592MHz
                   //P31=1;P32=0;P35=1;          //状态指示灯（根据需要使用）
                                   break;
                        case 2:TL0=0x00; TH0=0xF4;          // 1200bps @ 11.0592MHz
                   //P31=1;P32=1;P35=0;          //状态指示灯（根据需要使用）
                                   break;
                 }
                 UART_INIT();
                 TR0 = 1; 
          }
   }
}

void tm0() interrupt 1
{
   if(RING)
   {
      if(--RCNT == 0)
      {
         RCNT = 3;             //复位接收波特率计数器
         if(--RBIT == 0)
         {
            RBUF = RDAT;       //保存数据到RBUF
            RING = 0;          //停止接收
            REND = 1;          //接收完成标志
         }
         else
         {
            RDAT >>= 1;
            if(RXB)RDAT |= 0x80; //RX数据转移到RX缓冲区
         }
      }
   }
   else if(!RXB)                           //是否检测到低电平
   {
      RING = 1;                //设置开始接收标志
      RCNT = 4;                //初始接收波特率计数器
      RBIT = 9;                //初化始接收比特数（8个数据位+1个停止位）
   }

   if(REND)
   {
      REND = 0;
      if(RBUF == 0x7F)
          { 
             cnt++;
             if(cnt > 1)cn=0;           //“0x7F”连续 > 1，则清切换计数器
                 if(cnt > 100)START=1; //“0x7F”连续 > 100，则打开重启标志
          } 
          else cnt = 0;
   }

   if(++cn>600){ cn=0; NEW=1; }//切换计数器 > 600，则打开更新标志
}