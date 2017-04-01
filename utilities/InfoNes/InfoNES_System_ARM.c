#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "K6502.h"
#include "InfoNES.h"
#include "InfoNES_System.h"
#include "InfoNES_Mapper.h"
#include "InfoNES_pAPU.h"
#include "main.h"
#include "flash.h"
#include "malloc.h"
#include "cs42L52.h"


//.nes File Header
NesHeader *Neshd;
NesResource *NES;
ApuResource *APU;
CpuResource *CPU;
ApuEvent *ApuEventQueue;
Mapper1Res *MAP1;
Mapper4Res *MAP4;
Mapper6Res *MAP6;
Mapper21Res *MAP21;
Mapper23Res *MAP23;
Mapper33Res *MAP33;
Mapper118Res *MAP118;
MapperMISRes *MAPMIS;
Mapper1Res MYMAP1;


BYTE *RAM;//8K
BYTE *SRAM;//8K 
BYTE *PPURAM;//16K
BYTE *ChrBuf;//32K
BYTE *ROM;
BYTE *VROM;
BYTE *SRAMBANK;//SRAM BANK(8Kb)
//ROM BANK ( 8Kb*4 ) 
BYTE *ROMBANK0;
BYTE *ROMBANK1;
BYTE *ROMBANK2;
BYTE *ROMBANK3;
//1Kb*16
BYTE *PPUBANK[ 16 ];

WORD *wave_buffers;
WORD *Abuf1;
WORD *Abuf2;
__IO u8 TainsferCplt;
__IO u8 whichbuf;


extern void InfoNES_Main(void);
void NES_Main(void)
{
    Neshd=mymalloc(SRAMIN1,sizeof(NesHeader));
    NES=mymalloc(SRAMIN1,sizeof(NesResource));
    CPU=mymalloc(SRAMIN1,sizeof(CpuResource));
    RAM=mymalloc(SRAMIN1,RAM_SIZE);
    SRAM=mymalloc(SRAMIN1,SRAM_SIZE);
    PPURAM=mymalloc(SRAMIN1,PPURAM_SIZE);
    ChrBuf=mymalloc(SRAMIN1,256*2*8*8);
    APU=mymalloc(SRAMIN1,sizeof(ApuResource));    
    ApuEventQueue=mymalloc(SRAMIN1,APU_EVENT_MAX*sizeof(ApuEvent));    
    wave_buffers=mymalloc(SRAMIN1,1470);
    Abuf1=mymalloc(SRAMIN1,1470);
    Abuf2=mymalloc(SRAMIN1,1470);    
    if(!NES||!RAM||!SRAM||!PPURAM||!ChrBuf||!APU||!ApuEventQueue||!wave_buffers||!Abuf1||!Abuf2)return;    
    if(0!=InfoNES_Load((char *)path_curr))
    {
		InfoNES_MessageBox("load rom failed!");
        myfree(SRAMIN1,Neshd);
		return;
	}
    LCD_Dispmd(6);
	LCD_Clear(BLACK);
	LCD_Window(0,0,119,159);
	LCD_Cursor(119,0);
	LCD_WR_REG(0x22);//写入LCDRAM的准备        
    InfoNES_Main();	
}

void My_task(void)
{
}

void InfoNES_Wait(void)
{
}  

int InfoNES_Menu(void)
{
	//0:Normally,-1:Exit InfoNES
  	return 0;
}

void *InfoNES_MemoryCopy( void *dest, const void *src, int count )
{
	memcpy( dest, src, count );
	return dest;
}

void *InfoNES_MemorySet( void *dest, int c, int count )
{
	memset( dest, c, count);  
	return dest;
}

void InfoNES_SoundInit( void ) 
{
	//Codec_Init(70);  
    TX_callback=GAME_TX_callback;
}

int InfoNES_SoundOpen( int samples_per_sync, int sample_rate ) 
{
	AUDIO_Init(sample_rate/2);
	Codec_Start();
    TainsferCplt=1;
    whichbuf=1;
	NES->APU_Mute=0;
    Audio_MAL_Play((u32)Abuf1,1470);
	return 1;
}

void InfoNES_SoundClose( void ) 
{
	Codec_Stop();
}

void GAME_TX_callback(void)
{
    if(whichbuf)
    {
        Audio_MAL_Play((u32)Abuf1,1470);
        whichbuf=0;
    }
    else
    {
        Audio_MAL_Play((u32)Abuf2,1470);
        whichbuf=1;
    }       
    TainsferCplt=1;
}

void InfoNES_SoundOutput(int samples, WORD *wave)
{	
 	int i;	

	while(!TainsferCplt);     
    TainsferCplt=0;
    if(whichbuf)for(i=0;i<735;i++)Abuf1[i]=wave[i]<<5;
    else for(i=0;i<735;i++)Abuf2[i]=wave[i]<<5;       	  
}

void InfoNES_MessageBox( char *pszMsg, ... )
{
	LCD_Clear(WHITE);
	LCD_String(10,10,(u8*)pszMsg,RED);
	delay_ms(500);
}

void InfoNES_ReleaseRom()
{

}

void InfoNES_PadState( unsigned long *pdwPad1, unsigned long *pdwPad2, unsigned long *pdwSystem )
{
	*pdwPad1=0;
	if(!KEYU)*pdwPad1|=0x10;//4
    if(!KEYD)*pdwPad1|=0x20;//5
    if(!KEYL)*pdwPad1|=0x40;//6
    if(!KEYR)*pdwPad1|=0x80;//7
	if(!KEY1)*pdwPad1|=0x04;//2
    if(!KEY2)*pdwPad1|=0x08;//3
    if(!KEY3)*pdwPad1|=0x02;//1
    if(!KEY4)*pdwPad1|=0x01;//0    
}

int InfoNES_ReadRom(const char *pszFileName)
{
    FIL  file;
    int  size; 
	int  res,nIdx;
	UINT br;

	res = f_open(&file,pszFileName,FA_READ) ;		
	if(FR_OK == res)
	{
		InfoNES_MessageBox("NES File Open OK!"); 
	}
	else
	{
		InfoNES_MessageBox("File Open Error!"); 	
		f_close(&file);
		return -1;
	}
	//Read ROM Header
	f_read(&file,Neshd,16,&br);   
	if ((Neshd->byID[0]!= 0x4e)||(Neshd->byID[1]!= 0x45)
		||(Neshd->byID[2]!= 0x53)||(Neshd->byID[3]!= 0x1a))return -1;  
    //获取Mapper号
    NES->MapperNo=Neshd->byInfo1>>4;
    for(nIdx=4;nIdx<8&&Neshd->byReserve[nIdx]==0;++nIdx );
    if(nIdx==8)NES->MapperNo|=(Neshd->byInfo2&0xf0);//Mapper Number is 8bits 
    LCD_String(10,30,"Mapper number:",RED);
    LCD_Num(120,30,NES->MapperNo,3,BLACK);
    delay_ms(500);
    switch(NES->MapperNo)
    {
        case 1:MAP1=mymalloc(SRAMIN1,sizeof(Mapper1Res));break;
        case 4:MAP4=mymalloc(SRAMIN1,sizeof(Mapper4Res));break;
        case 6:MAP6=mymalloc(SRAMIN1,sizeof(Mapper6Res));break;
        case 21:MAP21=mymalloc(SRAMIN1,sizeof(Mapper21Res));break; 
        case 23:MAP23=mymalloc(SRAMIN1,sizeof(Mapper23Res));break; 
        case 33:MAP33=mymalloc(SRAMIN1,sizeof(Mapper33Res));break; 
        case 118:MAP118=mymalloc(SRAMIN1,sizeof(Mapper118Res));break;      
        case 32:
        case 64:
        case 69:    
            MAPMIS=mymalloc(SRAMIN1,sizeof(MapperMISRes));break;    
        default:break;    
    }     
  	memset( SRAM, 0, SRAM_SIZE );
  	//If trainer presents Read Triner at 0x7000-0x71ff
  	if ( Neshd->byInfo1 & 4 )
  	{
	 	f_read(&file,&SRAM[0x1000],512,&br);
  	}
  	size = Neshd->byRomSize * 0x4000 ;
	//LCD_Num(10,30,size,8,BLUE);
    res=Write_ROM(&file,size);
    if(res)while(1);
    ROM=(BYTE*)ADDR_FLASH_SECTOR_8;
  	if(Neshd->byVRomSize>0)
  	{
    	size = Neshd->byVRomSize * 0x2000 ;
		//LCD_Num(10,44,size,8,BLUE);
        res=Write_VROM(&file,size);
        if(res)while(1);      
        VROM=(BYTE*)ADDR_FLASH_SECTOR_10;
  	}
  	return 0;
}




