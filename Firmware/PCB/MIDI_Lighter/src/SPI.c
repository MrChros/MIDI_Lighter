/*
 * SPI.c
 *
 * Created: 07/10/2018 13:51:52
 *  Author: Chris
 */ 

// ============================================================================================
#include <avr/interrupt.h>

#include "SPI.h"
#include "Pin.h"

// ============================================================================================
#define FALSE		0
#define TRUE		1

// ============================================================================================
volatile uint8_t _SPI_Initialized = FALSE;


// ============================================================================================
void SPI_Send_Byte(uint8_t data);

/*******************************************************************
	Interrupt Service Routines
*******************************************************************/


/*******************************************************************
	Initialization
*******************************************************************/
void SPI_Init(void)
{
	DEBUG_DDR_SCK	|= (1 << DEBUG_PIN_SCK);
	DEBUG_DDR_MOSI	|= (1 << DEBUG_PIN_MOSI);
	DEBUG_DDR_SS	|= (1 << DEBUG_PIN_SS);
	
	DEBUG_ON(DEBUG_PORT_SS, DEBUG_PIN_SS);
	
	
	SPCR =	(0 << SPIE) |	// SPI Interrupt Enable
			(1 <<  SPE) |	// SPI Enable
			(0 << DORD) |	// Data Order (0 = MSB first)
			(1 << MSTR) |	// Master/Slave Select (1 = Master)
			(0 << CPOL) |	// Clock Polarity
			(0 << CPHA) |	// Clock Phase
			(0 << SPR0);	// SPI Clock Rate Select (0 = F_CPU/4 = 4 MHz)

	 _SPI_Initialized = TRUE;
}


/*******************************************************************
	Internal Functions
*******************************************************************/
void SPI_Send_Byte(uint8_t data)
{
	SPDR = data;
	while(!(SPSR & (1<<SPIF)));
}


/*******************************************************************
	Interface functions
*******************************************************************/


void SPI_Send_String(char* data, int length)
{
	if(_SPI_Initialized == FALSE)
	{
		return;
	}
	DEBUG_OFF(DEBUG_PORT_SS, DEBUG_PIN_SS);

	int i;
	for(i=0;i<length;i++)
	{
		SPI_Send_Byte(data[i]);
	}

	DEBUG_ON(DEBUG_PORT_SS, DEBUG_PIN_SS);
}