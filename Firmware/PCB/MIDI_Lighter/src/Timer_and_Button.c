/*
 * Timer.c
 *
 * Created: 02/06/2018 18:43:05
 *  Author: Chris
 */ 

// ============================================================================================
#include <avr/io.h>
#include <avr/interrupt.h>

#include "Configuration.h"
#include "Timer_and_Button.h"


// ============================================================================================
#define TRUE							1
#define FALSE							0

#define PRESSED							FALSE
#define RELEASED						TRUE
#define BUTTON_SHORT_LONG_THRESHOLD		10

#define FACTOR_200MS					(uint16_t)40	// 40 * 5ms = 200ms (in this case: MS = milliseconds)

#define TIMER1_DEFAULT_TOP				2000			// ~1ms

////////////////////////
// Button Pin Defines //
////////////////////////
#define BUTTON_PIN_NO					PINE6
#define BUTTON_PIN_REG					PINE
#define BUTTON_PORT						PORTE

// ============================================================================================
volatile uint8_t	_Timer_General_OV_Occurred;
volatile uint8_t	_Timer_Button_Counter;
volatile uint8_t	_Button_Short_Pressed, _Button_Long_Pressed;
volatile uint8_t	_Button_Short_State;
volatile uint8_t	_On_LED_Blink_Counter, _On_LED_Blink;

volatile uint8_t	_No_Data_Light_On;
volatile uint16_t	_No_Data_Light_Counter;


/*******************************************************************
	Interrupt Service Routines
*******************************************************************/
ISR(TIMER0_COMPA_vect)
{
	// Set general flag, that 5ms have passed
	_Timer_General_OV_Occurred = TRUE;
	
	/* Counter to set flag, when to switch on "No data light".
	 * Counter is reset by local function "void	Timer_No_Data_Light_Reset(void)"
	 * Counter is multiple of 5ms
	 */
	if(_No_Data_Light_On == FALSE)
	{
		_No_Data_Light_Counter++;
	}
	
	if(_No_Data_Light_Counter >= FACTOR_200MS * (uint16_t)Configuration_Get_No_Data_Light_Timeout()) 
	{
		_No_Data_Light_On = TRUE;
	}
	

	/* Set flag, when to toggle the status LED
	 * If the flag is set, the counter will not be increased
	 * Compare value is set in the condition below the increase
	 * A value of 255 results in 255 * 5ms = 1.275 s
	 * In total one On-Off-Toggle of the status LED takes 2.55 s
	 */
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
	TIFR1 |= (1 << OCF1B);
}

ISR(TIMER3_COMPA_vect)
{
	uint8_t Button_Value = ((BUTTON_PIN_REG & (1 << BUTTON_PIN_NO)) > 0);
	
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

ISR(INT6_vect)
{
	if(TCCR3B == 0)
	{
		_Timer_Button_Counter	= 0;
		_Button_Short_Pressed	= FALSE;
		_Button_Long_Pressed	= FALSE;
				
		Timer_Button_Start();
	}
}


/*******************************************************************
	Initialization
*******************************************************************/
void Timer_And_Button_Init(void)
{
	_Timer_General_OV_Occurred	= FALSE;
	_On_LED_Blink_Counter		= 0;
	_On_LED_Blink				= FALSE;
	
	_No_Data_Light_On			= FALSE;
	_No_Data_Light_Counter		= 0;

	_Timer_Button_Counter		= 0;
	_Button_Short_Pressed		= FALSE;
	_Button_Short_State			= FALSE;
	_Button_Long_Pressed		= FALSE;

	////////////////////////////////////
	// General Timer (Timer 0; 8 Bit) //
	////////////////////////////////////
	// Description:
	// This Timer is used to generate a general 5ms Tick in
	// order to use it for LED Fades and ADC Trigger
	TCCR0A |= (1 << WGM01);					// CTC Mode
	OCR0A	= 78;							// Compare value (78 = ~5ms)
	TCNT0	= 0;							// Reset Count Value
	TIMSK0 |= (1 << OCIE0A);				// Enable overflow interrupt
	TCCR0B |= (1 << CS02) | (1 << CS00);	// Enable Timer with Prescaler of 1024


	/////////////////////////////////
	// ADC Timer (Timer 1; 16 Bit) //
	/////////////////////////////////
	TIMSK1	= (1 << OCIE1A);				// Interrupt on Compare Match A
	Timer_Set_Timer1_Top(TIMER1_DEFAULT_TOP);


	////////////////////////////////////
	// Button Timer (Timer 3; 16 Bit) //
	////////////////////////////////////
	OCR3A	= 1562;							// Compare value (1562 = ~100ms)
	TCNT3	= 0;							// Reset Count Value
	TIMSK3 |= (1 << OCIE3A);				// Enable overflow interrupt
	
	
	////////////////////////////
	// Button Interrupt Input //
	////////////////////////////
	BUTTON_PORT |= (1 << BUTTON_PIN_NO);	// Activate Internal Pull-up
	EICRB		|= (1 << ISC61);			// Interrupt on Falling Edges
	EIMSK		|= (1 << INT6);				// Enable Interrupt in INT6
}


/*******************************************************************
	Interface functions
*******************************************************************/
uint8_t Timer_General_OV_Occurred(void)
{
	uint8_t Temp = _Timer_General_OV_Occurred;

	_Timer_General_OV_Occurred = FALSE;
	
	return Temp;
}

void Timer_General_Restart(void)
{
	TCNT0 = 0;
}

uint8_t	Timer_General_On_LED_Blick(void)
{
	uint8_t Temp = _On_LED_Blink;
	
	_On_LED_Blink = FALSE;
	
	return Temp;	
}

void Timer_Button_Start(void)
{
	TCNT3	= 0;										// Reset Count Value		
	TCCR3B |= (1 << WGM32) | (1 << CS32) | (1 << CS30);	// Enable Timer with Prescaler of 1024 in CTC Mode
}

void Timer_Button_Stop(void)
{
	TCCR3B	= 0;										// Stop Timer
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

void Timer_Set_Timer1_Top(uint16_t top)
{
	if(top > 0)
	{
		TCCR1B	= 0;							// Disable Timer 1
		TCNT1	= 0;							// Reset Count Value
	
		OCR1A	= 1000;							// Set Compare values
		OCR1B	= OCR1A;
		
		TCCR1B |= (1 << WGM12) | (1 << CS11);	// Enable Timer with Prescaler of 8 in CTC Mode
	}
}