/*
 * UART.c
 * Version: 1.0
 * Created: 25-02-2015 20:10:21
 * Author: alkj
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <ctype.h>

#include "ProjectDefines.h"
#include "UART.h"

//static FILE mystdInOut = FDEV_SETUP_STREAM(uart_putch, uart_getch, _FDEV_SETUP_RW);
// Hvis man fortrækker det, kan man også splitte sine streams op i en seperat stream i
// udgående retning og en seperat stream i indgående retning som vist herunder. Effekten
// er den samme !!!
static FILE uart_output = FDEV_SETUP_STREAM(uart_putch, NULL, _FDEV_SETUP_WRITE);
//static FILE uart_input = FDEV_SETUP_STREAM(NULL, uart_getch, _FDEV_SETUP_READ);

/* A helper macro is needed to expand AVR8_UART_NUMBER macro to the actual number,
   because macro arguments used with ## operator are not expanded, so 
   we can't pass AVR8_UART_NUMBER directly to this macro, but must do so via the 
   next macro - this expands the AVR8_UART_NUMBER to its value. */
#define 	MAKE_UART_NAME_(name1, number, name2) name1 ## number ## name2
/* create name of the register or bit based on */
#define		MAKE_UART_NAME(name1 , num, name2 ) MAKE_UART_NAME_(name1, num, name2 )

/* Create the actual registry and bit names for UART */
#define 	UCSRA	MAKE_UART_NAME(UCSR, AVR8_UART_NUMBER, A)
#define 	UCSRB	MAKE_UART_NAME(UCSR, AVR8_UART_NUMBER, B)
#define		UCSRC	MAKE_UART_NAME(UCSR, AVR8_UART_NUMBER, C)
#define		UBRRH	MAKE_UART_NAME(UBRR, AVR8_UART_NUMBER, H)
#define		UBRRL	MAKE_UART_NAME(UBRR, AVR8_UART_NUMBER, L)
#define		RXEN	MAKE_UART_NAME(RXEN, AVR8_UART_NUMBER, )
#define		TXEN	MAKE_UART_NAME(TXEN, AVR8_UART_NUMBER, )
#define		UCSZ0	MAKE_UART_NAME(UCSZ, AVR8_UART_NUMBER, 0)
#define		UCSZ1	MAKE_UART_NAME(UCSZ, AVR8_UART_NUMBER, 1)
#define		RXCIE	MAKE_UART_NAME(RXCIE, AVR8_UART_NUMBER, )
#define		UDRE	MAKE_UART_NAME(UDRE, AVR8_UART_NUMBER, )
#define		UDR		MAKE_UART_NAME(UDR, AVR8_UART_NUMBER, )
#define		RXC		MAKE_UART_NAME(RXC, AVR8_UART_NUMBER, )
#define		U2X		MAKE_UART_NAME(U2X, AVR8_UART_NUMBER, )

/* Define the USART vector name */
/* the other MCUs have USART number in the ISR name */
#define	MAKE_UART_VECT_(number)		USART ## number ## _RX_vect
#define	MAKE_UART_VECT(num)			MAKE_UART_VECT_(num)
#define	UART_ISR_VECTOR	MAKE_UART_VECT(AVR8_UART_NUMBER)	

static Function_Pointer_With_One_Char_Parameter Callback_Function_Pointer = NULL;

void SetupUARTFunctionCallbackPointer(Function_Pointer_With_One_Char_Parameter 
                                      Function_To_Callback)
{
	Callback_Function_Pointer = Function_To_Callback;
}

void RS232Init(void)
{
	// Set baud rate
	UBRRL = (unsigned char)(MyUBRR & 0xff);
	UBRRH = (unsigned char)(MyUBRR>>8);
	#ifdef UART_NORMAL_MODE
	UCSRA = 0x00;
	#else
	UCSRA = 1<<U2X0; // Set *2 mode
	#endif
	UCSRB = (1<<RXEN) | (1<<TXEN);  //0x18 , enable receive and transmit
	UCSRC =  (1<<UCSZ1) | (1<<UCSZ0); // 8 data bit 1 stop bit
	SetupOutputStreamToUart();
	//SetupInputStreamToUart();
}

void SetupOutputStreamToUart(void)
{
	stdout = &uart_output;
}

//void SetupInputStreamToUart(void)
//{
//	stdin = &uart_input;
//}

//int uart_getch(FILE* stream)
//{
//	while (!(UCSRA & (1<<RXC)))  ; // do nothing but wait
//	return (UDR);
//}

int uart_putch(char ch, FILE* stream)
{
	while( !(UCSRA & (1<<UDRE))) ;
	UDR = ch;
	return 0;
}

void Enable_UART_Receive_Interrupt()
{
	UCSRB |= (1 << RXCIE); // Enable the USART Recieve Complete interrupt (USART_RXC)
}

void Disable_UART_Receive_Interupt()
{
	UCSRB &= ~(1 << RXCIE); // Disable the USART Recieve Complete interrupt (USART_RXC)
}

#define Upper_Lower_Bit_Position 5
#define Upper_Lower_Bit_Value (1 << Upper_Lower_Bit_Position)


ISR(UART_ISR_VECTOR)
{
	char ReceivedChar;
	ReceivedChar = UDR;
	Callback_Function_Pointer(ReceivedChar);
	// Her bruges vi en pointer, når vi kalder funktionen: ConvertReceivedChar .
	// Dette er fordi, vi gerne vil have den konverterede værdi tilbage til her,
	// hvorfra vi har kaldt funktionen. Bruger vi ikke en pointer her, vil vi ikke se
	// ændringer lavet til vores parameter ReceivedChar lavet i funktionen:
	// ConvertReceivedChar.
	//ConvertReceivedChar(&ReceivedChar);
	//WriteReceivedCharacterFromUARTInDisplay(ReceivedChar);
	//Callback_Function_Pointer(ReceivedChar);
	UDR = ReceivedChar; // Echo back the received byte converted to the computer
	sei();
}



