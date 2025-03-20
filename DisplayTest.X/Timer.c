/*
 * Timer.c
 *
 * Created: 29-01-2018 13:16:57
 *  Author: ltpe
 */ 

#include "Timer.h"

 static uint16_t Timer_PrescalerBitValues[][2] =
 {
	 {TimerPrescalerValue1,    0b001},
	 {TimerPrescalerValue8,    0b010},
	 {TimerPrescalerValue64,   0b011},
	 {TimerPrescalerValue256,  0b100},
	 {TimerPrescalerValue1024, 0b101}
 };

 uint8_t GetBitValuesForClockSelect(uint_fast16_t ClockPrescalerValue)
 {
	uint8_t Counter = 0;

	do 
	{
		if (Timer_PrescalerBitValues[Counter][0] == ClockPrescalerValue)
		{
			return (Timer_PrescalerBitValues[Counter][1]);
		}
	} while (++Counter < sizeof(Timer_PrescalerBitValues) / sizeof(Timer_PrescalerBitValues[0]));

	return (Timer_PrescalerBitValues[0][1]);
 }