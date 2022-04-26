/**
 * @file dynamixel.cpp
 * @brief
 * @version 1.0
 *
 * Version 1.01 2022-04-14
 * 	-Première version
 */

#ifndef TEAMATLIBS_DYNAMIXEL_H_
#define TEAMATLIBS_DYNAMIXEL_H_


#include <TeamAt_H_Dynamixel/actuator_DynamixelXM.h>
#include <TeamAt_H_Dynamixel/dynamixel_xm540address.h>
#include <TeamAt_H_Dynamixel/dynamixelProtocolV2.h>
#include "stdint.h"
#include "stdbool.h"




/* Macro for Control Table Value */
#define DXL_MAKEWORD(a, b)  ((uint16_t)(((uint8_t)(((uint64_t)(a)) & 0xff)) | ((uint16_t)((uint8_t)(((uint64_t)(b)) & 0xff))) << 8))
#define DXL_MAKEDWORD(a, b) ((uint32_t)(((uint16_t)(((uint64_t)(a)) & 0xffff)) | ((uint32_t)((uint16_t)(((uint64_t)(b)) & 0xffff))) << 16))
#define DXL_LOWORD(l)       ((uint16_t)(((uint64_t)(l)) & 0xffff))
#define DXL_HIWORD(l)       ((uint16_t)((((uint64_t)(l)) >> 16) & 0xffff))
#define DXL_LOBYTE(w)       ((uint8_t)(((uint64_t)(w)) & 0xff))
#define DXL_HIBYTE(w)       ((uint8_t)((((uint64_t)(w)) >> 8) & 0xff))

#define DYNAMIXEL_MIN_PACKET_SIZE	11


#define TXPACKET_MAX_LEN    250
#define RXPACKET_MAX_LEN    250


#define DATA_COMM_IN_PROGRESS_NONE	0
#define DATA_COMM_IN_PROGRESS_HP 	1
#define DATA_COMM_IN_PROGRESS_LP	2


#define LP_DATA_PARAM_LEN (3) // Present Input Voltage(144) + Present Temperature(146)
#define LP_DATA_RX_LEN =

typedef struct
{
	uint8_t id;
	uint16_t paramLen;
	uint8_t instructionId;
	uint8_t paramPtr;
}instPacket_t;


/**
 * @fn unsigned short calculateDynamixelCRC(uint16_t, uint8_t*, uint16_t)
 * @brief Calculate CRC for dynamixel motors
 *
 * @param crc_accum	start at zero
 * @param data_blk_ptr data pointer
 * @param data_blk_size data size
 */
unsigned short calculateDynamixelCRC(uint16_t crc_accum, uint8_t *data_blk_ptr, uint16_t data_blk_size);

/**
 * @fn int findValidDynamixelHeader(uint8_t*, int)
 * @brief Find a valid dynamiel header in a buffer
 *
 * @param datathe buffer
 * @param len the lenght of the buffer
 * @return the position where the header was found, -1 if not found
 */
int findValidDynamixelHeader(uint8_t* data, int len);

/**
 * @fn int findValidDynamixelPacket(uint8_t*, int)
 * @brief Test a buffer to see if a complete and valid dynamixel packet is present
 *
 * @param data the buffer
 * @param len	the lenght of the buffer
 * @return 1 if valid
 */
int findValidDynamixelPacket(uint8_t* data, int len);

/**
 * @fn int packageReadInstruction(uint8_t*, uint8_t, uint16_t, uint16_t)
 * @brief package a read instruction packet
 *
 * @param targetBuffer the buffer to receive the packet
 * @param id	the id of the actuator
 * @param readStartAddress	The address of the data to read
 * @param readLen	the number of bytes to read
 *
 * @return The size of the packet
 */
int packageReadInstruction(uint8_t* targetBuffer, uint8_t id, uint16_t readStartAddress, uint16_t readLen);

/**
 * @fn int packageSyncReadInstruction(uint8_t*, uint8_t*, uint8_t, uint16_t, uint16_t)
 * @brief package a Sync Read instruction
 *
 * @param targetBuffer  	the buffer to receive the packet
 * @param idList	 		Buffer containing the list of IDs
 * @param idCount			The number of IDs
 * @param readStartAddress	The address of the data to read
 * @param readLen			the number of bytes to read
 *
 * @return The size of the packet
 */
int packageSyncReadInstruction(uint8_t* targetBuffer, uint8_t* idList,uint8_t idCount, uint16_t readStartAddress, uint16_t readLen);

/**
 * @fn int packageFastSyncReadInstruction(uint8_t*, uint8_t*, uint8_t, uint16_t, uint16_t)
 * @brief package a Fast Sync Read instruction
 *
 * @param targetBuffer  	the buffer to receive the packet
 * @param idList	 		Buffer containing the list of IDs
 * @param idCount			The number of IDs
 * @param readStartAddress	The address of the data to read
 * @param readLen			the number of bytes to read
 *
 * @return The size of the packet
 */
int packageFastSyncReadInstruction(uint8_t* targetBuffer, uint8_t* idList,uint8_t idCount, uint16_t readStartAddress, uint16_t readLen);

/**
 * @fn int packageSyncWriteInstruction(uint8_t*, uint8_t*, uint8_t, uint16_t, uint16_t, void*)
 * @brief  package a Sync Write instruction
 *
 * @param targetBuffer			the buffer to receive the packet
 * @param idList				Buffer containing the list of IDs
 * @param idCount				The number of IDs
 * @param writeStartAddress		The address of the data to write
 * @param dataTypeLen			The lenght of the data type
 * @param dataPtr				The source pointer of the data to send
 *
 * @return The size of the packet
 */
int packageSyncWriteInstruction(uint8_t* targetBuffer, uint8_t* idList,uint8_t idCount, uint16_t writeStartAddress, uint16_t dataTypeLen, void* dataPtr);

/**
 * @fn int bulkWriteInstructionPackage_start(uint8_t*)
 * @brief Start a dynamixel package ( call befor adding data to creat header and other bytes required
 *
 * @param targetBuffer	the buffer to receive the packet
 *
 * @return The size of the packet so far ( position of the write pointer)
 */
int bulkWriteInstructionPackage_start(uint8_t* targetBuffer);

/**
 * @fn int bulkWriteInstructionPackage_AddData(uint8_t*, int, uint8_t, uint16_t, uint16_t, void*)
 * @brief Add data to a started packet
 *
 * @param targetBuffer			the buffer to receive the packet
 * @param writePtr				The position where to write the data
 * @param id					The ID of the data
 * @param writeStartAddress		Address of the data
 * @param dataTypeLen			Data length
 * @param dataPtr				The source pointer of the data to send
 *
 * @return The size of the packet so far ( position of the write pointer)
 */
int bulkWriteInstructionPackage_AddData(uint8_t* targetBuffer, int writePtr, uint8_t id, uint16_t writeStartAddress, uint16_t dataTypeLen, void* dataPtr);

/**
 * @fn int bulkWriteInstructionPackage_complete(uint8_t*, int)
 * @brief Complete a started packet ( compute CRC, size, etc)
 *
 * @param targetBuffer the buffer to receive the packet
 * @param writePtr The position where to write the data
 *
 * @return The size of the packet ( position of the write pointer)
 */
int bulkWriteInstructionPackage_complete(uint8_t* targetBuffer, int writePtr);

/**
 * @fn bool validHeader(uint8_t*)
 * @brief test if header is valid
 *
 * @param targetBuffer	Buffer with the header
 * @return true of false
 */
bool validHeader(uint8_t* targetBuffer);

/**
 * @fn bool validStatusMessage(uint8_t*)
 * @brief Test if buffers contains a valid status message
 *
 * @param targetBuffer Buffer with the status message
 *
 * @return true of false
 */
bool validStatusMessage(uint8_t* targetBuffer);

#endif /* TEAMATLIBS_DYNAMIXEL_H_ */
