/*
 * LED_Strip.h
 *
 * Created: 16/09/2018 09:51:07
 *  Author: Chris
 */ 


#ifndef LED_STRIP_H_
#define LED_STRIP_H_

enum TIMER4_PRESCALER_e
{
	PRESCALER_1		= 0x1,
	PRESCALER_2		= 0x2,
	PRESCALER_4		= 0x3,
	PRESCALER_8		= 0x4,
	PRESCALER_16	= 0x5,
	PRESCALER_32	= 0x6,
	PRESCALER_64	= 0x7,
	PRESCALER_128	= 0x8,
	PRESCALER_256	= 0x9,
	PRESCALER_512	= 0xA,
	PRESCALER_1024	= 0xB,
	PRESCALER_2048	= 0xC,
	PRESCALER_4096	= 0xD,
	PRESCALER_8192	= 0xE,
	PRESCALER_16384	= 0xF
};

void LED_Strip_Init(void);
void LED_Strip_Update(uint8_t red, uint8_t green, uint8_t blue);

void LED_Strip_Set_Clock_Prescaler(uint8_t prescaler);



#endif /* LED_STRIP_H_ */