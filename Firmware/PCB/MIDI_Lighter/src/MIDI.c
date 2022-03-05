/*
 * MIDI.c
 *
 * Created: 16/09/2018 09:14:38
 *  Author: Chris
 */ 

// ============================================================================================
#include <avr/io.h>

#include "Pin.h"
#include "SPI.h"
#include "MIDI.h"
#include "UART.h"
#include "LED_Strip.h"
#include "Configuration.h"
#include "Timer_and_Button.h"


// ============================================================================================
#define TRUE					1
#define FALSE					0

#define RED						0
#define GREEN					1
#define BLUE					2
#define UNDEFINED				255


/////////////////
// MIDI Events //
/////////////////
#define MIDI_COMPARE_NOTE_OFF	0x8		// Note off
#define	MIDI_COMPARE_NOTE_ON	0x9		// Note on

#define TIMEOUT_THRESHOLD		50000
#define NOTE_OFF_TIMEOUT		100

// ============================================================================================
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

volatile uint8_t			_Color[3];
volatile uint8_t			_Note_Off_Pending[3];
volatile uint32_t			_Note_Off_Counter[3];
volatile uint8_t			_Update_LED_Strip;

volatile uint8_t			_No_Data_Light_On_Old;
volatile uint8_t			_No_Data_Light_On_Fade_Div;

volatile uint32_t			_Timeout;

volatile struct PARSING_s	_Parsing;
volatile enum MODE_e		_Mode;



/*******************************************************************
	Initialization
*******************************************************************/
void MIDI_Init(void)
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

	_Update_LED_Strip			= FALSE;

	_No_Data_Light_On_Old		= FALSE;
	_No_Data_Light_On_Fade_Div	= 0;
	
	////////////////////////////
	// Parsing Data Structure //
	////////////////////////////
	_Parsing.State				= WAITING;
	_Parsing.Event				= MIDI_COMPARE_NOTE_ON;
	_Parsing.Color				= UNDEFINED;
	

	/////////////////////
	// Timeout Counter //
	/////////////////////
	_Timeout					= 0;
	

	/////////////////
	// Inital Mode //
	/////////////////
	_Mode						= MIDI;
}


/*******************************************************************
	Interface functions
*******************************************************************/
void MIDI_Process(void)
{
	while(UART_Data_Available()>0)
	{
		// Read Data from incoming UART Buffer
		uint8_t Data = UART_Buffer_Read();
		

		// Ignore Data if Permanent Light Mode is activated
		if(_Mode != MIDI) { continue; }


		///////////////////////////////
		// Compare Channel and Event //
		///////////////////////////////
		uint8_t Channel_Match	= FALSE;
		uint8_t Event_Match		= FALSE;

		if(_Parsing.State == WAITING)
		{
			if((Data & 0x0F) == Configuration_Get_MIDI_Channel())
			{
				Channel_Match = TRUE;
			}
			if((((Data & 0xF0) >> 4) == MIDI_COMPARE_NOTE_ON) | ((((Data & 0xF0) >> 4) == MIDI_COMPARE_NOTE_OFF)))
			{
				Event_Match = TRUE;
			}
		}


		///////////////////////////////////
		// Check for No Data Light Reset //
		///////////////////////////////////
		uint8_t No_Data_Light_Reset = FALSE;

		if(Configuration_Get_No_Data_Light_Deactivate() == ANY_TRAFFIC)
		{
			No_Data_Light_Reset = TRUE;
		}
		else if(Configuration_Get_No_Data_Light_Deactivate() == CHANNEL_MATCH)
		{
			if(Channel_Match == TRUE)
			{
				No_Data_Light_Reset = TRUE;
			}
		}
		else if(Configuration_Get_No_Data_Light_Deactivate() == EVENT_MATCH)
		{
			if(Event_Match == TRUE)
			{
				No_Data_Light_Reset = TRUE;
			}
		}
		else if(Configuration_Get_No_Data_Light_Deactivate() == CHANNEL_MATCH)
		{
			if(Event_Match == TRUE && Channel_Match == TRUE)
			{
				No_Data_Light_Reset = TRUE;
			}
		}

		if(No_Data_Light_Reset == TRUE)
		{
			Timer_No_Data_Light_Reset();
			if(Timer_No_Data_Light_On() == FALSE && _No_Data_Light_On_Old == TRUE)
			{
				_No_Data_Light_On_Old	= Timer_No_Data_Light_On();
				_Color[RED]				= 0;
				_Color[GREEN]			= 0;
				_Color[BLUE]			= 0;
				_Update_LED_Strip		= TRUE;
			}
		}
		

		/////////////////////////////////
		// Parse received MIDI command //
		/////////////////////////////////
		switch(_Parsing.State)
		{
			case WAITING:
				if(Event_Match == TRUE && Channel_Match == TRUE)
				{
					_Parsing.State 	= STATUS_AND_CHANNEL_RECEIVED;
					_Parsing.Event	= ((Data & 0xF0) >> 4);
					_Timeout		= 0;
					LED_ON(LED0);
				}
				break;
		
			case STATUS_AND_CHANNEL_RECEIVED:
				if((Data & 0x80) == 0)
				{
						 if(Data == Configuration_Get_MIDI_Note(RED)	) { _Parsing.Color = RED; 	}
					else if(Data == Configuration_Get_MIDI_Note(GREEN)	) { _Parsing.Color = GREEN; }
					else if(Data == Configuration_Get_MIDI_Note(BLUE)	) { _Parsing.Color = BLUE; 	}
					else
					{
						_Parsing.State 	= WAITING;
						_Parsing.Color	= UNDEFINED;
						_Timeout		= 0;
						LED_OFF(LED0);
						LED_OFF(LED1);
						LED_OFF(LED2);
					}
					
					if(_Parsing.Color != UNDEFINED)
					{
						_Parsing.State 	= NOTE_RECEIVED;
						_Timeout		= 0;
						LED_ON(LED1);
					}
				}
				else
				{
					_Parsing.State 	= WAITING;
					_Parsing.Color	= UNDEFINED;
					_Timeout		= 0;
					LED_OFF(LED0);
					LED_OFF(LED1);
					LED_OFF(LED2);
				}
				break;
		
			case NOTE_RECEIVED:
				if((Data & 0x80) == 0)
				{
					if(_Parsing.Event == MIDI_COMPARE_NOTE_ON)
					{
						_Color[_Parsing.Color] 				= (uint8_t)(((uint16_t)Data * (uint16_t)Configuration_Get_MIDI_Factor())/(uint16_t)100);
						_Note_Off_Pending[_Parsing.Color]	= FALSE;
						_Note_Off_Counter[_Parsing.Color]	= 0;
						
					}
					else if(_Parsing.Event == MIDI_COMPARE_NOTE_OFF)
					{
						if(Configuration_Get_MIDI_Skip_Note_Off() == TRUE)
						{
							_Note_Off_Pending[_Parsing.Color]	= TRUE;
							_Note_Off_Counter[_Parsing.Color]	= NOTE_OFF_TIMEOUT;
						}
						else
						{
							_Color[_Parsing.Color] 				= 0;
							_Note_Off_Pending[_Parsing.Color]	= FALSE;
						}
					}
			
					_Update_LED_Strip = TRUE;
					LED_ON(LED2);
				}
				else
				{
					LED_OFF(LED0);
					LED_OFF(LED1);
					LED_OFF(LED2);
				}
				_Parsing.State	= WAITING;
				_Parsing.Color	= UNDEFINED;
				_Timeout		= 0;
				break;
		
			default:
				_Parsing.State = WAITING;
				break;
		}
	}


	/* Section: Check for Timeout if MIDI Byte is missing
	 *
	 * Only applies when first data of a MIDI Command has been received
	 * Note: The code only reacts to 'Note On' and 'Note Off' Event Commands
	 */
	if((_Parsing.State == STATUS_AND_CHANNEL_RECEIVED) || (_Parsing.State == NOTE_RECEIVED))
	{
		_Timeout++;
	
		if(_Timeout == TIMEOUT_THRESHOLD)
		{
			_Parsing.State 	= WAITING;
			_Parsing.Color 	= UNDEFINED;
		
			_Timeout = 0;
		}
	}

	
	/* Section: If a 'Note Off' has been received and if it should be skipped
	 * a timer will count down. If it reaches Zero, the according color will 
	 * switched off. If a 'Note On' Event it reached before, the 'Note Off'
	 * Event will be skipped
	 *
	 * This Code will only take effect, if the appropriate Configuration is set
	 */
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
				_Update_LED_Strip		= TRUE;
			}
		}
	}


	if(Button_Short_State() == TRUE && _Mode == MIDI)
	{
		if(Timer_No_Data_Light_On() == TRUE && _No_Data_Light_On_Old == FALSE)
		{
			_Color[RED]				= 0;
			_Color[GREEN]			= 0;
			_Color[BLUE]			= 0;
			
			_No_Data_Light_On_Old	= Timer_No_Data_Light_On();
		}
		
		_Update_LED_Strip			= TRUE;
	}
	else
	{
		Timer_No_Data_Light_Reset();
	}

	if(_Mode == CONSTANT)
	{
		_Color[RED]			= Configuration_Get_Permanent_Light_Color(RED);
		_Color[GREEN]		= Configuration_Get_Permanent_Light_Color(GREEN);
		_Color[BLUE]		= Configuration_Get_Permanent_Light_Color(BLUE);
		_Update_LED_Strip	= TRUE;
	}

	if(_Update_LED_Strip == TRUE)
	{
		_Update_LED_Strip		= FALSE;
		if(Button_Short_State() == TRUE && _No_Data_Light_On_Old == TRUE)
		{
			if(Timer_General_OV_Occurred()==TRUE)
			{
				if(_No_Data_Light_On_Fade_Div == 3)
				{
					if(_Color[RED]		< Configuration_Get_No_Data_Light_Color(RED)	) { _Color[RED]		+= 1; } else if(_Color[RED]		> Configuration_Get_No_Data_Light_Color(RED)	) { _Color[RED]		-= 1; }
					if(_Color[GREEN]	< Configuration_Get_No_Data_Light_Color(GREEN)	) { _Color[GREEN]	+= 1; } else if(_Color[GREEN]	> Configuration_Get_No_Data_Light_Color(GREEN)	) { _Color[GREEN]	-= 1; }
					if(_Color[BLUE]		< Configuration_Get_No_Data_Light_Color(BLUE)	) { _Color[BLUE]	+= 1; } else if(_Color[BLUE]	> Configuration_Get_No_Data_Light_Color(BLUE)	) { _Color[BLUE]	-= 1; }
					
					_No_Data_Light_On_Fade_Div = 0;
				}
				else
				{
					_No_Data_Light_On_Fade_Div++;
				}
			}
		}
		
		LED_Strip_Update(_Color[RED], _Color[GREEN], _Color[BLUE]);

		LED_OFF(LED0);
		LED_OFF(LED1);
		LED_OFF(LED2);
	}
}

void MIDI_Toggle_Mode(void)
{
	if(_Mode == MIDI)
	{
		_Mode = CONSTANT;
	}
	else if(_Mode == CONSTANT)
	{
		_Mode = MIDI;
		
		_Color[RED]		= 0;
		_Color[GREEN]	= 0;
		_Color[BLUE]	= 0;
	}

	_Update_LED_Strip = TRUE;
}