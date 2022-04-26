/*
 * microCounter.c
 *
 *  Created on: 25 mars 2021
 *      Author: teamat
 */
#include "microCounter.h"



uint64_t getMicrosDiff(uint64_t time1, uint64_t time2)
{


	if(time1 > time2) // Counter looped
	{
		return (time2 + (MAX_MICROS_VALUE - time1));
	}
	else	// Standard case
	{
		return time2 - time1;
	}
}

