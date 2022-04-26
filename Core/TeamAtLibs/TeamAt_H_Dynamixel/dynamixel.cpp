/**
 * @file dynamixel.cpp
 * @brief
 * @version 1.0
 *
 * Version 1.01 2022-04-14
 * 	-Première version
 */

#include <TeamAt_H_Dynamixel/dynamixel.h>
#include "at_plaformAbstraction_V1_0.h"
#include "uartApp.h"
#include "TeamAT_B_Basic/teamATbasic_V1_1.h"



const uint8_t g_dynamixelHeader[4] = {0xff,0xff,0xfd,0x00};


unsigned short calculateDynamixelCRC(uint16_t crc_accum, uint8_t *data_blk_ptr, uint16_t data_blk_size)
{
  uint16_t i;
  static const uint16_t crc_table[256] = {0x0000,
  0x8005, 0x800F, 0x000A, 0x801B, 0x001E, 0x0014, 0x8011,
  0x8033, 0x0036, 0x003C, 0x8039, 0x0028, 0x802D, 0x8027,
  0x0022, 0x8063, 0x0066, 0x006C, 0x8069, 0x0078, 0x807D,
  0x8077, 0x0072, 0x0050, 0x8055, 0x805F, 0x005A, 0x804B,
  0x004E, 0x0044, 0x8041, 0x80C3, 0x00C6, 0x00CC, 0x80C9,
  0x00D8, 0x80DD, 0x80D7, 0x00D2, 0x00F0, 0x80F5, 0x80FF,
  0x00FA, 0x80EB, 0x00EE, 0x00E4, 0x80E1, 0x00A0, 0x80A5,
  0x80AF, 0x00AA, 0x80BB, 0x00BE, 0x00B4, 0x80B1, 0x8093,
  0x0096, 0x009C, 0x8099, 0x0088, 0x808D, 0x8087, 0x0082,
  0x8183, 0x0186, 0x018C, 0x8189, 0x0198, 0x819D, 0x8197,
  0x0192, 0x01B0, 0x81B5, 0x81BF, 0x01BA, 0x81AB, 0x01AE,
  0x01A4, 0x81A1, 0x01E0, 0x81E5, 0x81EF, 0x01EA, 0x81FB,
  0x01FE, 0x01F4, 0x81F1, 0x81D3, 0x01D6, 0x01DC, 0x81D9,
  0x01C8, 0x81CD, 0x81C7, 0x01C2, 0x0140, 0x8145, 0x814F,
  0x014A, 0x815B, 0x015E, 0x0154, 0x8151, 0x8173, 0x0176,
  0x017C, 0x8179, 0x0168, 0x816D, 0x8167, 0x0162, 0x8123,
  0x0126, 0x012C, 0x8129, 0x0138, 0x813D, 0x8137, 0x0132,
  0x0110, 0x8115, 0x811F, 0x011A, 0x810B, 0x010E, 0x0104,
  0x8101, 0x8303, 0x0306, 0x030C, 0x8309, 0x0318, 0x831D,
  0x8317, 0x0312, 0x0330, 0x8335, 0x833F, 0x033A, 0x832B,
  0x032E, 0x0324, 0x8321, 0x0360, 0x8365, 0x836F, 0x036A,
  0x837B, 0x037E, 0x0374, 0x8371, 0x8353, 0x0356, 0x035C,
  0x8359, 0x0348, 0x834D, 0x8347, 0x0342, 0x03C0, 0x83C5,
  0x83CF, 0x03CA, 0x83DB, 0x03DE, 0x03D4, 0x83D1, 0x83F3,
  0x03F6, 0x03FC, 0x83F9, 0x03E8, 0x83ED, 0x83E7, 0x03E2,
  0x83A3, 0x03A6, 0x03AC, 0x83A9, 0x03B8, 0x83BD, 0x83B7,
  0x03B2, 0x0390, 0x8395, 0x839F, 0x039A, 0x838B, 0x038E,
  0x0384, 0x8381, 0x0280, 0x8285, 0x828F, 0x028A, 0x829B,
  0x029E, 0x0294, 0x8291, 0x82B3, 0x02B6, 0x02BC, 0x82B9,
  0x02A8, 0x82AD, 0x82A7, 0x02A2, 0x82E3, 0x02E6, 0x02EC,
  0x82E9, 0x02F8, 0x82FD, 0x82F7, 0x02F2, 0x02D0, 0x82D5,
  0x82DF, 0x02DA, 0x82CB, 0x02CE, 0x02C4, 0x82C1, 0x8243,
  0x0246, 0x024C, 0x8249, 0x0258, 0x825D, 0x8257, 0x0252,
  0x0270, 0x8275, 0x827F, 0x027A, 0x826B, 0x026E, 0x0264,
  0x8261, 0x0220, 0x8225, 0x822F, 0x022A, 0x823B, 0x023E,
  0x0234, 0x8231, 0x8213, 0x0216, 0x021C, 0x8219, 0x0208,
  0x820D, 0x8207, 0x0202 };

  for (uint16_t j = 0; j < data_blk_size; j++)
  {
    i = ((uint16_t)(crc_accum >> 8) ^ *data_blk_ptr++) & 0xFF;
    crc_accum = (crc_accum << 8) ^ crc_table[i];
  }

  return crc_accum;
}


int findValidDynamixelHeader(uint8_t* data, int len)
{
	int i = 0;
	
	for (i = 0; i < (len - 3); i++)
	{
		if ((data[i] == 0xFF) && (data[i+1] == 0xFF) && (data[i+2] == 0xFD) && (data[i+3] != 0xFD))
		  {
			  return i; // Return the starting position of the header;
		  }
	}

	return -1; // Header not found

}


int findValidDynamixelPacket(uint8_t* data, int len)
{

	int i = 0;
	if(len < DYNAMIXEL_MIN_PACKET_SIZE)
	{
		return 0;	// if smaller than minimum pack, it cannot be valid
	}


	i = findValidDynamixelHeader(data,len);
	if(i < 0)
	{
		return 0; // Header not found
	}

	 if((i + DYNAMIXEL_MIN_PACKET_SIZE)  > len)
	 {
		 //Valid so far but still incomplete.
		 return 0;
	 }

     if (data[i + PKT_RESERVED] != 0x00 ||
    		 data[i+PKT_ID] > 0xFC ||
           DXL_MAKEWORD(data[i+PKT_LENGTH_L], data[i+PKT_LENGTH_H]) > RXPACKET_MAX_LEN ||
           data[i+PKT_INSTRUCTION] != 0x55)
        {
    	 	 return -1; // Packet not valid or corrupted.
        }

     // re-calculate the exact length of the rx packet
	 uint16_t packetSize = DXL_MAKEWORD(data[i+PKT_LENGTH_L], data[i+PKT_LENGTH_H]) + PKT_LENGTH_H + 1;

	 if (len < packetSize)
	 {
		 //Valid so far but still incomplete.
	     return 0; 
	 }

	 uint16_t crcReceived = DXL_MAKEWORD(data[i+packetSize-2], data[i+ packetSize-1]);


	if(calculateDynamixelCRC(0, &data[i], packetSize - 2) == crcReceived)
	{
		//packet valid
	}

	return 1;
}





int packageReadInstruction(uint8_t* targetBuffer, uint8_t id, uint16_t readStartAddress, uint16_t readLen)
{
	int writePtr = 0;
	int i;

	for (i = 0; i < PKT_HEADER_LEN; i++)
	{
		targetBuffer[writePtr] = g_dynamixelHeader[i];
		writePtr++;
	}

	targetBuffer[writePtr] = id;
	writePtr++;

	uint16_t messageLen = 7;
	targetBuffer[writePtr] = DXL_LOBYTE(messageLen);	//Message len
	writePtr++;
	targetBuffer[writePtr] = DXL_HIBYTE(messageLen);	//Message len
	writePtr++;
	targetBuffer[writePtr] = DYNAMIXEL_INST_READ;	//instruction
	writePtr++;
	targetBuffer[writePtr] = DXL_LOBYTE(readStartAddress);	//Read address
	writePtr++;
	targetBuffer[writePtr] = DXL_HIBYTE(readStartAddress);
	writePtr++;
	targetBuffer[writePtr] = DXL_LOBYTE(readLen);	//read length
	writePtr++;
	targetBuffer[writePtr] = DXL_HIBYTE(readLen);
	writePtr++;

	unsigned short crc = calculateDynamixelCRC(0, targetBuffer, writePtr);

	targetBuffer[writePtr] = DXL_LOBYTE(crc);
	writePtr++;
	targetBuffer[writePtr] = DXL_HIBYTE(crc);
	writePtr++;

	return writePtr;
}

int packageSyncReadInstruction(uint8_t* targetBuffer, uint8_t* idList,uint8_t idCount, uint16_t readStartAddress, uint16_t readLen)
{
	int writePtr = 0;
	int i;

	for (i = 0; i < PKT_HEADER_LEN; i++)
	{
		targetBuffer[writePtr] = g_dynamixelHeader[i];
		writePtr++;
	}

	targetBuffer[writePtr] = 0xFE;
	writePtr++;
	uint16_t messageLen = 7 + idCount;

	targetBuffer[writePtr] = DXL_LOBYTE(messageLen);
	writePtr++;
	targetBuffer[writePtr] = DXL_HIBYTE(messageLen);
	writePtr++;
	targetBuffer[writePtr] = DYNAMIXEL_INST_SYNC_READ;
	writePtr++;
	targetBuffer[writePtr] = DXL_LOBYTE(readStartAddress);
	writePtr++;
	targetBuffer[writePtr] = DXL_HIBYTE(readStartAddress);
	writePtr++;
	targetBuffer[writePtr] = DXL_LOBYTE(readLen);
	writePtr++;
	targetBuffer[writePtr] = DXL_HIBYTE(readLen);
	writePtr++;
	for (i = 0; i < idCount; ++i) {
		targetBuffer[writePtr] = idList[i];
		writePtr++;
	}

	unsigned short crc = calculateDynamixelCRC(0, targetBuffer, writePtr);

	targetBuffer[writePtr] = DXL_LOBYTE(crc);
	writePtr++;
	targetBuffer[writePtr] = DXL_HIBYTE(crc);
	writePtr++;

	return writePtr;
}

int packageFastSyncReadInstruction(uint8_t* targetBuffer, uint8_t* idList,uint8_t idCount, uint16_t readStartAddress, uint16_t readLen)
{
	int writePtr = 0;
	int i;

	for (i = 0; i < PKT_HEADER_LEN; i++)
	{
		targetBuffer[writePtr] = g_dynamixelHeader[i];
		writePtr++;
	}

	targetBuffer[writePtr] = 0xFE;
	writePtr++;
	uint16_t messageLen = 7 + idCount;

	targetBuffer[writePtr] = DXL_LOBYTE(messageLen);
	writePtr++;
	targetBuffer[writePtr] = DXL_HIBYTE(messageLen);
	writePtr++;
	targetBuffer[writePtr] = DYNAMIXEL_INST_FAST_SYNC_READ;
	writePtr++;
	targetBuffer[writePtr] = DXL_LOBYTE(readStartAddress);
	writePtr++;
	targetBuffer[writePtr] = DXL_HIBYTE(readStartAddress);
	writePtr++;
	targetBuffer[writePtr] = DXL_LOBYTE(readLen);
	writePtr++;
	targetBuffer[writePtr] = DXL_HIBYTE(readLen);
	writePtr++;
	for (i = 0; i < idCount; ++i) {
		targetBuffer[writePtr] = idList[i];
		writePtr++;
	}

	unsigned short crc = calculateDynamixelCRC(0, targetBuffer, writePtr);

	targetBuffer[writePtr] = DXL_LOBYTE(crc);
	writePtr++;
	targetBuffer[writePtr] = DXL_HIBYTE(crc);
	writePtr++;

	return writePtr;
}

int packageSyncWriteInstruction(uint8_t* targetBuffer, uint8_t* idList,uint8_t idCount, uint16_t writeStartAddress, uint16_t dataTypeLen, void* dataPtr)
{
	int writePtr = 0;
	int i;

	uint8_t* bytePtr = (uint8_t*)dataPtr;

	for (i = 0; i < PKT_HEADER_LEN; i++)
	{
		targetBuffer[writePtr] = g_dynamixelHeader[i];
		writePtr++;
	}

	targetBuffer[writePtr] = 0xFE;
	writePtr++;
	uint16_t messageLen = PKT_INSTRUCTION_LEN + 2 + 2  + (idCount * PKT_ID_LEN) + (idCount * dataTypeLen) + PKT_CRC_LEN; //TODO valider

	targetBuffer[writePtr] = DXL_LOBYTE(messageLen);
	writePtr++;
	targetBuffer[writePtr] = DXL_HIBYTE(messageLen);
	writePtr++;
	targetBuffer[writePtr] = DYNAMIXEL_INST_SYNC_WRITE;
	writePtr++;
	targetBuffer[writePtr] = DXL_LOBYTE(writeStartAddress);
	writePtr++;
	targetBuffer[writePtr] = DXL_HIBYTE(writeStartAddress);
	writePtr++;
	targetBuffer[writePtr] = DXL_LOBYTE(dataTypeLen);
	writePtr++;
	targetBuffer[writePtr] = DXL_HIBYTE(dataTypeLen);
	writePtr++;
	for (i = 0; i < idCount; ++i) {
		targetBuffer[writePtr] = idList[i];
		writePtr++;

		for(int j = 0; j < dataTypeLen; j++)
		{
			targetBuffer[writePtr] = bytePtr[ i *dataTypeLen + j];
			writePtr++;
		}
	}

	unsigned short crc = calculateDynamixelCRC(0, targetBuffer, writePtr);

	targetBuffer[writePtr] = DXL_LOBYTE(crc);
	writePtr++;
	targetBuffer[writePtr] = DXL_HIBYTE(crc);
	writePtr++;

	return writePtr;
}


int bulkWriteInstructionPackage_start(uint8_t* targetBuffer)
{
	int writePtr = 0;
	int i;


	for (i = 0; i < PKT_HEADER_LEN; i++)
	{
		targetBuffer[writePtr] = g_dynamixelHeader[i];
		writePtr++;
	}

	targetBuffer[writePtr] = 0xFE;
	writePtr++;
	uint16_t messageLen = PKT_INSTRUCTION_LEN;
	//*(uint16_t*)&dynamixelTxBuffer[writePtr] = instPacket.paramLen;
	targetBuffer[writePtr] = DXL_LOBYTE(messageLen);
	writePtr++;
	targetBuffer[writePtr] = DXL_HIBYTE(messageLen);
	writePtr++;
	targetBuffer[writePtr] = DYNAMIXEL_INST_BULK_WRITE;
	writePtr++;

	return	writePtr;

}

int bulkWriteInstructionPackage_AddData(uint8_t* targetBuffer, int writePtr, uint8_t id, uint16_t writeStartAddress, uint16_t dataTypeLen, void* dataPtr)
{

	uint8_t* bytePtr = (uint8_t*)dataPtr;

	uint16_t messageLen = DXL_MAKEWORD(targetBuffer[PKT_LENGTH_L], targetBuffer[PKT_LENGTH_L+1]);


	messageLen = messageLen + PKT_ID_LEN + PKT_ADDRESS_LEN + PKT_LENGHT_LEN + dataTypeLen;
	*(uint16_t*)&targetBuffer[PKT_LENGTH_L] = messageLen; // update length

	targetBuffer[writePtr] = id;
	writePtr++;
	targetBuffer[writePtr] = DXL_LOBYTE(writeStartAddress);
	writePtr++;
	targetBuffer[writePtr] = DXL_HIBYTE(writeStartAddress);
	writePtr++;
	targetBuffer[writePtr] = DXL_LOBYTE(dataTypeLen);
	writePtr++;
	targetBuffer[writePtr] = DXL_HIBYTE(dataTypeLen);
	writePtr++;


	for(int i = 0; i < dataTypeLen; i++)
	{
		targetBuffer[writePtr] = bytePtr[i];
		writePtr++;
	}


	return writePtr;
}

int bulkWriteInstructionPackage_complete(uint8_t* targetBuffer, int writePtr)
{

	uint16_t messageLen = DXL_MAKEWORD(targetBuffer[PKT_LENGTH_L], targetBuffer[PKT_LENGTH_L+1]);


	messageLen = messageLen + PKT_CRC_LEN;
	*(uint16_t*)&targetBuffer[PKT_LENGTH_L] = messageLen; // update length

	targetBuffer[PKT_LENGTH_L] = DXL_LOBYTE(messageLen);
	targetBuffer[PKT_LENGTH_L+1] = DXL_HIBYTE(messageLen);


	unsigned short crc = calculateDynamixelCRC(0, targetBuffer, writePtr);

	targetBuffer[writePtr] = DXL_LOBYTE(crc);
	writePtr++;
	targetBuffer[writePtr] = DXL_HIBYTE(crc);
	writePtr++;

	return writePtr;
}


bool validHeader(uint8_t* targetBuffer)
 {

     for (int i = 0; i < PKT_HEADER_LEN; i++)
     {
         if(targetBuffer[i]  != g_dynamixelHeader[i])
         {
             return false;
         }
     }

     return true;
 }


bool validStatusMessage(uint8_t* targetBuffer)
{

	if(findValidDynamixelHeader(targetBuffer,DYNAMIXEL_MIN_PACKET_SIZE) >= 0)
	 	{
	 		if(targetBuffer[PKT_INSTRUCTION] == DYNAMIXEL_INST_STATUS_RETURN)
	 		{
	 			uint16_t paramLen = DXL_MAKEWORD(targetBuffer[PKT_LENGTH_L], targetBuffer[PKT_LENGTH_H]);
	 			uint8_t error = targetBuffer[PKT_ERROR];

	 			if(0 == error)
	 			{
	 				int crcPos = PKT_INSTRUCTION + paramLen - PKT_CRC_LEN;
	 				uint16_t rxCrc = DXL_MAKEWORD(targetBuffer[crcPos], targetBuffer[crcPos+1]);

	 				uint16_t calcCrc = calculateDynamixelCRC(0, targetBuffer, crcPos);

	 					if(calcCrc == rxCrc)
	 					{
	 						return true;
	 					}
	 					else
	 					{
	 						return false;
	 					}
	 			}
	 			else
	 			{
	 				//TODO Manage status error
	 				return false;
	 			}

	 		}
	 		else
	 		{
	 			return false;
	 			//TODO manage errors
	 		}

	 	}
	 	else
	 	{
	 		return false;
	 		//TODO manage errors
	 	}

}









