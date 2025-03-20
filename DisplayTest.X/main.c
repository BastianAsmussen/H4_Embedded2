/*
 * PointerUsage.c
 *
 * Created: 30-04-2020 14:11:58
 * Author : ltpe
 */ 
#define DisplayTestCode

#include <avr/io.h>
#include <stdbool.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "ProjectDefines.h"
#include "ssd1306.h"
#include "UART.h"
#include "Timer.h"
#include "Timer0.h"
#include "ExternalInterrupt.h"

static volatile uint8_t DisplaySetupTimeInRAM;
static volatile uint8_t ExternalInterruptLockoutTimeInRAM;
static volatile bool GoBackToInitStateOnTypeErrorsInRAM;

static volatile StateMachineStates StateMachineState = STATE_MACHINE_RECEIVING_ADDRESS;
static volatile bool PrintOutInfo = false;
static volatile AddressStruct ThisAddressStruct;
static volatile DisplayTimeSetupTimeStruct ThisDisplayTimeSetupTimeStruct;
static volatile ExternalInterruptLockoutTimeStruct ThisExternalInterruptLockoutTimeStruct;

static volatile uint8_t I2CAddress = SSD1306_ADDR;
static volatile uint8_t I2CDisplayStatus = SSD1306_SUCCESS;
static volatile uint8_t I2CDisplayStatusOperation = SSD1306_SUCCESS;
static volatile bool I2CDisplayErrorFound = false;

static volatile uint16_t NumberOfExternalInterruptsOnPin0 = 0;
static volatile uint16_t NumberOfExternalInterruptsOnPin0FromCallback = 0;

static volatile bool DisplayStartUpWaitTime = false;
static volatile bool DisplayErrorTestTimeout = false;

static volatile uint32_t NumberOfTimerInterrupts = 0;

void ExternalInterruptFunction_0(uint16_t NumberOfExternalInterruptsOnPin0FromCallbackHere)
{
	NumberOfExternalInterruptsOnPin0++;
	NumberOfExternalInterruptsOnPin0FromCallback = NumberOfExternalInterruptsOnPin0FromCallbackHere;
	PrintOutInfo = true;
}

uint16_t adc_read(uint8_t channel) {
    channel &= 0x07; // Ensure channel is within 0-7
    ADMUX = (ADMUX & 0xF8) | channel; // Select ADC channel
    ADCSRA |= (1 << ADSC); // Start conversion
    while (ADCSRA & (1 << ADSC)); // Wait for conversion to complete
    return ADC;
}

void adc_init() {
    ADMUX = (1 << REFS0); // Reference voltage is AVcc
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1); // Enable ADC and set prescaler to 64
}

// Function to initialize Timer 2 in Fast PWM mode
void timer2_pwm_init()
{
    // Set the PWM pin as output (Assuming LED is connected to PD3/OC2B)
    DDRD |= (1 << PD3);
    // Configure Timer 2 for Fast PWM mode, non-inverting mode
    // COM2A1 => Clear OC2A on compare match : Tabel 21-4
    // COM2B1 => Clear OC2B on compare match : Tabel 21-7
    // WGM20 | WGM21 => Fast PWM, Top 0xFF, update of OCR2x at Bottom
    TCCR2A = (1 << COM2A1) | (1 << COM2B1) | (1 << WGM20) | (1 << WGM21);
    // CS22 | CS20 => Prescaler = 128
    TCCR2B = (1 << CS22) | (1 << CS20); // Prescaler = 128
    OCR2A = 0; // LTPE
    OCR2B = 0;
    TCNT2 = 0;
}

void TreatUartReceivedCharAccordingToState(char InputChar)
{
    
}

void ReceiveTimeOutValueCounterInit(uint32_t TimeOutValueCounter)
{
	DisplaySetupTimeInRAM--;
		
	if (0 == DisplaySetupTimeInRAM)
	{
		Disable_Timer0_Overflow_Interrupt();
		PORTB &= ~(YellowLedBitValue);
		DisplayStartUpWaitTime = true;
	}
	else
	{
		printf("%d sekunder tilbage til Adafruit Display Opstart\n", DisplaySetupTimeInRAM);
	}
}

void ReceiveTimeoutValueErrorTest(uint32_t TimeOutValueCounter)
{
	DisplayErrorTestTimeout = true;
}

void ReceiveTimerSecondsValue(uint32_t TimeOutValueCounter)
{
    NumberOfTimerInterrupts = TimeOutValueCounter;
    PrintOutInfo = true;
}

void PrintDisplayCodeOnUART(uint8_t DisplayCode)
{
	char DisplayCodeString[MaxCharactersInDisplayInformationString];
	
	sprintf(DisplayCodeString, "0x%x", DisplayCode);
	printf("Display Code : %s\n", DisplayCodeString);
}

void TestI2CDisplayStatus(uint8_t I2CDisplayStatus)
{
    if (SSD1306_SUCCESS != I2CDisplayStatus)
    {
        PrintDisplayCodeOnUART(I2CDisplayStatus);
    }
}

ISR(TWI0_vect)
{
	static volatile uint8_t test = 10;
}

int main(void)
{
    char TimerCounterString[6];
    char InterruptCounterString[6];
    char ADCounterString[6];
    
    AttachAndEnableExternalInterrupt(EXTERNAL_INTERRUPT_0, ExternalInterruptFunction_0, FALLING_EDGE_GENERATE_INTERRUPT);
    
	DDRB |= YellowLedBitValue;
	RS232Init();
    
    Enable_UART_Receive_Interrupt();
    adc_init(); // Initialize ADC
    timer2_pwm_init ();
	
	DisplaySetupTimeInRAM = TimeoutValueInSecondsForAdafruitDisplatToStartup;;
	printf("\nVenter i %d sekunder på Adafruit Display Opstart\n", DisplaySetupTimeInRAM);
	
	sei(); // Enable the Global Interrupt Enable flag so that interrupts can be processed
	
	if (DisplaySetupTimeInRAM > 0)
	{
		Setup_Timer0_Overflow_Interrupt(VariableValue1SecValue8Bit, ReceiveTimeOutValueCounterInit);
		Enable_Timer0_Overflow_Interrupt();
		
		while (false == DisplayStartUpWaitTime)
		{
			// Wait here for the specified time in ProjectDefined.h before trying
			// to start up the AdaFruit Display.
			// I we try to start up the AdaFruit Display to soon after initialization
			// we might face some problems !!!
		}
	}
	
	SetupUARTFunctionCallbackPointer(TreatUartReceivedCharAccordingToState);
	Enable_UART_Receive_Interrupt();
	
	printf("\n\nNu starter vi med at teste Displayet !!!\n");
	
	Setup_Timer0_Overflow_Interrupt(VariableValue1SecValue8Bit, ReceiveTimeoutValueErrorTest);
	Enable_Timer0_Overflow_Interrupt();
	do 
	{
		DisplayErrorTestTimeout = false;
		
		while (false == DisplayErrorTestTimeout)
		{
			// Wait here for the specified time in ProjectDefined.h before trying
			// to start up the AdaFruit Display.
			// I we try to start up the AdaFruit Display to soon after initialization
			// we might face some problems !!!
		}
		I2CDisplayStatus = SSD1306_Init(I2CAddress);
		if (SSD1306_SUCCESS != I2CDisplayStatus)
		{
			printf("Display NOT properly initialized !!!\n");
			PrintDisplayCodeOnUART(I2CDisplayStatus);
		}
		else
		{
			printf("Display properly initialized i DisplayTestCode !!!\n");
			PrintDisplayCodeOnUART(I2CDisplayStatus);
		}
	} while (SSD1306_SUCCESS != I2CDisplayStatus);
	Disable_Timer0_Overflow_Interrupt();
	
	I2CDisplayStatusOperation = SSD1306_ClearScreen();
    if (SSD1306_SUCCESS != I2CDisplayStatusOperation)
    {
        printf("Fejl ved funktionen : SSD1306_ClearScreen\n");
        PrintDisplayCodeOnUART(I2CDisplayStatusOperation);
        I2CDisplayErrorFound = true;
    }
    
    I2CDisplayStatusOperation = SSD1306_SetPosition(XCoordinateStartPositionInDisplay, AddressLineInDisplay);
    if (SSD1306_SUCCESS != I2CDisplayStatusOperation)
    {
        printf("Fejl ved funktionen : SSD1306_SetPosition(%d,%d)\n", XCoordinateStartPositionInDisplay, AddressLineInDisplay);
        PrintDisplayCodeOnUART(I2CDisplayStatusOperation);
        I2CDisplayErrorFound = true;
    }
    
    I2CDisplayStatusOperation = SSD1306_DrawString("h4pd011125 styrer", NORMAL);
    if (SSD1306_SUCCESS != I2CDisplayStatusOperation)
    {
        printf("Fejl ved funktionen : SSD1306_DrawString('Waiting !!!', NORMAL)\n");
        PrintDisplayCodeOnUART(I2CDisplayStatusOperation);
        I2CDisplayErrorFound = true;
    }

    if (false == I2CDisplayErrorFound)
    {
        printf("Display properly initialized og ingen fejl herefter !!!\n");
    }
    else
    {
        printf("Display properly initialized men fejl herefter !!!\n");
    }
    
    Setup_Timer0_Overflow_Interrupt(VariableValue1SecValue8Bit, ReceiveTimerSecondsValue);
	Enable_Timer0_Overflow_Interrupt();
		
//    I2CDisplayStatusOperation = SSD1306_ClearScreen();
//    I2CDisplayStatusOperation = SSD1306_SetPosition(XCoordinateStartPositionInDisplay, AddressLineInDisplay);
//    I2CDisplayStatusOperation = SSD1306_DrawString("h4pd011125 styrer !!!", NORMAL);
    
	/* Replace with your application code */
    while (1) 
    {
		if (true == PrintOutInfo)
        {
//            PrintOutInfo = false;  
//            I2CDisplayStatusOperation = SSD1306_SetPosition(XCoordinateStartPositionInDisplay, AddressLineInDisplay + 2);
//            TestI2CDisplayStatus(I2CDisplayStatusOperation);
//            sprintf(TimerCounterString, "%d", NumberOfTimerInterrupts);
//            I2CDisplayStatusOperation = SSD1306_DrawString(TimerCounterString, NORMAL);
//            TestI2CDisplayStatus(I2CDisplayStatusOperation);
//            printf("TimerCounter : %d\n", NumberOfTimerInterrupts);
            
            
            PrintOutInfo = false;
            I2CDisplayStatusOperation = SSD1306_SetPosition(XCoordinateStartPositionInDisplay, AddressLineInDisplay + 2);
            TestI2CDisplayStatus(I2CDisplayStatusOperation);

            sprintf(TimerCounterString, "Timer > %d", NumberOfTimerInterrupts);
            I2CDisplayStatusOperation = SSD1306_DrawString(TimerCounterString, NORMAL);

            TestI2CDisplayStatus(I2CDisplayStatusOperation);

            I2CDisplayStatusOperation = SSD1306_SetPosition(XCoordinateStartPositionInDisplay, AddressLineInDisplay + 3);

            TestI2CDisplayStatus(I2CDisplayStatusOperation);

            sprintf(InterruptCounterString,"Clicks > %d", NumberOfExternalInterruptsOnPin0);
            I2CDisplayStatusOperation = SSD1306_DrawString(InterruptCounterString, NORMAL);
            
            I2CDisplayStatusOperation = SSD1306_SetPosition(XCoordinateStartPositionInDisplay, AddressLineInDisplay + 4);
            uint16_t adc_value = adc_read(0); // Read ADC value from channel 0
            sprintf(ADCounterString,"ADCounter > %d", adc_value);
            I2CDisplayStatusOperation = SSD1306_DrawString(ADCounterString, NORMAL);
            
            TestI2CDisplayStatus(I2CDisplayStatusOperation);
            printf("TimerCounter : %d\n", NumberOfTimerInterrupts);
            
        }
    }
}

