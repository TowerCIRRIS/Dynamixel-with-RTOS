/*
 * uartManager_V1_0.h
 *
 *  Created on: Jan 21, 2022
 *      Author: teamat
 */

#ifndef UARTMANAGER_V1_0_H_
#define UARTMANAGER_V1_0_H_

#include <CircularBuffer_V1_1.h>
#include "stdint.h"


typedef enum {
  HALF_DUPLEX_DISABLED,
  HALF_DUPLEX_ENABLED
} HalfDuplexMode_t;

#define UART_ERROR_SUCCESS	   	0
#define UART_ERROR_BUFFER_FULL 	-1

#define MODE_FULL_DUPLEX 0
#define MODE_HALF_DUPLEX 1

#define PARITY_NONE 0
#define PARITY_ODD 	1
#define PARITY_EVEN 2

#define UART1_ID 1
#define UART2_ID 2
#define UART3_ID 3
#define UART4_ID 4
#define UART5_ID 5

typedef struct {

	uint32_t uartId;
	HalfDuplexMode_t halfDuplexEnable = HALF_DUPLEX_DISABLED;

	uint8_t* txBuff;
	uint8_t* rxBuff;

	bool txInProgress;

}serialPortHandle_t;


// Fonction WEAK qui doivent être implémentées par l'appllication
void uart_portConfig(uint32_t uartId, uint32_t baud, uint32_t databits, uint32_t parity, uint32_t stopbits, HalfDuplexMode_t halfDuplex);
void uart_enableHalfDuplexReceiver(serialPortHandle_t *hSerial);
void uart_enableHalfDuplexTransmitter(serialPortHandle_t *hSerial);

void uart_enableReceiver(serialPortHandle_t *hSerial,int dataLen);
void uart_enableTransmitter(serialPortHandle_t *hSerial);
void uart_startRxInterrupt(serialPortHandle_t *hSerial, int dataLen);
void uart_stopRxInterrupt(serialPortHandle_t *hSerial);
void uart_Transmit(serialPortHandle_t *hSerial, uint32_t dataSize);


class uartManager
{
	private:

		unsigned long _baud;

		int m_lastErrorType = 0;

	public:

		uartManager(serialPortHandle_t *serial, unsigned char* txBuffer, uint32_t txBufferSize, unsigned char* rxBuffer,uint32_t rxBufferSize );

		void init(unsigned long baud, uint32_t databits,uint32_t stopbits, uint32_t parity);

		void begin(unsigned long baud);
		void end();
		void flush();
		//Read data in buffer, len =0 means read all buffer
		int read();
		int read(char *buffer, uint32_t maxLen = 0);
		int read(uint8_t *buffer, uint32_t maxLen = 0);
		int readToChar(char *buffer, char endChar);

		int write(char c);
		int write(uint8_t c);
		int write(const char *buffer, uint32_t len = 0);
		int write(uint8_t *buffer, uint32_t len);
		int print(const char *format, ...);


		uint32_t available();
		int peakNextByte();


		bool initiateTransfer();
		void enableRxMode();
		void startRxInterrupt(int datataCount = 1);
		void stopRxInterrupt();
		void enableTxMode();
		void rxCompleteCallback(uint16_t transferSize);
		void txCompleteCallback();
		bool txInProgress();

		unsigned long getBaud();

		serialPortHandle_t *_hSerial;
		CircularBuffer _rxBuffer;
		CircularBuffer _txBuffer;

		uint32_t _txBufferLen;
		uint32_t _rxBufferLen;

		bool _rxComplete = false;

};



#endif /* TEAMATLIBS_UARTMANAGER_V1_0_H_ */
