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



#define LED0				PC5
#define LED1				PC4
#define LED2				PC2
#define LEDON				PC0

#define LED_ON(_LED_)		PORTC &= ~(1 << _LED_)
#define LED_OFF(_LED_)		PORTC |= (1 << _LED_)
#define LED_TOGGLE(_LED_)	PORTC ^= _BV(_LED_)



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
ISR(USART_RXC_vect)
{
	_UART_Buffer.data[_UART_Buffer.write++] = UDR;		// Fetch the received byte value
	
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
	UBRRH = (unsigned char)(UBRR_VAL >> 8);
	UBRRL = (unsigned char)(UBRR_VAL);
	
	// Enable receiver and transmitter
	UCSRB = (1 << RXEN) | (1 << TXEN);
	
	// Set frame format: 8 data, 1 stop bit
	UCSRC  = (1 << URSEL) | (0 << USBS) | (1 << UCSZ1) | (1 << UCSZ0);
	UCSRB |= (1 << RXCIE);
	
	_UART_Buffer.read	= 0;
	_UART_Buffer.write	= 0;
}

void UART_Send_Char(char data)
{
	// Wait for empty transmit buffer
	while(!( UCSRA & (1 << UDRE)));
	
	// Put data into buffer, sends the data
	UDR = data;
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