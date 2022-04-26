/*
 * uartManager_V1_0.cpp
 *
 *  Created on: Jan 21, 2022
 *      Author: teamat
 */

#include "uartManager_V1_0.h"
#include "string.h"
#include "stm32l4xx_hal.h"
#include "main.h"
#include "stdio.h"
#include <stdarg.h>

//#include "uartApp.h"


//__weak void serialPortConfig(uint32_t uartId, uint32_t baud, uint32_t databits, uint32_t parity, uint32_t stopbits, HalfDuplexMode_t halfDuplex)
//{
//
//}
//
//__weak void uart_enableHalfDuplexReceiver(serialPortHandle_t *hSerial)
//{
//
//}
//
//__weak void uart_Transmit(serialPortHandle_t *hSerial, uint32_t dataSize)
//{
//
//}


uartManager::uartManager(serialPortHandle_t *serial, unsigned char* txBuffer, uint32_t txBufferSize, unsigned char* rxBuffer,uint32_t rxBufferSize )
{

	_txBufferLen = txBufferSize;
	_rxBufferLen = rxBufferSize;

	_rxBuffer.init(rxBuffer, rxBufferSize, CircularBufferMode::MODE_MANUAL_LENGHT);
	_txBuffer.init(txBuffer, txBufferSize, CircularBufferMode::MODE_MANUAL_LENGHT);
	_hSerial = serial;
}


void uartManager::init(unsigned long baud, uint32_t databits,uint32_t stopbits, uint32_t parity)
{

	_baud = baud;
	uart_portConfig(_hSerial->uartId, (uint32_t)baud, databits, parity, stopbits, _hSerial->halfDuplexEnable);


}

void uartManager::begin(unsigned long baud)
{
	init(baud, 8,1, PARITY_NONE);
}

void uartManager::end()
{
	//TODO later
}

void uartManager::flush()
{
	_txBuffer.empty();
	_rxBuffer.empty();
}

uint32_t uartManager::available()
{
	return (uint32_t)_rxBuffer.getUsedSpace();
}

int uartManager::peakNextByte()
{
  unsigned char rxData;

  if (0 == _rxBuffer.getUsedSpace()) {
    return -1;
  } else {
	  _rxBuffer.peakBytes(&rxData, 1);
	return (int)rxData;
  }
}

int uartManager::read(uint8_t *buffer, uint32_t maxLen)
{
	return _rxBuffer.readBytes(buffer, maxLen);
}

int uartManager::read(char *buffer, uint32_t maxLen)
{
	return _rxBuffer.readChar(buffer, maxLen);
}

int uartManager::readToChar(char *buffer, char endChar)
{
	return _rxBuffer.readToTokken((unsigned char*)buffer, endChar);
}

int uartManager::read()
{
	uint8_t readByte;

	if(_rxBuffer.readBytes(&readByte, 1))
	{
		return (int)readByte;
	}
	else
	{
		return -1;
	}



}

int uartManager::write(char c)
{
	cbError_t error;

	if(_txBuffer.getFreeSpace() < 1)
	{
		return UART_ERROR_BUFFER_FULL;
	}

	error  = (int)_txBuffer.writeChar(&c,1);

	initiateTransfer();

	//startRxInterrupt();

	return (int)error;
}

int uartManager::write(uint8_t c)
{
	cbError_t error;

	if(_txBuffer.getFreeSpace() < 1)
	{
		return UART_ERROR_BUFFER_FULL;
	}

	error = _txBuffer.writeBytes(&c,1);

	initiateTransfer();
	//startRxInterrupt();
	//return (int)error;

	if(!error)
		{
			return 1;
		}
	else
	{
		return 0;
	}


}

int uartManager::write(const char *buffer, uint32_t len)
{
	cbError_t error;
	if(len == 0)
	{
		len = strlen(buffer);
	}

	if(_txBuffer.getFreeSpace() < len)
	{
		return UART_ERROR_BUFFER_FULL;
	}

	error = _txBuffer.writeChar(buffer,len);

	if(!txInProgress()){
		initiateTransfer();
	}

	//startRxInterrupt();
	//return (int)error;
	if(!error)
	{
		return (int)len;
	}
	else
		{
			return 0;
		}
}

int uartManager::write(uint8_t *buffer, uint32_t len)
{
	cbError_t error;

	if(_txBuffer.getFreeSpace() < len)
		{
			return UART_ERROR_BUFFER_FULL;
		}

	error = _txBuffer.writeBytes(buffer,len);

	initiateTransfer();
	//startRxInterrupt();
	//return (int)error;
	if(!error)
		{
			return (int)len;
		}
	else
		{
			return 0;
		}

}

int uartManager::print(const char *format, ...)
{
//		char tempBuffer[256];
//
//	  va_list args;
//	  va_start (args, buffer);
//	  int retVal = vprintf(tempBuffer,256, buffer, args);
//
//	  va_end (args);
//
//	  if(retVal)
//	  {
//		  return write(tempBuffer);
//	  }
//	  return retVal;

	  char buffer[128];
	    va_list args;
	    va_start (args, format);
	    vsnprintf (buffer,128,format, args);
	    write (buffer);
	    va_end (args);

	    return 0;
}

void uartManager::rxCompleteCallback(uint16_t transferSize)
{
	cbError_t error;

	_rxComplete = true;

	error = _rxBuffer.writeBytes(_hSerial->rxBuff,transferSize);

	if(error)
	{
		m_lastErrorType = (int)error;

	}

	startRxInterrupt(transferSize); // redémarrer l'Acquisition avec la même longueur

}

void uartManager::txCompleteCallback()
{
	_hSerial->txInProgress = false;

	//Si il y a du data dans le buffer circulaire, on continue la transmission
	//Sinon la boucle de transfert s'arrêtra.
	initiateTransfer();
}
bool uartManager::txInProgress()
{
	return _hSerial->txInProgress;
}

bool uartManager::initiateTransfer()
{
		enableTxMode();

		uint32_t dataSize;
		if(_txBuffer.getUsedSpace())
		{
			//Copie du data du buffer circulaire vers le buffer de travail
			dataSize = _txBuffer.readBytes(_hSerial->txBuff, _txBufferLen);

			if(dataSize > 0)
			{
				// Démarrage de l'envoi par intrruption
				uart_Transmit(_hSerial, dataSize);
				_hSerial->txInProgress = true;
			}
			else
			{
				//fin de transmission, passe en mode réception
				enableRxMode();
			}
		}
		else
		{
			//fin de transmission, passe en mode réception
			enableRxMode();
		}

	return _hSerial->txInProgress;
}


void uartManager::enableRxMode()
{
	uart_enableReceiver(_hSerial,1);
}

void uartManager::startRxInterrupt(int datataCount)
{
	uart_startRxInterrupt(_hSerial,datataCount);
}

void uartManager::stopRxInterrupt()
{
	uart_stopRxInterrupt(_hSerial);
}

void uartManager::enableTxMode()
{
	uart_enableTransmitter(_hSerial);
}

unsigned long uartManager::getBaud()
{
	return _baud;
}
