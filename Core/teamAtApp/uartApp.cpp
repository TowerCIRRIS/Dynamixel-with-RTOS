/*
 * uartApp.cpp
 *
 *  Created on: Jan 17, 2022
 *      Author: teamat
 */


#include "stm32l4xx_hal.h"
#include "usart.h"
#include "uartApp.h"
#include "TeamAT_B_UartManager/uartManager_V1_0.h"



unsigned char uart1TxBuffer[UART1_TX_BUFFER_SIZE];
unsigned char uart1RxBuffer[UART1_RX_BUFFER_SIZE];

uint8_t uart1TxPortBuffer[UART1_TX_BUFFER_SIZE];
uint8_t uart1RxPortBuffer[1];

serialPortHandle_t hSerial1;

uartManager serial1(&hSerial1, uart1TxBuffer,UART1_TX_BUFFER_SIZE, uart1RxBuffer,UART1_RX_BUFFER_SIZE);


unsigned char uart3TxBuffer[UART3_TX_BUFFER_SIZE];
unsigned char uart3RxBuffer[UART3_RX_BUFFER_SIZE];

uint8_t uart3TxPortBuffer[UART3_TX_BUFFER_SIZE];
uint8_t uart3RxPortBuffer[UART3_RX_BUFFER_SIZE];

serialPortHandle_t hSerial3;

uartManager serial3(&hSerial3, uart3TxBuffer,UART3_TX_BUFFER_SIZE, uart3RxBuffer,UART3_RX_BUFFER_SIZE);

uint8_t uart2TxPortBuffer[UART2_TX_BUFFER_SIZE];
uint8_t uart2RxPortBuffer[1];


unsigned char uart2TxBuffer[UART2_TX_BUFFER_SIZE];
unsigned char uart2RxBuffer[UART2_RX_BUFFER_SIZE];

serialPortHandle_t hSerial2;

uartManager serial2(&hSerial2, uart2TxBuffer,UART2_TX_BUFFER_SIZE, uart2RxBuffer,UART2_RX_BUFFER_SIZE);


void uartInit()
{


	hSerial1.uartId = UART1_ID;
	hSerial1.halfDuplexEnable = HALF_DUPLEX_DISABLED;
	hSerial1.rxBuff = uart1RxPortBuffer;
	hSerial1.txBuff = uart1TxPortBuffer;
	hSerial1.txInProgress = false;

	serial1.init(460800, UART_WORDLENGTH_8B, UART_STOPBITS_1, UART_PARITY_NONE);
	serial1.enableRxMode();


	hSerial2.uartId = UART2_ID;
	hSerial2.halfDuplexEnable = HALF_DUPLEX_ENABLED;
	hSerial2.rxBuff = uart2RxPortBuffer;
	hSerial2.txBuff = uart2TxPortBuffer;
	hSerial2.txInProgress = false;

	serial2.init(115200, UART_WORDLENGTH_8B, UART_STOPBITS_1, UART_PARITY_NONE);
	serial2.enableRxMode(); // Set rx mode by default


	hSerial3.uartId = UART3_ID;
	hSerial3.halfDuplexEnable = HALF_DUPLEX_ENABLED;
	hSerial3.rxBuff = uart3RxPortBuffer;
	hSerial3.txBuff = uart3TxPortBuffer;
	hSerial3.txInProgress = false;

	serial3.init(2000000, UART_WORDLENGTH_8B, UART_STOPBITS_1, UART_PARITY_NONE);
	serial3.enableRxMode(); // Set rx mode by default


}


void uart_Transmit(serialPortHandle_t *hSerial, uint32_t dataSize)
{
	UART_HandleTypeDef *huart;

	huart = getUartHandle(hSerial->uartId);

	if(huart->Instance == USART3)
	{
		HAL_UART_Transmit_DMA(huart, hSerial->txBuff, dataSize);
	}
	else
	{
		HAL_UART_Transmit_IT(huart, hSerial->txBuff, dataSize);
	}

}


void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == LPUART1)
	{
		serial1.txCompleteCallback();
	}

	if(huart->Instance == USART2)
	{
//		hSerial2.txInProgress = false;
//
//		//Si il y a du data dans le buffer circulaire, on continue la transmission
//		//Sinon la boucle de transfert s'arrêtra.
//		serial2.initiateTransfer();

		//serial2.txCompleteCallback();
	}

	if(huart->Instance == USART3)
	{
		serial3.txCompleteCallback();
	}

	if(huart->Instance == UART4)
	{
		//serial4.txCompleteCallback();
	}

	if(huart->Instance == UART5)
	{
		//serial5.txCompleteCallback();
	}
}

int  rxPtr = 0;
char rxBuffer[128];

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == LPUART1)
	{
		//HAL_GPIO_WritePin(OUT_DEBUG_3_PORT, OUT_DEBUG_3_PIN, GPIO_PIN_SET); // For debug
		serial1.rxCompleteCallback(huart->RxXferSize);
		//HAL_GPIO_WritePin(OUT_DEBUG_3_PORT, OUT_DEBUG_3_PIN, GPIO_PIN_RESET); // For Debug
	}

	if(huart->Instance == USART2)
	{
		serial2.rxCompleteCallback(huart->RxXferSize);
	}

	if(huart->Instance == USART3)
	{
		// HAL_GPIO_WritePin(OUT_DEBUG_1_PORT, OUT_DEBUG_1_PIN, GPIO_PIN_SET); // For debug
		serial3.rxCompleteCallback(huart->RxXferSize);
		// HAL_GPIO_WritePin(OUT_DEBUG_1_PORT, OUT_DEBUG_1_PIN, GPIO_PIN_RESET); // For debug

	}

	if(huart->Instance == UART4)
	{

	}

	if(huart->Instance == UART5)
	{

	}
}

UART_HandleTypeDef * getUartHandle(uint32_t uartId)
{
	switch(uartId)
		{
			case UART1_ID:
				return &hlpuart1;

				break;

			case UART2_ID:
				return &huart2;

					break;

			case UART3_ID:
				return &huart3;

						break;
//			case UART4_ID:
//	//			HAL_HalfDuplex_EnableReceiver(&huart4);
//	//			HAL_UART_Receive_IT(&huart4, hSerial->rxBuff, 1);
//						break;
//			case UART5_ID:
//	//			HAL_HalfDuplex_EnableReceiver(&huart4);
//	//			HAL_UART_Receive_IT(&huart4, hSerial->rxBuff, 1);
//						break;
		}
	return NULL;
}


void uart_portConfig(uint32_t uartId, uint32_t baud, uint32_t databits, uint32_t parity, uint32_t stopbits, HalfDuplexMode_t halfDuplex)
{
	UART_HandleTypeDef *huart;

	huart = getUartHandle(uartId);

	huart->Init.BaudRate = baud;
	switch(databits)
	{

		case 7:
			huart->Init.WordLength = UART_WORDLENGTH_7B;
			break;

		case 8:
			huart->Init.WordLength = UART_WORDLENGTH_8B;
			break;

		case 9:
			huart->Init.WordLength = UART_WORDLENGTH_9B;
			break;

		default:
			huart->Init.WordLength = UART_WORDLENGTH_8B;
		break;
	}

	switch(stopbits)
	{

		case 1:
			huart->Init.StopBits = UART_STOPBITS_1;
			break;

		case 2:
			huart->Init.StopBits = UART_STOPBITS_2;
			break;

		default:
			huart->Init.StopBits = UART_STOPBITS_1;
		break;
	}

	switch(parity)
	{

		case PARITY_NONE:
			huart->Init.Parity = UART_PARITY_NONE;
			break;

		case PARITY_EVEN:
			huart->Init.Parity = UART_PARITY_EVEN;
			break;

		case PARITY_ODD:
			huart->Init.Parity = UART_PARITY_ODD;
			break;

		default:
			huart->Init.StopBits = UART_STOPBITS_1;
		break;
	}


	if(halfDuplex == HalfDuplexMode_t::HALF_DUPLEX_ENABLED)
	  {
		if (HAL_HalfDuplex_Init(huart) != HAL_OK)
		{
		  Error_Handler();
		}
	  }
	  else
	  {
		if (HAL_UART_Init(huart) != HAL_OK)
		{
		  Error_Handler();
		}
	  }

}


void uart_enableReceiver(serialPortHandle_t *hSerial, int dataLen)
{
	if(hSerial->halfDuplexEnable)
	{
		uart_enableHalfDuplexReceiver(hSerial);
	}
}

void uart_enableTransmitter(serialPortHandle_t *hSerial)
{
	if(hSerial->halfDuplexEnable)
	{
		uart_enableHalfDuplexTransmitter(hSerial);
	}
}

void uart_enableHalfDuplexReceiver(serialPortHandle_t *hSerial)
{
	switch(hSerial->uartId)
	{
		case UART1_ID:
			HAL_HalfDuplex_EnableReceiver(&hlpuart1);
			break;
		case UART2_ID:
			HAL_HalfDuplex_EnableReceiver(&huart2);
				break;
		case UART3_ID:
			HAL_HalfDuplex_EnableReceiver(&huart3);
					break;
		case UART4_ID:
//			HAL_HalfDuplex_EnableReceiver(&huart4);
//			HAL_UART_Receive_IT(&huart4, hSerial->rxBuff, 1);
					break;
		case UART5_ID:
//			HAL_HalfDuplex_EnableReceiver(&huart4);
//			HAL_UART_Receive_IT(&huart4, hSerial->rxBuff, 1);
					break;
	}
}


void uart_startRxInterrupt(serialPortHandle_t *hSerial, int dataLen)
{
	HAL_StatusTypeDef status;

	switch(hSerial->uartId)
	{
		case UART1_ID:
			HAL_UART_AbortReceive_IT(&hlpuart1);
			HAL_UART_Receive_IT(&hlpuart1, hSerial->rxBuff, dataLen);
			break;
		case UART2_ID:
			HAL_UART_AbortReceive_IT(&huart2);
			HAL_UART_Receive_IT(&huart2, hSerial->rxBuff, dataLen);
				break;

		case UART3_ID:
			HAL_UART_AbortReceive_IT(&huart3);

			status = HAL_UART_Receive_DMA(&huart3, hSerial->rxBuff, dataLen);
			if(status > 0)
			{
				Error_Handler();
			}

					break;
		case UART4_ID:
//			HAL_UART_AbortReceive_IT(&huart4);
//			HAL_UART_Receive_IT(&huart4, hSerial->rxBuff, dataLen);
					break;
		case UART5_ID:
//			HAL_StatusTypeDef HAL_UART_AbortReceive_IT(&huart5);
//			HAL_UART_Receive_IT(&huart5, hSerial->rxBuff, dataLen);
					break;
	}
}

void uart_stopRxInterrupt(serialPortHandle_t *hSerial)
{
	switch(hSerial->uartId)
	{
		case UART1_ID:
			HAL_UART_AbortReceive_IT(&hlpuart1);
			break;
		case UART2_ID:
			HAL_UART_AbortReceive_IT(&huart2);
				break;
		case UART3_ID:
			HAL_UART_AbortReceive_IT(&huart3);
					break;
		case UART4_ID:
//			HAL_UART_AbortReceive_IT(&huart4);
					break;
		case UART5_ID:
//			HAL_StatusTypeDef HAL_UART_AbortReceive_IT(&huart5);
					break;
	}
}



void uart_enableHalfDuplexTransmitter(serialPortHandle_t *hSerial)
{
	switch(hSerial->uartId)
		{
			case UART1_ID:
				HAL_HalfDuplex_EnableTransmitter(&hlpuart1);
				break;
			case UART2_ID:
				HAL_HalfDuplex_EnableTransmitter(&huart2);
					break;
			case UART3_ID:
				HAL_HalfDuplex_EnableTransmitter(&huart3);
						break;
			case UART4_ID:
	//			HAL_HalfDuplex_EnableTransmitter(&huart4);
						break;
			case UART5_ID:
	//			HAL_HalfDuplex_EnableTransmitter(&huart4);
						break;
		}
}

