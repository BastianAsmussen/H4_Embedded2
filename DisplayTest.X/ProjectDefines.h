/*
 * ProjectDefines.h
 *
 * Created: 23-11-2023 22:50:48
 *  Author: ltpe
 */ 


#ifndef PROJECTDEFINES_H_
#define PROJECTDEFINES_H_

#include <avr/pgmspace.h>
#include <stdbool.h>

#define SizeOfArray(arr)  (sizeof(arr)/sizeof(arr[0]))

#define TimeoutValueInSecondsForExternalInterruptToOccurAgain 10
#define TimeoutValueInSecondsForAdafruitDisplatToStartup 2
#define ShouldStateMachineGoBackToInitStateOnUserTypeErrors true

typedef void (*Function_Pointer_With_One_Char_Parameter)(char);
typedef void (*Function_Pointer)(char *);
typedef void (*Function_Pointer_With_One_Uint32_t_Parameter)(uint32_t);

#define DisplayBufferSize	64 

typedef struct
{
	volatile char DisplayBuffer[DisplayBufferSize];
	volatile uint8_t BufferHead;
	volatile uint8_t BufferTail;
	volatile uint16_t BufferOverrunCounter;
	volatile bool SkippedCharacterIndicator;
	volatile char SkippedCharacter;
} RingBufferStruct;

typedef enum
{
	STATE_MACHINE_INIT_STATE,
	STATE_MACHINE_RECEIVING_ADDRESS,
	STATE_MACHINE_RECEIVING_BIT_POSITION,
	STATE_MACHINE_RECEIVING_BIT_VALUE,
	//STATE_MACHINE_CONVERT_RECEIVED_CHARACTERS,
	//STATE_MACHINE_WRITE_FUNNY_TEXT,
	STATE_MACHINE_RECEIVING_DISPLAY_SETUP_TIME,
	STATE_MACHINE_RECEIVING_EXTERNAL_INTERRUPT_LOCKOUT_TIME,
	STATE_MACHINE_UNKNOWN_STATE
} StateMachineStates;

typedef enum
{
	ERROR_WRONG_CHARACTER_TYPED,
	ERRROR_WRONG_ADDRESS,
	ERROR_WRONG_BIT_POSITION,
	ERROR_WRONG_BIT_VALUE
} ErrorStates;

typedef struct
{
	StateMachineStates WorkingState;
	Function_Pointer ThisFunctionPointer;
	PGM_P ptrToFlashStatusString;
} StateMachineStruct;

#define MaxCharactersInTimeoutString 2
#define MaxCharactersInAddressString 4
#define MaxCharactersInDisplayInformationString 4
#define NumberOfBitsInAddresses	8
#define MaxNumberOfDigitsInString 8

typedef struct
{
	volatile uint16_t Address;
	volatile char Address_String[MaxCharactersInAddressString + 1];
	volatile uint8_t Address_StringPointer;
	volatile uint8_t Bit_Position;
	volatile uint8_t Bit_State;
	volatile uint8_t *Address_Pointer;
	volatile char AddressBinaryContent_String[NumberOfBitsInAddresses + 1];
} AddressStruct;

typedef struct 
{
	volatile char DisplaySetupTime_String[MaxCharactersInTimeoutString + 1];
	volatile uint8_t DisplaySetupTime_StringPointer;
} DisplayTimeSetupTimeStruct;

typedef struct
{
	volatile char ExternalInterruptLockoutTime_String[MaxCharactersInTimeoutString + 1];
	volatile uint8_t ExternalInterruptLockoutTime_StringPointer;
} ExternalInterruptLockoutTimeStruct;

#define Upper_Lower_Bit_Position 5
#define Upper_Lower_Bit_Value (1 << Upper_Lower_Bit_Position)

#define Max_Number_Of_Characters_In_String 256

#define Write_Funny_Text_Character ':'
#define String_End_Character '\0'

#define MAX_NUMBER_OF_LINES_IN_DISPLAY	8
#define AddressLineInDisplay 1
#define BitPositionLineInDisplay AddressLineInDisplay + 1
#define BitValueLineInDisplay AddressLineInDisplay + 2
#define AddressContentLineInDisplay	AddressLineInDisplay + 3
#define ExternailInterruptCounterLineInDisplay	AddressLineInDisplay + 5
#define TimeoutValueCounterLineInDisplay AddressLineInDisplay + 6

#define XCoordinateStartPositionInDisplay 1

#define MinNumberOfCharactersInString 2

#define BitNotSetCharacter '0'
#define BitSetCharacter '1'

#define CharToIntConverterCharacter '0'

#define YellowLedBit 	1
#define GreenLedBit	    2
#define RedLedBit		3

#define GreenLedBitValue (1 << GreenLedBit)
#define YellowLedBitValue (1 << YellowLedBit)
#define RedLedBitValue (1 << RedLedBit)

#endif /* PROJECTDEFINES_H_ */