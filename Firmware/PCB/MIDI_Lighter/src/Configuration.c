/*
 * Configuration.c
 *
 * Created: 24/06/2018 13:24:32
 *  Author: Chris
 */ 

#include "EEPROM.h"
#include "Configuration.h"

// ============================================================================================
// Defines
#define CONFIGURATION_NAME_LENGTH		31


#define EEPROM_PAGE_SIZE				32
#define EEPROM_ADDRESS_CHECKSUM			0 * EEPROM_PAGE_SIZE
#define EEPROM_ADDRESS_NAME				1 * EEPROM_PAGE_SIZE
#define EEPROM_ADDRESS_MIDI				2 * EEPROM_PAGE_SIZE
#define EEPROM_ADDRESS_NO_DATA_LIGHT	3 * EEPROM_PAGE_SIZE
#define EEPROM_ADDRESS_PERMANENT_LIGHT	4 * EEPROM_PAGE_SIZE
#define EEPROM_ADDRESS_RGB_ORDER		5 * EEPROM_PAGE_SIZE

#define EEPROM_DATA_CHECKSUM			0xAA

// ============================================================================================
// Variables
volatile struct Configuration_Name_s
{
	char	Name[CONFIGURATION_NAME_LENGTH];
	uint8_t Name_Length;
} _Configuration_Name;

volatile struct Configuration_MIDI_s
{
	uint8_t Channel;
	uint8_t Note[3];
	uint8_t	Factor;
	uint8_t Skip_Note_Off;
} _Configuration_MIDI;

volatile struct Configuration_No_Data_Light_s
{
	uint8_t Color[3];
	uint8_t Timeout;
	uint8_t Deactivate;
} _Configuration_No_Data_Light;

volatile struct Configuration_Permanent_Light_s
{
	uint8_t Color[3];
} _Configuration_Permanent_Light;

volatile struct Configuration_RGB_Order_s
{
	uint8_t RGB_Order;
} _Configuration_RGB_Order;

// ============================================================================================
// Internal Functions
void Configuration_Factory_Reset(void);
void Configuration_Read_From_EEPROM(void);

void Configuration_Factory_Reset(void)
{
	_Configuration_Name.Name[0]							= 'N';
	_Configuration_Name.Name[1]							= 'a';
	_Configuration_Name.Name[2]							= 'm';
	_Configuration_Name.Name[3]							= 'e';
	_Configuration_Name.Name_Length						=  4;

	_Configuration_MIDI.Channel							= MIDI_CHANNEL_1;
	_Configuration_MIDI.Note[COLOR_RED]					= 0x24;
	_Configuration_MIDI.Note[COLOR_GREEN]				= 0x28;
	_Configuration_MIDI.Note[COLOR_BLUE]				= 0x2C;
	_Configuration_MIDI.Factor							= 100;
	_Configuration_MIDI.Skip_Note_Off					= TRUE;

	_Configuration_No_Data_Light.Color[COLOR_RED]		= 255;
	_Configuration_No_Data_Light.Color[COLOR_GREEN]		= 255;
	_Configuration_No_Data_Light.Color[COLOR_BLUE]		= 0;
	_Configuration_No_Data_Light.Timeout				= 5;

	_Configuration_Permanent_Light.Color[COLOR_RED]		= 255;
	_Configuration_Permanent_Light.Color[COLOR_GREEN]	= 255;
	_Configuration_Permanent_Light.Color[COLOR_BLUE]	= 0;

	_Configuration_RGB_Order.RGB_Order					= RGB;
}

void Configuration_Read_From_EEPROM(void)
{
	EEPROM_Read_Block(EEPROM_ADDRESS_NAME,				(uint8_t*)&_Configuration_Name,				sizeof(_Configuration_Name));
	EEPROM_Read_Block(EEPROM_ADDRESS_MIDI,				(uint8_t*)&_Configuration_MIDI,				sizeof(_Configuration_MIDI));
	EEPROM_Read_Block(EEPROM_ADDRESS_NO_DATA_LIGHT,		(uint8_t*)&_Configuration_No_Data_Light,	sizeof(_Configuration_No_Data_Light));
	EEPROM_Read_Block(EEPROM_ADDRESS_PERMANENT_LIGHT,	(uint8_t*)&_Configuration_Permanent_Light,	sizeof(_Configuration_Permanent_Light));
	EEPROM_Read_Block(EEPROM_ADDRESS_RGB_ORDER,			(uint8_t*)&_Configuration_RGB_Order,		sizeof(_Configuration_RGB_Order));
}

// ============================================================================================
// Initialization
void Configuration_Init(void)
{
	uint8_t Checksum = EEPROM_Read_Byte(EEPROM_ADDRESS_CHECKSUM);
	
	if(Checksum == EEPROM_DATA_CHECKSUM)
	{
		Configuration_Read_From_EEPROM();
	}
	else
	{
		Configuration_Factory_Reset();
	}
}

// ============================================================================================
// Public Functions
char* Configuration_Get_Name(void)
{
	return (char*)_Configuration_Name.Name;
}

uint8_t Configuration_Get_Name_Length(void)
{
	return _Configuration_Name.Name_Length;
}

void Configuration_Set_Name(char* name, uint8_t length)
{
	uint8_t i;
	if(length>CONFIGURATION_NAME_LENGTH)
	{
		length = CONFIGURATION_NAME_LENGTH;
	}
	
	for(i=0;i<length;i++)
	{
		_Configuration_Name.Name[i]	= name[i];
	}
	
	_Configuration_Name.Name_Length = length;
}

uint8_t	Configuration_Get_MIDI_Channel(void)
{
	return _Configuration_MIDI.Channel;
}

uint8_t	Configuration_Get_MIDI_Note(uint8_t color)
{
	if(color <= COLOR_BLUE)
	{
		return _Configuration_MIDI.Note[color];
	}
	
	return 255;
}

uint8_t	Configuration_Get_MIDI_Factor(void)
{
	return _Configuration_MIDI.Factor;
}

uint8_t	Configuration_Get_MIDI_Skip_Note_Off(void)
{
	return _Configuration_MIDI.Skip_Note_Off;
}

void Configuration_Set_MIDI(uint8_t channel, uint8_t note_red, uint8_t note_green, uint8_t note_blue, uint8_t factor, uint8_t skip_note_off)
{
	if(channel<=MIDI_CHANNEL_16)	{ _Configuration_MIDI.Channel			= channel;		}
	if(note_red		<= 127)			{ _Configuration_MIDI.Note[COLOR_RED]	= note_red;		}
	if(note_green	<= 127)			{ _Configuration_MIDI.Note[COLOR_GREEN]	= note_green;	}
	if(note_blue	<= 127)			{ _Configuration_MIDI.Note[COLOR_BLUE]	= note_blue;	}
	
	if(factor == 0	) { factor = 1; } else
	if(factor > 200	) { factor = 200; }
	_Configuration_MIDI.Factor = factor;
	
	if(skip_note_off != 0) {
		_Configuration_MIDI.Skip_Note_Off = TRUE;
	} else {
		_Configuration_MIDI.Skip_Note_Off = FALSE;
	}
}

uint8_t Configuration_Get_No_Data_Light_Color(uint8_t color)
{
	if(color <= COLOR_BLUE)
	{
		return _Configuration_No_Data_Light.Color[color];
	}

	return 0;
}

uint8_t Configuration_Get_No_Data_Light_Timeout(void)
{
	return _Configuration_No_Data_Light.Timeout;
}

uint8_t	Configuration_Get_No_Data_Light_Deactivate(void)
{
	return _Configuration_No_Data_Light.Deactivate;
}

void Configuration_Set_No_Data_Light(uint8_t red, uint8_t green, uint8_t blue, uint8_t timeout, uint8_t deactivate)
{
	_Configuration_No_Data_Light.Color[COLOR_RED]	= red;
	_Configuration_No_Data_Light.Color[COLOR_GREEN]	= green;
	_Configuration_No_Data_Light.Color[COLOR_BLUE]	= blue;

	if(timeout == 0) { timeout = 1; }
	_Configuration_No_Data_Light.Timeout = timeout;

	if(deactivate > CHANNEL_AND_EVENT_MATCH) { deactivate = CHANNEL_AND_EVENT_MATCH; }
	_Configuration_No_Data_Light.Deactivate = deactivate;
}

uint8_t Configuration_Get_Permanent_Light_Color(uint8_t color)
{
	if(color <= COLOR_BLUE)
	{
		return _Configuration_Permanent_Light.Color[color];
	}

	return 0;
}

void Configuration_Set_Permanent_Light(uint8_t red, uint8_t green, uint8_t blue)
{
	_Configuration_Permanent_Light.Color[COLOR_RED]		= red;
	_Configuration_Permanent_Light.Color[COLOR_GREEN]	= green;
	_Configuration_Permanent_Light.Color[COLOR_BLUE]	= blue;
}

uint8_t Configuration_Get_RGB_Order(void)
{
	return _Configuration_RGB_Order.RGB_Order;
}

void Configuration_Set_RGB_Order(uint8_t rgb_order)
{
	if(rgb_order>BGR)
	{
		rgb_order = _Configuration_RGB_Order.RGB_Order;
	}

	_Configuration_RGB_Order.RGB_Order = rgb_order;
}

uint8_t Configuration_Write_EEPROM(void)
{
	EEPROM_Write_Block(EEPROM_ADDRESS_NAME,				(uint8_t*)&_Configuration_Name,				sizeof(_Configuration_Name));
	EEPROM_Write_Block(EEPROM_ADDRESS_MIDI,				(uint8_t*)&_Configuration_MIDI,				sizeof(_Configuration_MIDI));
	EEPROM_Write_Block(EEPROM_ADDRESS_NO_DATA_LIGHT,	(uint8_t*)&_Configuration_No_Data_Light,	sizeof(_Configuration_No_Data_Light));
	EEPROM_Write_Block(EEPROM_ADDRESS_PERMANENT_LIGHT,	(uint8_t*)&_Configuration_Permanent_Light,	sizeof(_Configuration_Permanent_Light));
	EEPROM_Write_Block(EEPROM_ADDRESS_RGB_ORDER,		(uint8_t*)&_Configuration_RGB_Order,		sizeof(_Configuration_RGB_Order));

	EEPROM_Write_Byte(EEPROM_ADDRESS_CHECKSUM, EEPROM_DATA_CHECKSUM);

	return TRUE;
}