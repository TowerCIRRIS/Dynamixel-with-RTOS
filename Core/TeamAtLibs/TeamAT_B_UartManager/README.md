# TeamAT Uart Manager

## Introduction

 Librairie qui permet  d'interface les UART d'une façon un peu plus intuitive

## Hardware Requirement and compatibility

### Module compatibility

#### Supported hardware

##### Arduino boards

- Non testé / non implémenté

##### ST boards

- Stm32 nucléo L496zg-p



## Software Requirement and compatibility

### Required Libraries

- CircularBuffer_V1_1


## Quick Getting started

### Pour Nucléo

      1. Inclure le .h et le .cpp dans le dossier de librairies Team AT



### Application

Pour chaque port à utiliser:

#### Déclaration

1. Déclarer un tableau qui sera utiliser à l'interne pour la réception et transmission 

   Ces tableaux seront utiliser par le uart manager pour stocké les messages reçu / à envoyer.  Assurez-vous d'avoir suffisamment d'espace pour les données à recevoir

   ```
   unsigned char uart1TxBuffer[UART1_TX_BUFFER_SIZE];
   unsigned char uart1RxBuffer[UART1_RX_BUFFER_SIZE];
   ```

   

2. Déclarer tableau pour le port

   Le tableau pour le port un un "buffer" qui seras assez grand pour recevoir les bytes tu port série directement avant d'être ajouté à la pile d'accumulation du uart manager.

​		

```
uint8_t uart1TxPortBuffer[UART1_TX_BUFFER_SIZE];
uint8_t uart1RxPortBuffer[1];
```

Note: Le buffer de réception est 1 dans cet exemple ca on ne sait pas la longueur des messages à recevoir alors on traite la réception en byte par byte.  Si vosu avec un application qui reçoit un des messages de tailles fix, pour pouvez utiliser un nombre différent.  Vous devrez également utiliser le même notre en démarrant la réception plus tard ex: serial1.startRxInterrupt(1).

Pour le buffer de transmission on se base sur la taille de ce qu'on désire envoyer.



1. Déclarer un "handle" pour notre port série

   ```
   serialPortHandle_t hSerial1;
   ```

   

2. Déclarer un instance de notre port et lui passer les tableaux et le handle

   ```
   uartManager serial1(&hSerial1, uart1TxBuffer,UART1_TX_BUFFER_SIZE, uart1RxBuffer,UART1_RX_BUFFER_SIZE);
   ```

   

#### Callbacks

​	Les callback sotn les fonction à appeler lors d'interruptions du système

##### HAL_UART_RxCpltCallback

 Réception complète.

Exemple plateforme Stm32:

```
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == LPUART1)
	{
		serial1.rxCompleteCallback(huart->RxXferSize);
	}
}
```

##### HAL_UART_TxCpltCallback

Exemple plateforme Stm32:

```
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == LPUART1)
	{
		serial1.txCompleteCallback();
	}

}
```



#### Fonctions plateforme

Ces fonctions sont les fonctions propre à la plateforme qui doivent être implémentées poru chaques différente splateformes



##### void uart_portConfig(uint32_t uartId, uint32_t baud, uint32_t databits, uint32_t parity, uint32_t stopbits, HalfDuplexMode_t halfDuplex)

Exemple plateforme Stm32:

```
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

```



##### void uart_Transmit(serialPortHandle_t *hSerial, uint32_t dataSize)

Dans l'Exemple suivant sur le stm32 , le uart #3 est géré en DMA, les autres in interruption.

```
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
```



##### void uart_enableReceiver(serialPortHandle_t *hSerial, int dataLen)

Exemple plateforme Stm32:

```
void uart_enableReceiver(serialPortHandle_t *hSerial, int dataLen)
{
	if(hSerial->halfDuplexEnable)
	{
		uart_enableHalfDuplexReceiver(hSerial);
	}
}
```



##### void uart_enableTransmitter(serialPortHandle_t *hSerial)

Exemple plateforme Stm32:

```
void uart_enableTransmitter(serialPortHandle_t *hSerial)
{
	if(hSerial->halfDuplexEnable)
	{
		uart_enableHalfDuplexTransmitter(hSerial);
	}
}
```



##### void uart_enableHalfDuplexReceiver(serialPortHandle_t *hSerial)

Exemple plateforme Stm32:

```
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
	}
}
```

##### void uart_startRxInterrupt(serialPortHandle_t *hSerial, int dataLen)

Exemple plateforme Stm32:

```
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

	}
}
```



##### void uart_stopRxInterrupt(serialPortHandle_t *hSerial)

Exemple plateforme Stm32:

```
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
	}
}
```



##### void uart_enableHalfDuplexTransmitter(serialPortHandle_t *hSerial)

Exemple plateforme Stm32:

```
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
		}
}
```



#### Initialisation

```
	hSerial1.uartId = UART1_ID; // The ID of the usart ( defines inm uartManagerVX_X.h)
	hSerial1.halfDuplexEnable = HALF_DUPLEX_DISABLED;	// full duplex or half duplex( 1 wire)
	hSerial1.rxBuff = uart1RxPortBuffer;				// Port buffer
	hSerial1.txBuff = uart1TxPortBuffer;				// Port buffer
	hSerial1.txInProgress = false;						// for internal use, init to false;

	serial1.init(460800, UART_WORDLENGTH_8B, UART_STOPBITS_1, UART_PARITY_NONE);
	serial1.enableRxMode();
```

On initialise le port série et on démarre la réception 



 
