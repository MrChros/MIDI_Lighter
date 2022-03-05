/*
 * LED_Strip.c
 *
 * Created: 16/09/2018 09:50:55
 *  Author: Chris
 */ 

// ============================================================================================
#include <avr/io.h>
#include <avr/interrupt.h>

#include "Pin.h"
#include "LED_Strip.h"
#include "Configuration.h"


// ============================================================================================
#define RED					COLOR_RED
#define GREEN				COLOR_GREEN
#define BLUE				COLOR_BLUE

#define DEFAULT_PRESCALER	PRESCALER_4

// ============================================================================================
volatile uint8_t			_Color[3];


/*******************************************************************
	Interrupt Service Routines
*******************************************************************/
ISR(TIMER4_OVF_vect)
{
	switch(Configuration_Get_RGB_Order())
	{
		case RGB:	OCR4A = _Color[RED];
					OCR4B = _Color[GREEN];
					OCR4D = _Color[BLUE];
			break;
		case RBG:	OCR4A = _Color[RED];
					OCR4B = _Color[BLUE];
					OCR4D = _Color[GREEN];
			break;
		case GRB:	OCR4A = _Color[GREEN];
					OCR4B = _Color[RED];
					OCR4D = _Color[BLUE];
			break;
		case GBR:	OCR4A = _Color[GREEN];
					OCR4B = _Color[BLUE];
					OCR4D = _Color[RED];
			break;
		case BRG:	OCR4A = _Color[BLUE];
					OCR4B = _Color[RED];
					OCR4D = _Color[GREEN];
			break;
		case BGR:	OCR4A = _Color[BLUE];
					OCR4B = _Color[GREEN];
					OCR4D = _Color[RED];
			break;
	}
}

/*******************************************************************
	Initialization
*******************************************************************/
void LED_Strip_Init(void)
{
	DDRF |= (1 << STRIP_PIN_EN);
	STRIP_ENABLE();
	

	OCR4C = 255;
	LED_Strip_Update(0, 0, 0);
		
	
	/////////////////////////////////
	// PWM Timer (Timer 4; 10 Bit) //
	/////////////////////////////////
	PLLFRQ |=	(1 << PLLTM0);			// PLL Postscaler Factor for High-Speed Timer = 1 (see Bits 5..4 in chapter 6.10.3)
										// Output frequency of the PLL is 48 MHz (see Bits 3..0 in chapter 6.10.3)

	TCCR4C =	(0b10 << COM4D0) |		// OC4D: Cleared on Compare Match. Set when TCNT4 = 0x000. (see Table 15-17)
				(1    << PWM4D);		// When set (one) this bit enables PWM mode based on comparator OCR4D
	
	TCCR4A |=	(0b10 << COM4A0) |		// OC4A: Cleared on Compare Match. Set when TCNT4 = 0x000. (see Table 15-7)
				(0b10 << COM4B0) |		// OC4B: Cleared on Compare Match. Set when TCNT4 = 0x000. (see Table 15-12)
				(1	  << PWM4A)	 |		// When set (one) this bit enables PWM mode based on comparator OCR4A
				(1	  << PWM4B);		// When set (one) this bit enables PWM mode based on comparator OCR4B

	TCNT4  =	0;						// Reset Count Value
	TIMSK4 =	(1 << TOIE4);			// Enable Overflow Interrupt
	
	LED_Strip_Set_Clock_Prescaler(DEFAULT_PRESCALER);

	DDRC  |=	(1 << DDC7);			// Set Output Pin: OCRA, Red
	DDRB  |=	(1 << DDB6);			// Set Output Pin: OCRB, Green
	DDRD  |=	(1 << DDD7);			// Set Output Pin: OCRD, Blue
}

void LED_Strip_Update(uint8_t red, uint8_t green, uint8_t blue)
{
	_Color[RED]		= red;
	_Color[GREEN]	= green;
	_Color[BLUE]	= blue;
}

void LED_Strip_Set_Clock_Prescaler(uint8_t prescaler)
{
	if(prescaler >= PRESCALER_1 && prescaler <= PRESCALER_16384)
	{
		TCCR4B = 0;						// Stop Timer 4
		TCNT4  = 0;						// Reset Count Value
		TCCR4B = (prescaler << CS40);	// Start Timer 4 with new Prescaler (see Table 15-15)
	}
}