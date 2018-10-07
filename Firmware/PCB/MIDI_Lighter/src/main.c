#define F_CPU	16000000UL

// ============================================================================================
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <util/atomic.h>

#include "ADC.h"
#include "USB.h"
#include "Pin.h"
#include "SPI.h"
#include "Help.h"
#include "MIDI.h"
#include "UART.h"
#include "Commands.h"
#include "Watchdog.h"
#include "LED_Strip.h"
#include "Configuration.h"
#include "Timer_and_Button.h"


// ============================================================================================
/////////////////////
// General Defines //
/////////////////////
#define FALSE				0
#define TRUE				1


///////////////////
// Clock Defines //
///////////////////
#define CPU_PRESCALE(n)		(CLKPR = 0x80, CLKPR = (n))
#define CPU_16MHz			0x00
#define CPU_8MHz			0x01
#define CPU_4MHz			0x02
#define CPU_2MHz			0x03
#define CPU_1MHz			0x04
#define CPU_500kHz			0x05
#define CPU_250kHz			0x06
#define CPU_125kHz			0x07
#define CPU_62kHz			0x08




/*******************************************************************
	Main Function
*******************************************************************/
int main (void)
{
	//////////////////////
	// Disable Watchdog //
	//////////////////////
	Watchdog_Disable();

	
	//////////////////
	// Disable JTAG //
	//////////////////	
	MCUCR |= (1 << JTD);
	MCUCR |= (1 << JTD);
	
	
	///////////////
	// Set Clock //
	///////////////
	CPU_PRESCALE(CPU_16MHz);

	
	////////////////////////
	// Configure LED PINs //
	////////////////////////
	DDRF |= (1 << LEDS) | (1 << LED2) | (1 << LED1) | (1 << LED0);
	LED_OFF(LED0);
	LED_OFF(LED1);
	LED_OFF(LED2);
	LED_ON(LEDS);


	///////////////////////////
	// Module Initialization //
	///////////////////////////
	ADC_Init();
	Configuration_Init();
	Timer_And_Button_Init();
	UART_Init();
	LED_Strip_Init();
	MIDI_Init();
	Commands_Init();

//	SPI_Init();	// Needed only for Debug

	USB_Init();
	_delay_ms(250);


	////////////////////////////////
	// Globally Enable Interrupts //
	////////////////////////////////
	sei();


	
	///////////////////
	// Infinite Loop //
	///////////////////
	while(1)
	{
		Commands_Process();
		MIDI_Process();


		//////////////////////////
		// Button Short Pressed //
		//////////////////////////
		if(Button_Short_Pressed() == TRUE)
		{
			uint8_t i;
			for(i=0;i<6;i++)
			{
				LED_TOGGLE(LED0);	LED_TOGGLE(LED1);	LED_TOGGLE(LED2);
				_delay_ms(150);
			}
		}
		

		/////////////////////////
		// Button Long Pressed //
		/////////////////////////
		if(Button_Long_Pressed() == TRUE)
		{
			MIDI_Toggle_Mode();
		}


		/////////////////////////
		// LED Status Behavior //
		/////////////////////////
		if(Timer_General_On_LED_Blick() == TRUE)
		{
			if(Button_Short_State() == FALSE) { LED_ON(LEDS); } else { LED_TOGGLE(LEDS); }
		}

		_delay_us(1);
	}
}
