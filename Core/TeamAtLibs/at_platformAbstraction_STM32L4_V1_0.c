
#include "at_plaformAbstraction_V1_0.h"

#include "stm32l4xx_hal.h"
#include "main.h"


extern uint32_t sysTick_teamAt;
uint32_t atGetSysTick_ms()
{
	//return HAL_GetTick();
	return sysTick_teamAt;
}



