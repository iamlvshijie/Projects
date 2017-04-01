#include "avifile.h"
#include <stdio.h>
#include <string.h>

AVI_TypeDef AVI_file;
avih_TypeDef* avihChunk;
strh_TypeDef* strhChunk;
MYBITMAPINFO* bmpinfo;
MYWAVEFORMAT* wavinfo;
u32 temp=0x00;
u8 vids_ID;
u8 auds_ID;

u8 AVI_Parser(u8 *buffer)
{
	temp=ReadUnit(buffer,0,4,1);//读"RIFF"
	if(temp!=RIFF_ID)return 1;
	AVI_file.RIFFchunksize=ReadUnit(buffer,4,4,1);//RIFF数据块长度
	temp=ReadUnit(buffer,8,4,1);//读"AVI "
	if(temp!=AVI_ID)return 2;
	temp=ReadUnit(buffer,12,4,1);//读"LIST"
	if(temp!=LIST_ID)return 3;
	AVI_file.LISTchunksize=ReadUnit(buffer,16,4,1);//LIST数据块长度
	temp=ReadUnit(buffer,20,4,1);//读"hdrl"
	if(temp!=hdrl_ID)return 4;
	temp=ReadUnit(buffer,24,4,1);//读"avih"
	if(temp!=avih_ID)return 5;
	AVI_file.avihsize=ReadUnit(buffer,28,4,1);//avih数据块长度	
	return 0;				
}

u8 Avih_Parser(u8 *buffer)
{
	avihChunk=(avih_TypeDef*)buffer;
	if((avihChunk->Width>320)||(avihChunk->Height>240))return 1;//视频尺寸不支持
	if(avihChunk->Streams!=2)return 2;//视频流数不支持
	return 0;
}

u8 Strl_Parser(u8 *buffer)
{
	temp=ReadUnit(buffer,0,4,1);//读"LIST"
	if(temp!=LIST_ID)return 1;
	AVI_file.strlsize=ReadUnit(buffer,4,4,1);//strl数据块长度
	temp=ReadUnit(buffer,8,4,1);//读"strl"
	if(temp!=strl_ID)return 2;
	temp=ReadUnit(buffer,12,4,1);//读"strh"
	if(temp!=strh_ID)return 3;
	AVI_file.strhsize=ReadUnit(buffer,16,4,1);//strh数据块长度
	strhChunk=(strh_TypeDef*)(buffer+20);		 //108
	if(strhChunk->Handler[0]!='H')return 4;
	return 0;
}

u8 Strf_Parser(u8 *buffer)
{
	temp=ReadUnit(buffer,0,4,1);//读"strf"
	if(temp!=strf_ID)return 1;
	if(strhChunk->StreamType[0]=='v')//第一个流为视频流
	{
		vids_ID='0';
		auds_ID='1';
		bmpinfo=(MYBITMAPINFO*)(buffer+8);
		wavinfo=(MYWAVEFORMAT*)(buffer+4332);
	}
	else if(strhChunk->StreamType[0]=='a')//第一个流为音频流
	{
		vids_ID='1';
		auds_ID='0';
		wavinfo=(MYWAVEFORMAT*)(buffer+8);
		bmpinfo=(MYBITMAPINFO*)(buffer+4332);
	}
	return 0;
}

u16 Search_Movi(u8* buffer)
{
	u16 i;
	for(i=0;i<20480;i++)
	{
	   	if(buffer[i]==0x6d)
			if(buffer[i+1]==0x6f)
				if(buffer[i+2]==0x76)	
					if(buffer[i+3]==0x69)return i;//找到"movi"	
	}
	return 0;		
}

u16 Search_Fram(u8* buffer)
{
	u16 i;
	for(i=0;i<20480;i++)
	{
	   	if(buffer[i]=='0')
			if(buffer[i+1]==vids_ID)
				if(buffer[i+2]=='d')	
					if(buffer[i+3]=='c')return i;//找到"xxdc"	
	}
	return 0;		
}

u32 ReadUnit(u8 *buffer,u8 index,u8 Bytes,u8 Format)//1:大端模式;0:小端模式
{
  	signed char off=0;
  	u32 unit=0;  
  	
  	if(Format)for(off=0;off<Bytes;off++)unit|=buffer[off+index]<<((Bytes-off-1)*8);
	else for(off=0;off<Bytes;off++)unit|=buffer[off+index]<<(off*8);
  	return unit;
}


