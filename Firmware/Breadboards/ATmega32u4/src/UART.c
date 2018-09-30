/*
 * UART.c
 *
 * Created: 02.06.2018 12:48:41
 *  Author: Chris
 */ 

#define F_CPU	16000000UL

#include <avr/interrupt.h>

#include "UART.h"

/////////////
// Defines //
/////////////
#define UART_BUFFER_SIZE	500
#define BAUD				31250UL		// Baud rate

#define UBRR_VAL ((F_CPU+BAUD*8)/(BAUD*16)-1)
#define BAUD_REAL (F_CPU/(16*(UBRR_VAL+1)))
#define BAUD_ERROR ((BAUD_REAL*1000)/BAUD)

#if ((BAUD_ERROR<990) || (BAUD_ERROR>1010))
	#error Systematischer Fehler der Baudrate groesser 1% und damit zu hoch!
#endif


///////////////
// Datatypes //
///////////////
struct UART_BUFFER_s
{
	uint8_t data[UART_BUFFER_SIZE];
	uint8_t	read;
	uint8_t	write;
} _UART_Buffer;


////////////////////////
// Interrupt Handlers //
////////////////////////
ISR(USART1_RX_vect)
{
	_UART_Buffer.data[_UART_Buffer.write++] = UDR1;		// Fetch the received byte value
	
	if(_UART_Buffer.write == UART_BUFFER_SIZE)
	{
		_UART_Buffer.write = 0;
	}
}


///////////////
// Functions //
///////////////
void UART_Init(void)
{
	// Set Baud Rate
	UBRR1H = (unsigned char)(UBRR_VAL >> 8);
	UBRR1L = (unsigned char)(UBRR_VAL);
	
	// Enable receiver and transmitter
	UCSR1B = (1 << RXEN1) | (1 << TXEN1);
	
	// Set frame format: 8 data, 1 stop bit
	UCSR1C  = (0 << USBS1) | (3 << UCSZ10);
	UCSR1B |= (1 << RXCIE1);
	
	_UART_Buffer.read	= 0;
	_UART_Buffer.write	= 0;
}

void UART_Send_Char(char data)
{
	// Wait for empty transmit buffer
	while(!( UCSR1A & (1 << UDRE1)));
	
	// Put data into buffer, sends the data
	UDR1 = data;
}

void UART_Send_String(char* data, int length)
{
	int i;
	for(i=0;i<length;i++)
	{
		UART_Send_Char(data[i]);
	}
}

uint8_t UART_Data_Available(void)
{
	if(_UART_Buffer.write >= _UART_Buffer.read)
	{
		return (_UART_Buffer.write - _UART_Buffer.read);
	}
	else
	{
		return ((_UART_Buffer.write + UART_BUFFER_SIZE) - _UART_Buffer.read);
	}
}

uint8_t	UART_Buffer_Read(void)
{
	uint8_t Temp = 0;
	
	if(UART_Data_Available()>0)
	{
		Temp = _UART_Buffer.data[_UART_Buffer.read++];
		
		if(_UART_Buffer.read == UART_BUFFER_SIZE)
		{
			_UART_Buffer.read = 0;
		}
	}
	
	return Temp;
}