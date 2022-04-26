/**
 * @file dynamixelProtocolV2.h
 * @brief
 * @version 1.0
 *
 * Version 1.01 2022-04-14
 * 	-Première version
 */

#ifndef TEAMATLIBS_DYNAMIXELPROTOCOLV2_H_
#define TEAMATLIBS_DYNAMIXELPROTOCOLV2_H_


#define DYNAMIXEL_INST_PING					0x01
#define DYNAMIXEL_INST_READ					0x02
#define DYNAMIXEL_INST_WRITE				0x03
#define DYNAMIXEL_INST_REGWRITE				0x04
#define DYNAMIXEL_INST_ACTION				0x05
#define DYNAMIXEL_INST_FACTORY_RESET		0x06
#define DYNAMIXEL_INST_REBOOT				0x08
#define DYNAMIXEL_INST_CLEAR				0x09
#define DYNAMIXEL_INST_CONTROL_TABLE_BACKUP	0x20
#define DYNAMIXEL_INST_STATUS_RETURN		0x55
#define DYNAMIXEL_INST_SYNC_READ			0x82
#define DYNAMIXEL_INST_SYNC_WRITE			0x83
#define DYNAMIXEL_INST_FAST_SYNC_READ		0x8A
#define DYNAMIXEL_INST_BULK_READ			0x92
#define DYNAMIXEL_INST_BULK_WRITE			0x93
#define DYNAMIXEL_INST_FAST_BULK_READ		0x9A


///////////////// for Protocol 2.0 Packet /////////////////
#define PKT_HEADER0             0
#define PKT_HEADER1             1
#define PKT_HEADER2             2
#define PKT_RESERVED            3
#define PKT_ID                  4
#define PKT_LENGTH_L            5
#define PKT_LENGTH_H            6
#define PKT_INSTRUCTION         7
#define PKT_ERROR               8
#define PKT_PARAMETER0          8

///////////////// Protocol 2.0 Error bit /////////////////
#define ERRNUM_RESULT_FAIL      1       // Failed to process the instruction packet.
#define ERRNUM_INSTRUCTION      2       // Instruction error
#define ERRNUM_CRC              3       // CRC check error
#define ERRNUM_DATA_RANGE       4       // Data range error
#define ERRNUM_DATA_LENGTH      5       // Data length error
#define ERRNUM_DATA_LIMIT       6       // Data limit error
#define ERRNUM_ACCESS           7       // Access error

#define ERRBIT_ALERT            128     //When the device has a problem, this bit is set to 1. Check "Device Status Check" value.


#define PKT_HEADER_LEN		4
#define PKT_ID_LEN			1
#define PKT_LENGHT_LEN		2
#define PKT_INSTRUCTION_LEN	1
#define PKT_ERROR_LEN		1
#define PKT_ADDRESS_LEN		2
#define PKT_CRC_LEN			2

#define PKT_STATUS_OVERHEAD_LEN	(PKT_HEADER_LEN + PKT_ID_LEN + PKT_LENGHT_LEN + PKT_INSTRUCTION_LEN + PKT_ERROR_LEN + PKT_CRC_LEN)

typedef enum
{
	br_9600bps = 0,
	br_56700bps,
	br_115200bps,
	br_1Mbps,
	br_2Mbps,
	br_3Mbps,
	br_4Mbps,
	br_4_5Mbps
}dynamixelBaudrate_t;




#endif /* TEAMATLIBS_DYNAMIXELPROTOCOLV2_H_ */
