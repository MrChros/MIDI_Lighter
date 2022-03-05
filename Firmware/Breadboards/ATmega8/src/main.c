#define F_CPU	16000000UL

//////////////
// INCLUDES //
//////////////
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <util/atomic.h>

#include "UART.h"
#include "Timer_and_Button.h"


/////////////
// DEFINES //
/////////////
#define FALSE	0
#define TRUE	1

//////////////////////////////
// LED Defines
#define LED0				PC5
#define LED1				PC4
#define LED2				PC2
#define LEDON				PC0

#define LED_ON(_LED_)		PORTC &= ~(1 << _LED_)
#define LED_OFF(_LED_)		PORTC |= (1 << _LED_)
#define LED_TOGGLE(_LED_)	PORTC ^= _BV(_LED_)

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
#define MIDI_COMAPRE_NOTE_RED	0x30
#define MIDI_COMAPRE_NOTE_GREEN	0x34
#define MIDI_COMAPRE_NOTE_BLUE	0x38



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
	
	DDRC |= (1 << LED0) | (1 << LED1) | (1 << LED2) | (1 << LEDON);
	
	LED_OFF(LED0);
	LED_OFF(LED1);
	LED_OFF(LED2);
	LED_ON(LEDON);
	
	UART_Init();
	
	// Initialize SPI @ 4 MHz
	DDRB |= (1 << PB5) | (1 << PB3) | (1 << PB2);	// Set SCK, MOSI and CS to Output
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
	
	sei();
			
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
						LED_ON(LED0);
					}
					break;
				
				case STATUS_AND_CHANNEL_RECEIVED:
					if((Data & 0x80) == 0)
					{
							 if(Data == MIDI_COMAPRE_NOTE_RED	)	{ Parsing.Color = RED; 			}
						else if(Data == MIDI_COMAPRE_NOTE_GREEN	)	{ Parsing.Color = GREEN; 		}
						else if(Data == MIDI_COMAPRE_NOTE_BLUE	)	{ Parsing.Color = BLUE; 		}
						else										{ Parsing.Color = UNDEFINED;	}
					}
					
					if(Parsing.Color != UNDEFINED)
					{
						Parsing.State 	= NOTE_RECEIVED;
						Timeout			= 0;
						LED_ON(LED1);
					}					
					else
					{
						Parsing.State 	= WAITING;
						Parsing.Color	= UNDEFINED;
						Timeout			= 0;
						LED_OFF(LED0);
						LED_OFF(LED1);
						LED_OFF(LED2);
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
						LED_ON(LED2);
					}
					else
					{
						LED_OFF(LED0);
						LED_OFF(LED1);
						LED_OFF(LED2);
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
			_Update_WS2801 = FALSE;
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
				LED_TOGGLE(LED0);
				LED_TOGGLE(LED1);
				LED_TOGGLE(LED2);
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
				LED_ON(LEDON);
			}
			else
			{
				LED_TOGGLE(LEDON);
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
		
		LED_OFF(LED0);
		LED_OFF(LED1);
		LED_OFF(LED2);
	}
}