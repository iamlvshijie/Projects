#include "diskio.h"
#include "mmc_sd.h"

//��ʼ������
DSTATUS disk_initialize (BYTE drv)
{
 	switch(drv) 
 	{
		case 0://SD��	  
    		if(SD_Init())return STA_NOINIT;
    		else return 0;	
		case 1:
	  		return STA_NOINIT;
		case 2:
			return STA_NOINIT;
  	}
	return STA_NOINIT;
}
//���ش���״̬
DSTATUS disk_status (BYTE drv)		
{
	switch(drv) 
	{
	  	case 0:		
	    	return 0;
	  	case 1:
	    	return 0;
	  	case 2:
	    	return 0;
	  	default:
        	break;
	}
	return STA_NOINIT;
}
//������
DRESULT disk_read (BYTE drv,BYTE *buff,DWORD sector,BYTE count)
{
  	u8 Status;

	switch(drv) 
	{
	  	case 0://SD��
		  	if(count==1)Status=SD_ReadSingleBlock(sector,buff);                                                              
		  	else Status=SD_ReadMultiBlock(sector,buff,count);				
		  	if(Status)return RES_ERROR;
		  	else return RES_OK;
	  	case 1://U��
		  	return RES_ERROR;
	  	case 2:
	    	return RES_PARERR;
	  	default:
        	break;
	}
	return RES_ERROR; 
}
//д����
#if _READONLY==0
DRESULT disk_write(BYTE drv,const BYTE *buff,DWORD sector,BYTE count)
{
  	u8 Status;

	switch(drv) 
	{
	  	case 0://SD��		
		  	if(count==1)Status=SD_WriteSingleBlock(sector,buff);                                                              
		  	else Status=SD_WriteMultiBlock(sector,buff,count);				
		  	if(Status)return RES_ERROR;
		  	else return RES_OK;
	  	case 1:
		    return RES_ERROR;
	  	case 2:
	    	return RES_PARERR;
	  	default:
        	break;
	} 
	return RES_ERROR;   	                                              
}
#endif /* _READONLY */

//��������
DRESULT disk_ioctl(BYTE drv,BYTE ctrl,void *buff)
{
	if(drv==0)//SD��
	{
		switch(ctrl) 
		{
		  	case CTRL_SYNC:	      
				return RES_OK;
		  	case GET_SECTOR_COUNT: 
		    	*(DWORD*)buff=CardCapacity/512;
		    	return RES_OK;
		  	case GET_BLOCK_SIZE:
		    	*(WORD*)buff = 512;
		    	return RES_OK;	
		  	case CTRL_POWER:
				break;
		  	case CTRL_LOCK:
				break;
		  	case CTRL_EJECT:
				break;
		  	case MMC_GET_TYPE:
				break;
		  	case MMC_GET_CSD:
				break;
		  	case MMC_GET_CID:
				break;
		  	case MMC_GET_OCR:
				break;
		  	case MMC_GET_SDSTAT:
				break;	
		}
	}
	return RES_PARERR;   
}

/* �õ��ļ�Calendar��ʽ�Ľ�������,��DWORD get_fattime (void) ��任 */							
/*-----------------------------------------------------------------------*/
/* User defined function to give a current time to fatfs module          */
/* 31-25: Year(0-127 org.1980), 24-21: Month(1-12), 20-16: Day(1-31) */                                                                                                                                                                                                                                          
/* 15-11: Hour(0-23), 10-5: Minute(0-59), 4-0: Second(0-29 *2) */                                                                                                                                                                                                                                                
DWORD get_fattime (void)
{  
    return 0;
}


