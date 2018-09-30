/*
 * Configuration.h
 *
 * Created: 24/06/2018 13:24:41
 *  Author: Chris
 */ 


#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include <stdint.h>

#define FALSE				0
#define TRUE				1

#define MIDI_CHANNEL_1		00
#define MIDI_CHANNEL_2		01
#define MIDI_CHANNEL_3		02
#define MIDI_CHANNEL_4		03
#define MIDI_CHANNEL_5		04
#define MIDI_CHANNEL_6		05
#define MIDI_CHANNEL_7		06
#define MIDI_CHANNEL_8		07
#define MIDI_CHANNEL_9		08
#define MIDI_CHANNEL_10		09
#define MIDI_CHANNEL_11		10
#define MIDI_CHANNEL_12		11
#define MIDI_CHANNEL_13		12
#define MIDI_CHANNEL_14		13
#define MIDI_CHANNEL_15		14
#define MIDI_CHANNEL_16		15

#define COLOR_RED			0
#define COLOR_GREEN			1
#define COLOR_BLUE			2

enum RGB_ORDER_e
{
	RGB = 0,
	RBG = 1,
	GRB = 2,
	GBR = 3,
	BRG = 4,
	BGR = 5
};


void	Configuration_Init(void);

char*	Configuration_Get_Name(void);
uint8_t Configuration_Get_Name_Length(void);
void	Configuration_Set_Name(char* name, uint8_t length);

uint8_t	Configuration_Get_MIDI_Channel(void);
uint8_t	Configuration_Get_MIDI_Note(uint8_t color);
uint8_t	Configuration_Get_MIDI_Factor(void);
uint8_t	Configuration_Get_MIDI_Skip_Note_Off(void);
void	Configuration_Set_MIDI(uint8_t channel, uint8_t note_red, uint8_t note_green, uint8_t note_blue, uint8_t factor, uint8_t skip_note_off);

uint8_t Configuration_Get_No_Data_Light_Color(uint8_t color);
uint8_t Configuration_Get_No_Data_Light_Timeout(void);
void	Configuration_Set_No_Data_Light(uint8_t red, uint8_t green, uint8_t blue, uint8_t timeout);

uint8_t Configuration_Get_Permanent_Light_Color(uint8_t color);
void	Configuration_Set_Permanent_Light(uint8_t red, uint8_t green, uint8_t blue);

uint8_t Configuration_Get_RGB_Order(void);
void	Configuration_Set_RGB_Order(uint8_t rgb_order);

uint8_t Configuration_Write_EEPROM(void);

#endif /* CONFIGURATION_H_ */