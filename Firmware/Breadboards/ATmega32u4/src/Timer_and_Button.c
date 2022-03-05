/*
 * Timer.c
 *
 * Created: 02/06/2018 18:43:05
 *  Author: Chris
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

#include "Timer_and_Button.h"


#define TRUE							1
#define FALSE							0

#define PRESSED							FALSE
#define RELEASED						TRUE
#define BUTTON_SHORT_LONG_THRESHOLD		10
#define NO_DATA_LIGHT_TIMEOUT_S			3


#define LED_ON( _PORT_, _PIN_)		_PORT_ &= ~(1 << _PIN_)
#define LED_OFF( _PORT_, _PIN_)		_PORT_ |= (1 << _PIN_)
#define LED_TOGGLE( _PORT_, _PIN_)	_PORT_ ^= _BV(_PIN_)


volatile uint8_t	_Timer_WS2801_OV_Occurred;
volatile uint8_t	_Timer_Button_Counter;
volatile uint8_t	_Button_Short_Pressed, _Button_Long_Pressed;
volatile uint8_t	_Button_Short_State;
volatile uint8_t	_On_LED_Blink_Counter, _On_LED_Blink;

volatile uint8_t	_No_Data_Light_On;
volatile uint16_t	_No_Data_Light_Counter;


ISR(TIMER3_COMPA_vect)
{
	_Timer_WS2801_OV_Occurred = TRUE;
	
	if(_No_Data_Light_On == FALSE)
	{
		_No_Data_Light_Counter++;
	}
	
	if(_No_Data_Light_Counter == 200 * NO_DATA_LIGHT_TIMEOUT_S)
	{
		_No_Data_Light_On = TRUE;
	}
	
	if(_On_LED_Blink == TRUE) { return; }
	
	_On_LED_Blink_Counter++;
	
	if(_On_LED_Blink_Counter == 255)
	{
		_On_LED_Blink			= TRUE;
		_On_LED_Blink_Counter	= 0;
	}
}

ISR(TIMER1_COMPA_vect)
{
	uint8_t Button_Value = (PIND & 0x01);
	
	if(Button_Value == PRESSED)
	{
		if(_Timer_Button_Counter < 255)
		{
			_Timer_Button_Counter++;
		}
	}
	else
	{
		Timer_Button_Stop();	
		
		if(_Timer_Button_Counter>0)
		{
			if(_Timer_Button_Counter<BUTTON_SHORT_LONG_THRESHOLD)
			{
				_Button_Short_Pressed = TRUE;
				_Button_Short_State  ^= 0x01;
			}
			else
			{
				_Button_Long_Pressed = TRUE;
			}
		}
	}
}

ISR(INT0_vect)
{
	if(TCCR1B == 0)
	{
		_Timer_Button_Counter	= 0;
		_Button_Short_Pressed	= FALSE;
		_Button_Long_Pressed	= FALSE;
				
		Timer_Button_Start();
	}
}

void Timer_And_Button_Init(void)
{
	_Timer_WS2801_OV_Occurred	= FALSE;
	_On_LED_Blink_Counter		= 0;
	_On_LED_Blink				= FALSE;
	
	_No_Data_Light_On			= FALSE;
	_No_Data_Light_Counter		= 0;
	
	//////////////////////////////////////////
	// WS2801 Timer (Timer 0; 8 Bit)
	TCCR3B |= (1 << WGM32);					// CTC Mode
	OCR3A	= 78;							// Compare value (78 = ~5ms)
	TCNT3	= 0;							// Reset Count Value
	TIMSK3 |= (1 << OCIE3A);				// Enable overflow interrupt
	TCCR3B |= (1 << CS32) | (1 << CS30);	// Enable Timer with Prescaler of 1024
	
	
	
	_Timer_Button_Counter	= 0;
	_Button_Short_Pressed	= FALSE;
	_Button_Short_State		= FALSE;
	_Button_Long_Pressed	= FALSE;
	
	//////////////////////////////////////////
	// Button Timer (Timer 1; 16 Bit)
	OCR1A	= 1562;							// Compare value (1562 = ~100ms)
	TIMSK1 |= (1 << OCIE1A);				// Enable overflow interrupt
	
	
	//////////////////////////////////////////
	// Button Interrupt Input
	
	PORTD |= (1 << PD0);					// Activate Internal Pull-up
	EICRA |= (1 << ISC01);					// Interrupt on Falling Edges
	EIMSK |= (1 << INT0);					// Enable Interrupt in INT0
}

uint8_t Timer_WS2801_OV_Occurred(void)
{
	uint8_t Temp = _Timer_WS2801_OV_Occurred;

	_Timer_WS2801_OV_Occurred = FALSE;
	
	return Temp;
}

void Timer_WS2801_Restart(void)
{
	TCNT3 = 0;
}

uint8_t	Timer_WS2801_On_LED_Blick(void)
{
	uint8_t Temp = _On_LED_Blink;
	
	_On_LED_Blink = FALSE;
	
	return Temp;	
}

void Timer_Button_Start(void)
{
	TCNT1	= 0;										// Reset Count Value		
	TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10);	// Enable Timer with Prescaler of 1024 in CTC Mode
}

void Timer_Button_Stop(void)
{
	TCCR1B	= 0;										// Stop Timer
}

uint8_t Button_Short_Pressed(void)
{
	uint8_t Temp = _Button_Short_Pressed;
	
	_Button_Short_Pressed = FALSE;
	
	return Temp;
}

uint8_t	Button_Short_State(void)
{
	return _Button_Short_State;
}

uint8_t Button_Long_Pressed(void)
{
	uint8_t Temp = _Button_Long_Pressed;
	
	_Button_Long_Pressed = FALSE;
	
	return Temp;
}

void Timer_No_Data_Light_Reset(void)
{
	_No_Data_Light_Counter	= 0;
	_No_Data_Light_On		= FALSE;
}

uint8_t	Timer_No_Data_Light_On(void)
{
	return _No_Data_Light_On;
}