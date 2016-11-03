#include "ee_memory.h"
#include "bsp.h"
#include "common.h"


uint8_t Device_INFO[INFO_SIZE];
uint8_t Device_SN[DSN_SIZE];
uint8_t Device_PID[PID_SIZE];
uint8_t Device_HW_REV[HW_REV_SIZE];
uint8_t Device_FW_REV[FW_REV_SIZE];
uint8_t Device_CTL_REV[CTL_REG_SIZE];


extern uint8_t Device_SERVER_IP[];
extern uint8_t Device_SERVER_PORT[];
extern uint8_t Device_WIFI_SSID[];
extern uint8_t Device_WIFI_PSWD[];

uint8_t Write_Block_Index;
uint8_t Read_Block_Index;


uint8_t Data[2]={0};

uint8_t Memory_Init(void)
{
	EE_Init(BSP_I2C);
	return 1;
}	

uint8_t Memory_ReadInfo()
{
	uint16_t NumDataRead = 0;
	
	NumDataRead = INFO_SIZE;
	/* Read from I2C EEPROM from EE_READ_ADDRESS1 */
	EE_ReadBuffer(Device_INFO, INFO_ADDRESS, (uint16_t *)(&NumDataRead));
	/* Wait for EEPROM standby state */
	EE_WaitEepromStandbyState(); 
	
	BufferOffsetCpy(Device_SN, Device_INFO,0, DSN_ADDRESS, DSN_SIZE);
	BufferOffsetCpy(Device_PID, Device_INFO,0, PID_ADDRESS, PID_SIZE);
	BufferOffsetCpy(Device_HW_REV, Device_INFO,0, HW_REV_ADDRESS, HW_REV_SIZE);
	BufferOffsetCpy(Device_FW_REV, Device_INFO,0, FW_REV_ADDRESS, FW_REV_SIZE);
	BufferOffsetCpy(Device_CTL_REV, Device_INFO,0, CTL_REG_ADDRESS, CTL_REG_SIZE);
	
	BufferOffsetCpy(Device_SERVER_IP, Device_INFO, 0, SERVER_IP_ADDRESS,SERVER_IP_SIZE);
	BufferOffsetCpy(Device_SERVER_PORT, Device_INFO, 0, SERVER_PORT_ADDRESS,SERVER_PORT_SIZE);
	
	BufferOffsetCpy(Device_WIFI_SSID, Device_INFO, 0, SSID_ADDRESS,SSID_SIZE);
	BufferOffsetCpy(Device_WIFI_PSWD, Device_INFO, 0, PSWD_ADDRESS,PSWD_SIZE);
		
	Write_Block_Index = Device_INFO[WB_INDEX_ADDRESS];
	Read_Block_Index = Device_INFO[RB_INDEX_ADDRESS];	
	
	return 1;
}

uint8_t Memory_Write_Contrl_Reg(uint8_t regNum, uint8_t bit, FlagStatus s)
{
	uint8_t temp = Device_CTL_REV[regNum];
	if(s==SET)
		temp = temp|bit;
	else
		temp = temp&(0xFF-bit);
	
	Device_CTL_REV[regNum] =temp ;
	
	Memory_Write_Buffer(Device_CTL_REV, CTL_REG_ADDRESS, 2);
	return SUCCESS;
}
uint8_t Memory_Read_Contrl_Reg(uint8_t regNum, uint8_t bit)
{

    return Device_CTL_REV[regNum]&bit;

}


uint8_t Memory_Read_Buffer(uint8_t* buf, uint8_t addr, uint8_t len)
{
	uint16_t NumDataRead = len;
	EE_ReadBuffer(buf, addr, (uint16_t *)(&NumDataRead)); 
	/* Wait for EEPROM standby state */
	EE_WaitEepromStandbyState(); 
	return SUCCESS;
}

uint8_t Memory_Write_Buffer(uint8_t* buf, uint8_t addr, uint8_t len)
{
	EE_WriteBuffer(buf, addr, len); 
	/* Wait for EEPROM standby state */
	EE_WaitEepromStandbyState(); 
	return 1;
}

uint8_t Memory_Write_Block(uint8_t* bData)
{
	printf("write block %d \r\n",Write_Block_Index);
	
	if(Write_Block_Index==Read_Block_Index)
	{
		printf("catch up ...");
	if(Read_Block_Index==BLOCK_NUM)
		Read_Block_Index =0;
	else
		Read_Block_Index++;
	
		Data[0] = Read_Block_Index;

		EE_WriteBuffer(Data, RB_INDEX_ADDRESS, RB_INDEX_SIZE); 
		/* Wait for EEPROM standby state */
		EE_WaitEepromStandbyState(); 	
	
	}
	EE_WriteBuffer(bData, BLOCK_START_ADDRESS+Write_Block_Index*BLOCK_SIZE,BLOCK_SIZE); 
	/* Wait for EEPROM standby state */
	EE_WaitEepromStandbyState(); 
	
	if(Write_Block_Index==BLOCK_NUM)
		Write_Block_Index =0;
	else
		Write_Block_Index++;
	
	Data[0] = Write_Block_Index;
	EE_WriteBuffer(Data, WB_INDEX_ADDRESS, WB_INDEX_SIZE); 
	/* Wait for EEPROM standby state */
	EE_WaitEepromStandbyState(); 
	

		
	
	return 0;
}

uint8_t Memory_Read_Block(uint8_t* bData)
{
	uint16_t NumDataRead = BLOCK_SIZE;
	if(Read_Block_Index == Write_Block_Index)
		return 2;

	printf("read block %d \r\n", Read_Block_Index);
	EE_ReadBuffer(bData, BLOCK_START_ADDRESS+Read_Block_Index*BLOCK_SIZE, (uint16_t *)(&NumDataRead)); 
	/* Wait for EEPROM standby state */
	EE_WaitEepromStandbyState();
	
	if(Read_Block_Index==BLOCK_NUM)
		Read_Block_Index =0;
	else
		Read_Block_Index++;
	
	Data[0] = Read_Block_Index;
	EE_WriteBuffer(Data, RB_INDEX_ADDRESS, RB_INDEX_SIZE); 
	/* Wait for EEPROM standby state */
	EE_WaitEepromStandbyState(); 	
	
	return 1;

}
