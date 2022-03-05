/*
 * Commands.c
 *
 * Created: 24/06/2018 13:15:23
 *  Author: Chris
 */ 

// ============================================================================================
#include "Commands.h"

#include "ADC.h"
#include "USB.h"
#include "Help.h"
#include "EEPROM.h"
#include "Watchdog.h"
#include "LED_Strip.h"
#include "Configuration.h"
#include "Timer_and_Button.h"

// ============================================================================================
// Defines
#define COMMAND_MAX_LENGTH					128
#define COMMAND_TERMINATOR					0x0D

#define COMMAND_NAME_GET					'a'
#define COMMAND_NAME_GET_LENGTH				 1
#define COMMAND_NAME_SET					'A'
#define COMMAND_NAME_SET_LENGTH				31		// Can be shorter

#define COMMAND_MIDI_GET					'b'
#define COMMAND_MIDI_GET_LENGTH				 1
#define COMMAND_MIDI_SET					'B'
#define COMMAND_MIDI_SET_LENGTH				11		// Command			(1 Byte)
													// Channel			(1 Byte)
													// Note Red			(2 Byte)
													// Note Green		(2 Byte)
													// Note Blue		(2 Byte)
													// Factor			(2 Byte)
													// Skip Note Off	(1 Byte)

#define COMMAND_NO_DATA_LIGHT_GET			'c'
#define COMMAND_NO_DATA_LIGHT_GET_LENGTH	 1
#define COMMAND_NO_DATA_LIGHT_SET			'C'
#define COMMAND_NO_DATA_LIGHT_SET_LENGTH	 10

#define COMMAND_PERMANENT_LIGHT_GET			'd'
#define COMMAND_PERMANENT_LIGHT_GET_LENGTH	 1
#define COMMAND_PERMANENT_LIGHT_SET			'D'
#define COMMAND_PERMANENT_LIGHT_SET_LENGTH	 7

#define COMMAND_RGB_ORDER_GET				'e'
#define COMMAND_RGB_ORDER_GET_LENGTH		 1
#define COMMAND_RGB_ORDER_SET				'E'
#define COMMAND_RGB_ORDER_SET_LENGTH		 2

#define COMMAND_ADC_GET						'f'
#define COMMAND_ADC_GET_LENGTH				 1


#define COMMAND_SET_TIMER1_TOP				'V'
#define COMMAND_SET_TIMER1_TOP_LENGTH		 5

#define COMMAND_SET_TIMER4_PRESCALER		'W'
#define COMMAND_SET_TIMER4_PRESCALER_LENGTH	 2

#define COMMAND_READ_EEPROM					'y'
#define COMMAND_READ_EEPROM_LENGTH			 4

#define COMMAND_WRITE_EEPROM				'Y'
#define COMMAND_WRITE_EEPROM_LENGTH			 1

#define COMMAND_PERFORM_RESET				'X'
#define COMMAND_PERFORM_RESET_LENGTH		 1


// ============================================================================================
// Variables
volatile struct Commands_Buffer_s
{
	char Data[COMMAND_MAX_LENGTH];
	uint8_t Count;
} _Commands_Buffer;


/*******************************************************************
	Internal functions
*******************************************************************/
void Command_Parse_Command(void);


void Command_Parse_Command(void)
{
	uint8_t				Send_Success = FALSE;
	
	if(_Commands_Buffer.Count == 0) { return; }
	
	switch(_Commands_Buffer.Data[0])
	{
		case COMMAND_NAME_GET:
			if(_Commands_Buffer.Count != COMMAND_NAME_GET_LENGTH) { break; }
			Send_Success = USB_Send_String(Configuration_Get_Name(), Configuration_Get_Name_Length());	if(Send_Success==FALSE) { break; }
			Send_Success = USB_Send_Char(COMMAND_TERMINATOR);
			break;
			
		case COMMAND_NAME_SET:
			Configuration_Set_Name((char*)(&_Commands_Buffer.Data[1]), _Commands_Buffer.Count - 1);
			break;

		case COMMAND_MIDI_GET:
			if(_Commands_Buffer.Count != COMMAND_MIDI_GET_LENGTH) { break; }
			Send_Success = USB_Send_Int_Hex(Configuration_Get_MIDI_Channel()		, 1, FALSE);	if(Send_Success==FALSE) { break; }
			Send_Success = USB_Send_Int_Hex(Configuration_Get_MIDI_Note(COLOR_RED)	, 2, FALSE);	if(Send_Success==FALSE) { break; }
			Send_Success = USB_Send_Int_Hex(Configuration_Get_MIDI_Note(COLOR_GREEN), 2, FALSE);	if(Send_Success==FALSE) { break; }
			Send_Success = USB_Send_Int_Hex(Configuration_Get_MIDI_Note(COLOR_BLUE)	, 2, FALSE);	if(Send_Success==FALSE) { break; }
			Send_Success = USB_Send_Int_Hex(Configuration_Get_MIDI_Factor()			, 2, FALSE);	if(Send_Success==FALSE) { break; }
			Send_Success = USB_Send_Int_Hex(Configuration_Get_MIDI_Skip_Note_Off()	, 1, FALSE);	if(Send_Success==FALSE) { break; }
			Send_Success = USB_Send_Char(COMMAND_TERMINATOR);
			break;

		case COMMAND_MIDI_SET:
			if(_Commands_Buffer.Count != COMMAND_MIDI_SET_LENGTH) { break; }
			Configuration_Set_MIDI( CharArray_To_Number(&_Commands_Buffer.Data[1], 1),		// MIDI Channel
									CharArray_To_Number(&_Commands_Buffer.Data[2], 2),		// MIDI Note Red
									CharArray_To_Number(&_Commands_Buffer.Data[4], 2),		// MIDI Note Green
									CharArray_To_Number(&_Commands_Buffer.Data[6], 2),		// MIDI Note Blue
									CharArray_To_Number(&_Commands_Buffer.Data[8], 2),		// MIDI Factor
									CharArray_To_Number(&_Commands_Buffer.Data[10], 1));	// MIDI Skip Note Off
			break;

		case COMMAND_NO_DATA_LIGHT_GET:
			if(_Commands_Buffer.Count != COMMAND_NO_DATA_LIGHT_GET_LENGTH) { break; }
			Send_Success = USB_Send_Int_Hex(Configuration_Get_No_Data_Light_Color(COLOR_RED)	, 2, FALSE);	if(Send_Success==FALSE) { break; }
			Send_Success = USB_Send_Int_Hex(Configuration_Get_No_Data_Light_Color(COLOR_GREEN)	, 2, FALSE);	if(Send_Success==FALSE) { break; }
			Send_Success = USB_Send_Int_Hex(Configuration_Get_No_Data_Light_Color(COLOR_BLUE)	, 2, FALSE);	if(Send_Success==FALSE) { break; }
			Send_Success = USB_Send_Int_Hex(Configuration_Get_No_Data_Light_Timeout()			, 2, FALSE);	if(Send_Success==FALSE) { break; }
			Send_Success = USB_Send_Int_Hex(Configuration_Get_No_Data_Light_Deactivate()		, 1, FALSE);	if(Send_Success==FALSE) { break; }
			Send_Success = USB_Send_Char(COMMAND_TERMINATOR);
			break;

		case COMMAND_NO_DATA_LIGHT_SET:
			if(_Commands_Buffer.Count != COMMAND_NO_DATA_LIGHT_SET_LENGTH) { break; }
			Configuration_Set_No_Data_Light(CharArray_To_Number(&_Commands_Buffer.Data[1], 2),	// Color Red
											CharArray_To_Number(&_Commands_Buffer.Data[3], 2),	// Color Green
											CharArray_To_Number(&_Commands_Buffer.Data[5], 2),	// Color Blue
											CharArray_To_Number(&_Commands_Buffer.Data[7], 2),	// Timeout
											CharArray_To_Number(&_Commands_Buffer.Data[9], 1));	// Deactivate
			break;

		case COMMAND_PERMANENT_LIGHT_GET:
			if(_Commands_Buffer.Count != COMMAND_PERMANENT_LIGHT_GET_LENGTH) { break; }
			Send_Success = USB_Send_Int_Hex(Configuration_Get_Permanent_Light_Color(COLOR_RED)	, 2, FALSE);	if(Send_Success==FALSE) { break; }
			Send_Success = USB_Send_Int_Hex(Configuration_Get_Permanent_Light_Color(COLOR_GREEN), 2, FALSE);	if(Send_Success==FALSE) { break; }
			Send_Success = USB_Send_Int_Hex(Configuration_Get_Permanent_Light_Color(COLOR_BLUE)	, 2, FALSE);	if(Send_Success==FALSE) { break; }
			Send_Success = USB_Send_Char(COMMAND_TERMINATOR);
			break;

		case COMMAND_PERMANENT_LIGHT_SET:
			if(_Commands_Buffer.Count != COMMAND_PERMANENT_LIGHT_SET_LENGTH) { break; }
			Configuration_Set_Permanent_Light(	CharArray_To_Number(&_Commands_Buffer.Data[1], 2),	// Color Red
												CharArray_To_Number(&_Commands_Buffer.Data[3], 2),	// Color Green
												CharArray_To_Number(&_Commands_Buffer.Data[5], 2));	// Color Blue
			break;

		case COMMAND_RGB_ORDER_GET:
			if(_Commands_Buffer.Count != COMMAND_RGB_ORDER_GET_LENGTH) { break; }
			Send_Success = USB_Send_Int_Hex(Configuration_Get_RGB_Order(), 1, FALSE);	if(Send_Success==FALSE) { break; }
			Send_Success = USB_Send_Char(COMMAND_TERMINATOR);
			break;

		case COMMAND_RGB_ORDER_SET:
			if(_Commands_Buffer.Count != COMMAND_RGB_ORDER_SET_LENGTH) { break; }
			Configuration_Set_RGB_Order(CharArray_To_Number(&_Commands_Buffer.Data[1], 1));
			break;

		case COMMAND_ADC_GET:
			if(_Commands_Buffer.Count != COMMAND_ADC_GET_LENGTH) { break; }
			Send_Success = USB_Send_Int_Hex(ADC_Get_Value(), 8, FALSE);	if(Send_Success==FALSE) { break; }
			Send_Success = USB_Send_Char(COMMAND_TERMINATOR);
			break;

		
		///////////////////////////////////////////
		//   Debug Commands after this comment   //
		// Not meant for use in real application //
		///////////////////////////////////////////
		case COMMAND_SET_TIMER1_TOP:
			if(_Commands_Buffer.Count != COMMAND_SET_TIMER1_TOP_LENGTH) { break; }
			Timer_Set_Timer1_Top(CharArray_To_Number(&_Commands_Buffer.Data[1], 4));
			break;

		case COMMAND_SET_TIMER4_PRESCALER:
			if(_Commands_Buffer.Count != COMMAND_SET_TIMER4_PRESCALER_LENGTH) { break; }
			LED_Strip_Set_Clock_Prescaler(CharArray_To_Number(&_Commands_Buffer.Data[1], 1));
			break;

		case COMMAND_WRITE_EEPROM:
			if(_Commands_Buffer.Count != COMMAND_WRITE_EEPROM_LENGTH) { break; }
			Configuration_Write_EEPROM();
			break;

		case COMMAND_READ_EEPROM:
			if(_Commands_Buffer.Count != COMMAND_READ_EEPROM_LENGTH) { break; }
			Send_Success = USB_Send_Int_Hex(EEPROM_Read_Byte(CharArray_To_Number(&_Commands_Buffer.Data[1], 3)), 2, FALSE);	if(Send_Success==FALSE) { break; }
			Send_Success = USB_Send_Char(COMMAND_TERMINATOR);
			break;

		case COMMAND_PERFORM_RESET:
			if(_Commands_Buffer.Count != COMMAND_PERFORM_RESET_LENGTH) { break; }
			Watchdog_Perform_Reset();
			break;
	}
	
	_Commands_Buffer.Count = 0;
}


/*******************************************************************
	Initialization
*******************************************************************/
void Commands_Init(void)
{
	_Commands_Buffer.Count = 0;
}


/*******************************************************************
	Interface functions
*******************************************************************/
void Commands_Process(void)
{
	if(USB_Data_Available()>0)
	{
		while(USB_Data_Available()>0)
		{
			char USB_Data = USB_Get_Char();

			if(USB_Data == COMMAND_TERMINATOR)
			{
				Command_Parse_Command();
			}
			else
			{
				_Commands_Buffer.Data[_Commands_Buffer.Count++] = USB_Data;
			}
		}
	}
}