/*
 * Timer.h
 *
 * Created: 16-01-2018 07:48:54
 *  Author: ltpe
 */ 


#ifndef TIMER_H_
#define TIMER_H_

#include "ProjectDefines.h"
#include <math.h>

#ifndef F_CPU
	#define F_CPU 16000000UL
#endif

#define		TimerPrescalerValue1						 1
#define		TimerPrescalerValue8						 8
#define		TimerPrescalerValue64						64
#define		TimerPrescalerValue256					    256
#define		TimerPrescalerValue1024						1024	

#define		Timer16BitPrescalerValue				TimerPrescalerValue64
#define		MaxValueIn16BitRegister					0xFFFF
#define		VariableValue1SecValue16Bit				(uint16_t)round((1/((((float)1/F_CPU)) * Timer16BitPrescalerValue * MaxValueIn16BitRegister)))
#define		VariableValue2SecValue16Bit				(uint16_t)(2 * VariableValue1SecValue16Bit)
#define		VariableValue3SecValue16Bit				(uint16_t)(3 * VariableValue1SecValue16Bit)
#define		VariableValue4SecValue16Bit				(uint16_t)(4 * VariableValue1SecValue16Bit)

//#define		Timer8BitPrescalerValue					TimerPrescalerValue8
#define		Timer8BitPrescalerValue					TimerPrescalerValue64
#define		MaxValueIn8BitRegister					0xFF
//#define		VariableValue1SecValue8Bit				(uint8_t)round((1/(((float)1/F_CPU)*Timer8BitPrescalerValue * MaxValueIn8BitRegister)) - 1)
#define		VariableValue1SecValue8Bit				(uint16_t)round((1/(((float)1/F_CPU)*Timer8BitPrescalerValue * MaxValueIn8BitRegister)))
#define		VariableValue2SecValue8Bit				(uint16_t)(2 * VariableValue1SecValue8Bit)
#define		VariableValue3SecValue8Bit				(uint16_t)(3 * VariableValue1SecValue8Bit)
#define		VariableValue4SecValue8Bit				(uint16_t)(4 * VariableValue1SecValue8Bit)

// Har man behov for ekstra lange Timeout Perioder, kan man lave alle "VariableValue" defines
// herover om til 32 bit værdier (uint32_t). Dette kræver så, man laver alle variable til at tælle
// Timer ticks op med om til 32 bit størelser også.

extern  uint8_t GetBitValuesForClockSelect(uint_fast16_t ClockPrescalerValue);

#endif /* TIMER_H_ */