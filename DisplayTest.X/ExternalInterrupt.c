/*
 * ExternalInterrupt.c
 *
 * Created: 24-11-2023 13:25:43
 *  Author: ltpe
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>

#include "ExternalInterrupt.h"

static void NoCallbackIsPerformedOnExternalInterrupt(uint16_t NumberOfExternalInterruptsOnPin)
{
	//uint16_t Test = NumberOfExternalInterruptsOnPin;
}

static volatile ExternalInterruptCallbackStruct ExternalInterruptCallbackStructArray[NUMBER_OF_EXTERNAL_INTRRUPTS] =
{
	{NoCallbackIsPerformedOnExternalInterrupt, 0},
	{NoCallbackIsPerformedOnExternalInterrupt, 0},
#if defined (_AVR_IOM2560_H_)
	{NoCallbackIsPerformedOnExternalInterrupt, 0},
	{NoCallbackIsPerformedOnExternalInterrupt, 0},
	{NoCallbackIsPerformedOnExternalInterrupt, 0},
	{NoCallbackIsPerformedOnExternalInterrupt, 0},
	{NoCallbackIsPerformedOnExternalInterrupt, 0},
	{NoCallbackIsPerformedOnExternalInterrupt, 0}	
#endif							
};

bool AttachAndEnableExternalInterrupt(uint8_t ExternalInterruptNumber, Function_Pointer_With_One_Uint16t_Parameter FunctionPointer, Interrupt_Cause_C InterruptMode)
{
	if (ExternalInterruptNumber < NUMBER_OF_EXTERNAL_INTRRUPTS)
	{
		if (!(NoCallbackIsPerformedOnExternalInterrupt == ExternalInterruptCallbackStructArray[ExternalInterruptNumber].ThisFunctionPointer))
		{
			return (false);
		}
		else
		{
			switch (ExternalInterruptNumber)
			{
				case EXTERNAL_INTERRUPT_0:
					EICRA = (EICRA & ~((1 << ISC00) | (1 << ISC01))) | (InterruptMode << ISC00);
					EIMSK |= (1 << INT0);
					break;
					
				case EXTERNAL_INTERRUPT_1:
					EICRA = (EICRA & ~((1 << ISC10) | (1 << ISC11))) | (InterruptMode << ISC10);
					EIMSK |= (1 << INT1);
					break;
					
#if defined (_AVR_IOM2560_H_)				 
				case EXTERNAL_INTERRUPT_2:
					 EICRA = (EICRA & ~((1 << ISC20) | (1 << ISC21))) | (InterruptMode << ISC20);
					 EIMSK |= (1 << INT2);
					break;
					
				case EXTERNAL_INTERRUPT_3:
					EICRA = (EICRA & ~((1 << ISC30) | (1 << ISC31))) | (InterruptMode << ISC30);
					EIMSK |= (1 << INT3);
					break;
					
				case EXTERNAL_INTERRUPT_4:
					EICRB = (EICRB & ~((1 << ISC40) | (1 << ISC41))) | (InterruptMode << ISC40);
					EIMSK |= (1 << INT4);
					break;
					
				case EXTERNAL_INTERRUPT_5:
					EICRB = (EICRB & ~((1 << ISC50) | (1 << ISC51))) | (InterruptMode << ISC50);
					EIMSK |= (1 << INT5);
					break;
					
				case EXTERNAL_INTERRUPT_6:
					EICRB = (EICRB & ~((1 << ISC60) | (1 << ISC61))) | (InterruptMode << ISC60);
					EIMSK |= (1 << INT6);
					break;
					
				case EXTERNAL_INTERRUPT_7:
					EICRB = (EICRB & ~((1 << ISC70) | (1 << ISC71))) | (InterruptMode << ISC70);
					EIMSK |= (1 << INT7);
					break;
#endif
			}
			ExternalInterruptCallbackStructArray[ExternalInterruptNumber].ThisFunctionPointer =
				FunctionPointer;
				
				return (true);
		}
	}
	else
	{
		return (false);
	}
}

bool DetachAndDisableExternalInterrupt(uint8_t ExternalInterruptNumber)
{
	if (ExternalInterruptNumber < NUMBER_OF_EXTERNAL_INTRRUPTS)
	{
		if (!(NoCallbackIsPerformedOnExternalInterrupt == ExternalInterruptCallbackStructArray[ExternalInterruptNumber].ThisFunctionPointer))
		{
			switch (ExternalInterruptNumber)
			{
				case EXTERNAL_INTERRUPT_0:
					EIMSK &= ~(1 << INT0);
					break; 
					
				case EXTERNAL_INTERRUPT_1:
					EIMSK &= ~(1 << INT1);
					break;
					
#if defined (_AVR_IOM2560_H_)					
				case EXTERNAL_INTERRUPT_2:
					EIMSK &= ~(1 << INT2);
					break;
					
				case EXTERNAL_INTERRUPT_3:
					EIMSK &= ~(1 << INT3);
					break;
					
				case EXTERNAL_INTERRUPT_4:
					EIMSK &= ~(1 << INT4);
					break;
					
				case EXTERNAL_INTERRUPT_5:
					EIMSK &= ~(1 << INT5);
					break;
					
				case EXTERNAL_INTERRUPT_6:
					EIMSK &= ~(1 << INT6);
					break;
					
				case EXTERNAL_INTERRUPT_7:
					EIMSK &= ~(1 << INT7);
					break;
#endif
			}
			ExternalInterruptCallbackStructArray[ExternalInterruptNumber].ThisFunctionPointer =
			  NoCallbackIsPerformedOnExternalInterrupt;
			  
			  return (true);
		}
		else
		{
			return (false);
		}
	}
	else
	{
		return (false);
	}
}

ISR(INT0_vect)
{
	ExternalInterruptCallbackStructArray[0].NumberOfExternalInterruptsOnPin++;
	ExternalInterruptCallbackStructArray[0].ThisFunctionPointer(ExternalInterruptCallbackStructArray[0].NumberOfExternalInterruptsOnPin); 	
}

ISR(INT1_vect)
{
	ExternalInterruptCallbackStructArray[1].NumberOfExternalInterruptsOnPin++;
	ExternalInterruptCallbackStructArray[1].ThisFunctionPointer(ExternalInterruptCallbackStructArray[1].NumberOfExternalInterruptsOnPin);
}

#if defined (_AVR_IOM2560_H_)
ISR(INT2_vect)
{
	ExternalInterruptCallbackStructArray[2].NumberOfExternalInterruptsOnPin++;
	ExternalInterruptCallbackStructArray[2].ThisFunctionPointer(ExternalInterruptCallbackStructArray[2].NumberOfExternalInterruptsOnPin);
}

ISR(INT3_vect, ISR_BLOCK ISR_NAKED)
{
	ExternalInterruptCallbackStructArray[3].NumberOfExternalInterruptsOnPin++;
	ExternalInterruptCallbackStructArray[3].ThisFunctionPointer(ExternalInterruptCallbackStructArray[3].NumberOfExternalInterruptsOnPin);
}

ISR(INT4_vect, ISR_BLOCK ISR_NAKED)
{
	ExternalInterruptCallbackStructArray[4].NumberOfExternalInterruptsOnPin++;
	ExternalInterruptCallbackStructArray[4].ThisFunctionPointer(ExternalInterruptCallbackStructArray[4].NumberOfExternalInterruptsOnPin);
}

ISR(INT5_vect)
{
	ExternalInterruptCallbackStructArray[5].NumberOfExternalInterruptsOnPin++;
	ExternalInterruptCallbackStructArray[5].ThisFunctionPointer(ExternalInterruptCallbackStructArray[5].NumberOfExternalInterruptsOnPin);
}

ISR(INT6_vect)
{
	ExternalInterruptCallbackStructArray[6].NumberOfExternalInterruptsOnPin++;
	ExternalInterruptCallbackStructArray[6].ThisFunctionPointer(ExternalInterruptCallbackStructArray[6].NumberOfExternalInterruptsOnPin);
}

ISR(INT7_vect)
{
	ExternalInterruptCallbackStructArray[7].NumberOfExternalInterruptsOnPin++;
	ExternalInterruptCallbackStructArray[7].ThisFunctionPointer(ExternalInterruptCallbackStructArray[7].NumberOfExternalInterruptsOnPin);
}
#endif
