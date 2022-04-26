/*
 * microCOunter.h
 *
 *  Created on: 25 mars 2021
 *      Author: teamat
 */

#ifndef MICROCOUNTER_H_
#define MICROCOUNTER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

#define MAX_MILLI_VALUE 	0xFFFF
#define MAX_MICROS_VALUE 	(1000 * MAX_MILLI_VALUE + 999)


static inline uint32_t LL_SYSTICK_IsActiveCounterFlag(void)
{
  return ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == (SysTick_CTRL_COUNTFLAG_Msk));
}


static inline uint64_t getTimeMicros(void)
{
  /* Ensure COUNTFLAG is reset by reading SysTick control and status register */
  LL_SYSTICK_IsActiveCounterFlag();
  uint64_t m = HAL_GetTick();
  const uint64_t tms = SysTick->LOAD + 1;
  volatile uint64_t u = tms - SysTick->VAL;
  if (LL_SYSTICK_IsActiveCounterFlag()) {
    m = HAL_GetTick();
    u = tms - SysTick->VAL;
  }
  return (m * 1000 + (u * 1000) / tms);
}

uint64_t getMicrosDiff(uint64_t time1, uint64_t time2);

#ifdef __cplusplus
}
#endif

#endif /* SRC_MICROCOUNTER_H_ */
