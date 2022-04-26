/**
 * @def AT_PLATFORMABSTRACTION_H
 * @brief 	This librarie is used to do an abstraction of the different platforms
 * 			so that the code build using this library may be reused on ST,
 * 			Arduino, Atmel, etc.
 *
 */

#ifndef AT_PLATFORMABSTRACTION_H
#define AT_PLATFORMABSTRACTION_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"


// For STM32
#ifdef STM32L496xx

	#include "stm32l4xx.h"

	#define digitalPinRead(pin,port)  ((unsigned char)HAL_GPIO_ReadPin((GPIO_TypeDef*)port, (uint16_t)pin))
	#define digitalPinWrite(pin,port,state) (HAL_GPIO_WritePin((GPIO_TypeDef*)port, (uint16_t)pin, (GPIO_PinState)state))
#endif



#ifdef ARDUINO
		// Arduino Example (Not tested yet)
		#define digitalPinRead(pin,port)  ((unsigned char)digitalRead(pin)
		#define digitalPinWrite(pin,port,state) (digitalWrite((uint32_t)pin, (uint32_t)state))
#endif


	uint32_t atGetSysTick_ms();

#ifdef __cplusplus
}
#endif


#endif
