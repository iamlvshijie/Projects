#include <stdlib.h>
#include "h264player.h"
#include "dsputil.h"
#include "h264.h"
#include "lcd.h"
#include "led.h"
#include "avifile.h"
#include "malloc.h"
#include "fsearch.h"
#include "cs42L52.h"
#include "mp3dec.h"
#include "mp3common.h"

#define  Vidio_size     10*1024
#define  Audio_size     2304



short *Sound_buf1;
short *Sound_buf2;
short *Decbuf;
__IO u8  XferCplt;
__IO u8  audstop;
__IO u8  buf_switch;
__IO u32 Audsize;

extern void Convert_Stereo(short *buffer);
int H264_player(void)
{
    static HMP3Decoder hMP3Decoder;
    static AVCodecContext c;
    static AVFrame pic;
    static FIL inpf;
	int i,j,YY,CB,CR,temp;  
    u16 *temp1,*temp2,*temp3;
 	u8  *yy,*cb,*cr,res;   
    u32 Strsize,mid;
    u16 Strtype,color; 
    u8  *Frame_buf;   
    u8  *Dbuf,*Abuf;    
    UINT br;

	res=f_open(&inpf,(TCHAR*)path_curr,FA_READ);
	if(res)return 1;				//文件打开失败
    if(avcodec_open(&c)<0)return 3;	//解码器初始化失败 	
    //申请流缓冲      
    Frame_buf=mymalloc(SRAMIN1,Vidio_size);   
    Sound_buf1=mymalloc(SRAMIN1,Audio_size*2);
    Sound_buf2=mymalloc(SRAMIN1,Audio_size*2);
    temp1=mymalloc(SRAMIN2,320);
    temp2=mymalloc(SRAMIN2,320);
    temp3=mymalloc(SRAMIN2,320);
    if(!Frame_buf)return 5;
    if(!Sound_buf1||!Sound_buf2)return 6;
    if(!temp1||!temp2||!temp3)return 7;
    
    Dbuf=Frame_buf;    
    //解析文件头
	f_read(&inpf,Dbuf,10*1024,&br);
	res=AVI_Parser(Dbuf);			//解析AVI文件格式	
	if(res)return 8;
	res=Avih_Parser(Dbuf+32);		//解析avih数据块
	if(res)return 9;
	res=Strl_Parser(Dbuf+88);		//解析strh数据块
	if(res)return 10;
	res=Strf_Parser(Dbuf+164);		//解析strf数据块
	if(res)return 11;
	mid=Search_Movi(Dbuf);			//寻找movi ID		
	if(!mid)return 12;
	Strtype=MAKESWORD(Dbuf+mid+6);//流类型
	Strsize=MAKEDWORD(Dbuf+mid+8);//流大小
	if(Strsize%2)Strsize++;//奇数加1		
	f_lseek(&inpf,mid+12);//跳过标志ID
    //LCD刷屏设置
	LCD_Dispmd(6);
    //LCD_Window(0,0,119,159);
	LCD_Cursor(0,0);
	LCD_WR_REG(0x22);
    //初始化各全局变量           
    audstop=0;
    XferCplt=1;
    buf_switch=0;
    Decbuf=Sound_buf2;       
    //申请MP3解码缓冲及变量  
    hMP3Decoder=MP3InitDecoder();
    //初始化音频接口 
    TX_callback=VIDIO_TX_callback;
	AUDIO_Init(I2S_AudioFreq_16k);  
    //播放循环
	while(!f_eof(&inpf))
	{	
		if(Strtype==0x6463)//显示帧
		{
			f_read(&inpf,Dbuf,Strsize+8,&br);
			decode_frame(&c,&pic,&temp,Dbuf,Strsize); 
			if(temp) 	
			{
				yy=pic.data[0];
				cb=pic.data[1];
				cr=pic.data[2];
				for(j=0;j<60;j++)
				{
					for(i=0;i<80;i++)
					{
						YY=yy[2*i]-10;
						CB=cb[i]-128;
						CR=cr[i]-128;
						temp1[i]=(1436*CR)>>10;
						temp2[i]=(1815*CB)>>10;
						temp3[i]=(352*CB+731*CR)>>10;
						color=((BYTECLIP(YY+temp1[i])&0xF8)<<8);
						color|=((BYTECLIP(YY-temp3[i])&0xFc)<<3);
						color|=(BYTECLIP(YY+temp2[i])>>3);	
						LCD_WR_DATA(color);	
						YY=yy[2*i+1]-10;
						color=((BYTECLIP(YY+temp1[i])&0xF8)<<8);
						color|=((BYTECLIP(YY-temp3[i])&0xFc)<<3);
						color|=(BYTECLIP(YY+temp2[i])>>3);	
						LCD_WR_DATA(color);															
					}
					yy+=Ylinesize;
					for(i=0;i<80;i++)
					{
						YY=yy[2*i]-10;
						color=((BYTECLIP(YY+temp1[i])&0xF8)<<8);
						color|=((BYTECLIP(YY-temp3[i])&0xFc)<<3);
						color|=(BYTECLIP(YY+temp2[i])>>3);	
						LCD_WR_DATA(color);							
						YY=yy[2*i+1]-10;
						color=((BYTECLIP(YY+temp1[i])&0xF8)<<8);
						color|=((BYTECLIP(YY-temp3[i])&0xFc)<<3);
						color|=(BYTECLIP(YY+temp2[i])>>3);	
						LCD_WR_DATA(color);															
					}
					yy+=Ylinesize;
					cb+=UVlinesize;
					cr+=UVlinesize;
				}
                for(i=0;i<1280;i++)LCD_WR_DATA(BLACK);
			}            
            Strtype=MAKESWORD(Dbuf+Strsize+2);//流类型
            Strsize=MAKEDWORD(Dbuf+Strsize+4);//流大小									
            if(Strsize%2)Strsize++;//奇数加1            
		}
		else if(Strtype==0x7762)//播放音频流
		{		          
			f_read(&inpf,Dbuf,Strsize+8,&br);
            temp=Strsize;
            Abuf=Dbuf;
            Strtype=MAKESWORD(Dbuf+Strsize+2);//流类型
            Strsize=MAKEDWORD(Dbuf+Strsize+4);//流大小									
            if(Strsize%2)Strsize++;//奇数加1                        
            MP3Decode(hMP3Decoder,&Abuf,&temp,Decbuf,0);
            Convert_Stereo(Decbuf);
            while(XferCplt==0);
            XferCplt=0;							
            if(!buf_switch)
            {
                Decbuf=Sound_buf1;
                Audio_MAL_Play((u32)Sound_buf2,Audio_size);					        	
                buf_switch=1;
            }
            else 
            {   
                Decbuf=Sound_buf2;
                Audio_MAL_Play((u32)Sound_buf1,Audio_size);
                buf_switch=0;
            }                        	
		}
		else break;								   	
	}
    LCD_String(20,40,"Finished!",RED);
	f_close(&inpf);	
	avcodec_close(&c); 
	return 0;				
}

void VIDIO_TX_callback(void)
{    
	XferCplt=1;//DMA开始传送
}


