/*
 * uartApp.h
 *
 *  Created on: Jan 17, 2022
 *      Author: teamat
 */

#ifndef TEAMATAPP_UARTAPP_H_
#define TEAMATAPP_UARTAPP_H_

#include <CircularBuffer_V1_1.h>
#include "stdint.h"
#include "usart.h"

#include "TeamAT_B_UartManager/uartManager_V1_0.h"


#define UART1_RX_BUFFER_SIZE (128+1)
#define UART1_TX_BUFFER_SIZE (128+1)

#define UART2_RX_BUFFER_SIZE (64+1)
#define UART2_TX_BUFFER_SIZE (64+1)


#define UART_NB_DYNAMIXEL_MAX		6
#define UART_DYNAMIXEL_DATA_SIZE	23
#define UART_DYNAMIXEL_REPLY_SIZE	(13  + ((UART_NB_DYNAMIXEL_MAX-1)*3) + ((UART_DYNAMIXEL_DATA_SIZE + 1) * UART_NB_DYNAMIXEL_MAX))
#define UART3_RX_BUFFER_SIZE 		3 * UART_DYNAMIXEL_REPLY_SIZE + 1 //on peut avoir 3 replay non traitées en mémoire.        //(35*6)//64 // Place pour recevoir 6 message d'actionneur
#define UART3_TX_BUFFER_SIZE 		(64+1)








//extern uint8_t uart3TxPortBuffer[UART3_TX_BUFFER_SIZE];
//extern uint8_t uart3RxPortBuffer[UART3_TX_BUFFER_SIZE];


#define UARTAPP_PARITY_NONE 0
#define UARTAPP_PARITY_EVEN 0

extern uartManager serial1;
extern uartManager serial2;
extern uartManager serial3;
extern uint8_t uart2RxPortBuffer[1];


void uartInit();

/**
 * @fn void serialPortConfig(UART_HandleTypeDef*, uint32_t, uint32_t, uint32_t, uint32_t, HalfDuplexMode_t)
 * @brief
 *
 * @param huart		pointer to the uart handle
 * @param baud		baud rate
 * @param databits  UART_WORDLENGTH_... in file stm32l4xx_hal_uart_ex.h
 * @param parity	see UART_PARITY_... in file stm32l4xx_hal_uart.h
 * @param stopbits  see UART_STOPBITS_... in file stm32l4xx_hal_uart.h
 * @param halfDuplex see HALF_DUPLEX_ in usart.h
 */
//void serialPortConfig(UART_HandleTypeDef *huart, uint32_t baud, uint32_t databits, uint32_t parity, uint32_t stopbits, HalfDuplexMode_t halfDuplex);


//void serialPortConfig(uint32_t uartId, uint32_t baud, uint32_t databits, uint32_t parity, uint32_t stopbits, HalfDuplexMode_t halfDuplex);
//void disableUartInterrupts(UART_HandleTypeDef * huart);
//void enableUartInterrupts(UART_HandleTypeDef * huart);


//void uart_enableHalfDuplexReceiver(serialPortHandle_t *hSerial);


//void uart_Transmit(serialPortHandle_t *hSerial, uint32_t dataSize);


UART_HandleTypeDef * getUartHandle(uint32_t uartId);

//typedef uartManager serial_t;



#endif /* TEAMATAPP_UARTAPP_H_ */
