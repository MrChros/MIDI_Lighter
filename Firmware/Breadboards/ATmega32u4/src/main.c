#define F_CPU	16000000UL

//////////////
// INCLUDES //
//////////////
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <util/atomic.h>

#include "USB.h"
#include "UART.h"
#include "Timer_and_Button.h"


/////////////
// DEFINES //
/////////////
#define FALSE	0
#define TRUE	1


//////////////////////////////
// Clock Defines
#define CPU_PRESCALE(n) (CLKPR = 0x80, CLKPR = (n))
#define CPU_16MHz       0x00
#define CPU_8MHz        0x01
#define CPU_4MHz        0x02
#define CPU_2MHz        0x03
#define CPU_1MHz        0x04
#define CPU_500kHz      0x05
#define CPU_250kHz      0x06
#define CPU_125kHz      0x07
#define CPU_62kHz       0x08

//////////////////////////////
// LED Defines
#define LED0_PORT	PORTF
#define LED1_PORT	PORTF
#define LED2_PORT	PORTB
#define LEDON_PORT	PORTD

#define LED0_PIN	PF6
#define LED1_PIN	PF7
#define LED2_PIN	PB6
#define LEDON_PIN	PD6

#define LED_ON( _PORT_, _PIN_)		_PORT_ &= ~(1 << _PIN_)
#define LED_OFF( _PORT_, _PIN_)		_PORT_ |= (1 << _PIN_)
#define LED_TOGGLE( _PORT_, _PIN_)	_PORT_ ^= _BV(_PIN_)

//////////////////////////////
// UART Defines
#define UART_CONST_STRING( _str_ )	UART_Send_String( _str_ , sizeof( _str_ )-1)

//////////////////////////////
// LED Defines
#define NUMBER_OF_LEDS			60
#define RED						0
#define GREEN					1
#define BLUE					2
#define UNDEFINED				255

#define CONSTANT_RED			255
#define CONSTANT_GREEN			255
#define CONSTANT_BLUE			0

#define TIMEOUT_THRESHOLD		50000
#define NOTE_OFF_TIMEOUT		100


//////////////////////////////
// MIDI Defines
#define MIDI_COMPARE_NOTE_OFF	0x8		// Note off
#define	MIDI_COMPARE_NOTE_ON	0x9		// Note on
#define	MIDI_COMPARE_CHANNEL	0x0
#define MIDI_COMAPRE_NOTE_RED	0x24
#define MIDI_COMAPRE_NOTE_GREEN	0x28
#define MIDI_COMAPRE_NOTE_BLUE	0x2C



///////////////
// FUNCTIONS //
///////////////
void	WS2801_Update(void);

///////////////
// VARIABLES //
///////////////
volatile uint8_t	_Color[3];
volatile uint8_t	_Note_Off_Pending[3];
volatile uint32_t	_Note_Off_Counter[3];
volatile uint8_t	_Update_WS2801;
volatile uint8_t	_No_Data_Light_On_Old;
volatile uint8_t	_No_Data_Light_On_Fade_Div;

enum PARSING_STATE_e
{
	WAITING,
	STATUS_AND_CHANNEL_RECEIVED,
	NOTE_RECEIVED
};

enum MODE_e
{
	MIDI,
	CONSTANT	
};

struct PARSING_s
{
	enum PARSING_STATE_e State;
	uint8_t Event;
	uint8_t Color;
};



int main (void)
{
	CPU_PRESCALE(CPU_16MHz);
	
	_Color[RED] 				= 0;
	_Color[GREEN] 				= 0;
	_Color[BLUE] 				= 0;
	
	_Note_Off_Pending[RED]		= FALSE;
	_Note_Off_Pending[GREEN]	= FALSE;
	_Note_Off_Pending[BLUE]		= FALSE;
	
	_Note_Off_Counter[RED]		= 0;
	_Note_Off_Counter[GREEN]	= 0;
	_Note_Off_Counter[BLUE]		= 0;
		
	_Update_WS2801				= FALSE;
	
	_No_Data_Light_On_Old		= FALSE;
	_No_Data_Light_On_Fade_Div	= 0;

	// Configure LEDs
	DDRF |= (1 << PF6) | (1 << PF7);	// Set LED Pins to Output
	DDRB |= (1 << PB6);					// Set LED Pin to Output
	DDRD |= (1 << PD6);

	PORTD |= (1 << PD6);
	
	UART_Init();

	// Initialize SPI @ 4 MHz
	DDRB |= (1 << PB2) | (1 << PB1) | (1 << PB0);	// Set SCK, MOSI and CS to Output
	SPCR  = (1 << SPE) | (1 << MSTR);				// Enable SPI as Master
	
	WS2801_Update();
	
	// Parsing Structure
	struct PARSING_s Parsing;
	Parsing.State = WAITING;
	Parsing.Event = MIDI_COMPARE_NOTE_ON;
	Parsing.Color = UNDEFINED;	
	
	// Timeout Counter
	uint32_t Timeout = 0;
	
	enum MODE_e Mode = MIDI;	
	
	Timer_And_Button_Init();
	
//	USB_Init();
//	_delay_ms(250);
	sei();
	
	LED_OFF(LED0_PORT, LED0_PIN);
	LED_OFF(LED1_PORT, LED1_PIN);
	LED_OFF(LED2_PORT, LED2_PIN);
	
	while(1)
	{
		while(UART_Data_Available()>0)
		{
			uint8_t Data = UART_Buffer_Read();
			
			Timer_No_Data_Light_Reset();
			if(Timer_No_Data_Light_On() == FALSE && _No_Data_Light_On_Old == TRUE)
			{
				_No_Data_Light_On_Old	= Timer_No_Data_Light_On();
				_Color[RED]				= 0;
				_Color[GREEN]			= 0;
				_Color[BLUE]			= 0;
				_Update_WS2801			= TRUE;
			}
			
			if(Mode != MIDI) { continue; }
						
			switch(Parsing.State)
			{
				case WAITING:
					if(((((Data & 0xF0) >> 4) == MIDI_COMPARE_NOTE_ON) | ((((Data & 0xF0) >> 4) == MIDI_COMPARE_NOTE_OFF))) && ((Data & 0x0F) == MIDI_COMPARE_CHANNEL))
					{
						Parsing.State 	= STATUS_AND_CHANNEL_RECEIVED;
						Parsing.Event	= ((Data & 0xF0) >> 4);
						Timeout			= 0;
						LED_ON(LED0_PORT, LED0_PIN);
					}
					break;
				
				case STATUS_AND_CHANNEL_RECEIVED:
					if((Data & 0x80) == 0)
					{
							 if(Data == MIDI_COMAPRE_NOTE_RED	) { Parsing.Color = RED; 	}
						else if(Data == MIDI_COMAPRE_NOTE_GREEN	) { Parsing.Color = GREEN; 	}
						else if(Data == MIDI_COMAPRE_NOTE_BLUE	) { Parsing.Color = BLUE; 	}
						else
						{
							Parsing.State 	= WAITING;
							Parsing.Color	= UNDEFINED;
							Timeout			= 0;
							LED_OFF(LED0_PORT, LED0_PIN);
							LED_OFF(LED1_PORT, LED1_PIN);
							LED_OFF(LED2_PORT, LED2_PIN);
						}
						
						if(Parsing.Color != UNDEFINED)
						{
							Parsing.State 	= NOTE_RECEIVED;
							Timeout			= 0;
							LED_ON(LED1_PORT, LED1_PIN);
						}
					}
					else
					{
						Parsing.State 	= WAITING;
						Parsing.Color	= UNDEFINED;
						Timeout			= 0;
						LED_OFF(LED0_PORT, LED0_PIN);
						LED_OFF(LED1_PORT, LED1_PIN);
						LED_OFF(LED2_PORT, LED2_PIN);
					}
					break;
				
				case NOTE_RECEIVED:
					if((Data & 0x80) == 0)
					{
						if(Parsing.Event == MIDI_COMPARE_NOTE_ON)
						{
							_Color[Parsing.Color] 				= Data << 1;
							_Note_Off_Pending[Parsing.Color]	= FALSE;
						}
						else if(Parsing.Event == MIDI_COMPARE_NOTE_OFF)
						{
							_Note_Off_Pending[Parsing.Color]	= TRUE;
							_Note_Off_Counter[Parsing.Color]	= NOTE_OFF_TIMEOUT;
						}
						
						_Update_WS2801 = TRUE;
						LED_ON(LED2_PORT, LED2_PIN);
					}
					else
					{
						LED_OFF(LED0_PORT, LED0_PIN);
						LED_OFF(LED1_PORT, LED1_PIN);
						LED_OFF(LED2_PORT, LED2_PIN);
					}
					Parsing.State	= WAITING;
					Parsing.Color	= UNDEFINED;
					Timeout			= 0;
					break;
				
				default:
					Parsing.State = WAITING;
					break;
			}
		}
	
		if((Parsing.State == STATUS_AND_CHANNEL_RECEIVED) || (Parsing.State == NOTE_RECEIVED))
		{
			Timeout++;
			
			if(Timeout == TIMEOUT_THRESHOLD)
			{
				Parsing.State 	= WAITING;
				Parsing.Color 	= UNDEFINED;
				
				Timeout = 0;
			}
		}
		
		uint8_t i;
		for(i=RED;i<=BLUE;i++)
		{
			if(_Note_Off_Counter[i]>0)
			{
				_Note_Off_Counter[i]--;
				if(_Note_Off_Counter[i]==0 && _Note_Off_Pending[i]==TRUE)
				{
					_Color[i]				= 0;
					_Note_Off_Pending[i]	= FALSE;
					_Update_WS2801			= TRUE;
				}
			}
		}
		
		if(_Update_WS2801 == TRUE && Timer_WS2801_OV_Occurred() == TRUE)
		{
			_Update_WS2801		= FALSE;
			if(Button_Short_State() == TRUE && _No_Data_Light_On_Old == TRUE)
			{
				if(_No_Data_Light_On_Fade_Div == 3)
				{
					if(_Color[RED]		< CONSTANT_RED	) { _Color[RED]		+= 1; }
					if(_Color[GREEN]	< CONSTANT_GREEN) { _Color[GREEN]	+= 1; }
					if(_Color[BLUE]		< CONSTANT_BLUE	) { _Color[BLUE]	+= 1; }
					
					_No_Data_Light_On_Fade_Div = 0;
				}
				else
				{
					_No_Data_Light_On_Fade_Div++;
				}
			}
			
			WS2801_Update();
		}
		
		if(Button_Short_Pressed() == TRUE)
		{
			uint8_t i;
			for(i=0;i<6;i++)
			{
				LED_TOGGLE(LED0_PORT, LED0_PIN);
				LED_TOGGLE(LED1_PORT, LED1_PIN);
				LED_TOGGLE(LED2_PORT, LED2_PIN);
				_delay_ms(150);
			}
		}
		
		if(Button_Long_Pressed() == TRUE)
		{
			if(Mode == MIDI)
			{
				Mode = CONSTANT;
				
				_Color[RED]		= CONSTANT_RED;
				_Color[GREEN]	= CONSTANT_GREEN;
				_Color[BLUE]	= CONSTANT_BLUE;
				
				_Update_WS2801 = TRUE;
			}
			else if(Mode == CONSTANT)
			{
				Mode = MIDI;
				
				_Color[RED]		= 0;
				_Color[GREEN]	= 0;
				_Color[BLUE]	= 0;
				
				_Update_WS2801 = TRUE;
			}
		}
		
		if(Timer_WS2801_On_LED_Blick() == TRUE)
		{
			if(Button_Short_State() == FALSE)
			{
				LED_OFF(PORTD, PD6);	// In fact, it's turning the LED on
			}
			else
			{
				LED_TOGGLE(PORTD, PD6);
			}
		}
		
		if(Button_Short_State() == TRUE)
		{
			if(Timer_No_Data_Light_On() == TRUE && _No_Data_Light_On_Old == FALSE)
			{
				_Color[RED]				= 0;
				_Color[GREEN]			= 0;
				_Color[BLUE]			= 0;
		
				_No_Data_Light_On_Old	= Timer_No_Data_Light_On();
			}
			
			_Update_WS2801			= TRUE;
		}
		else
		{
			Timer_No_Data_Light_Reset();
		}
		
		_delay_us(10);
	}
}


void WS2801_Update(void)
{
	ATOMIC_BLOCK(ATOMIC_FORCEON)
	{
		uint16_t i;
		
		Timer_WS2801_Restart();
		
		for(i=0;i<NUMBER_OF_LEDS;i++)
		{
			SPDR = _Color[RED];				// Start SPI Transmission
			while(!(SPSR & (1 << SPIF)));	// Wait Transmission complete
		
			SPDR = _Color[BLUE];			// Start SPI Transmission
			while(!(SPSR & (1 << SPIF)));	// Wait Transmission complete
		
			SPDR = _Color[GREEN];			// Start SPI Transmission
			while(!(SPSR & (1 << SPIF)));	// Wait Transmission complete
		}
	
		LED_OFF(LED0_PORT, LED0_PIN);
		LED_OFF(LED1_PORT, LED1_PIN);
		LED_OFF(LED2_PORT, LED2_PIN);
	}
}
