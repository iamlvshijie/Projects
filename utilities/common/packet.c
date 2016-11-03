#include "packet.h"

Packet_TypeDef ReceivePacket={0};
Packet_TypeDef TransmitPacket={0};


//uint16_t offset = 0;

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
//uint8_t PacketParser(uint8_t* pkt)
//{

//}


//uint8_t PacketCheckSum(uint8_t * data, uint16_t length)
//{
//	uint8_t sum = 0;
//	for(uint16_t i=0;i<length,i++)
//		sum = sum ^ (*(data+i));
//	return sum;
//}


uint8_t Packet_Parser(uint8_t* pkt, uint16_t len, uint8_t* data)
{
  return 0;
}


/**
  * @brief  
  * @param  
						@arg type: packet type
  * @retval None
  */
uint8_t Pakcet_Create(uint8_t* data)
{

//	switch (type)
//	{
//		case LOGIN_REQUEST:
//			pkt_data[offset++] = 13;
//		
//			pkt_data[offset++] = LOGIN_REQUEST;
//		
//			pkt_data[offset++] = 0x12;
//			pkt_data[offset++] = 0x34;
//		
//			pkt_data[offset++] = 0x67;
//			pkt_data[offset++] = 0x89;
//		
//			pkt_data[offset++] = 0x00;
//			pkt_data[offset++] = 0x00;
//			pkt_data[offset++] = 0x00;
//			pkt_data[offset++] = 0x00;
//		
//		  pkt_data[offset++] = 0x00;
//			pkt_data[offset++] = 0x00;
//		
//			pkt_data[offset++] = 0xFF;
//		
//		//	pkt_data[offset] = PacketCheckSum(pkt_data, offset);
//			
//			break;
//		case	QUIT_REQUEST:
//			break;
//		case SENSOR_DATA:
//			pkt_data[offset++] = 18;
//			pkt_data[offset++] = SENSOR_DATA;
//			pkt_data[offset++] = 
//			
//			break;
//		default:
//			break;
//	}
 return 0;
}






