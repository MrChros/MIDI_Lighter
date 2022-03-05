/*
 * Watchdog.c
 *
 * Created: 16/09/2018 20:34:50
 *  Author: Chris
 */

// ============================================================================================
#include <avr/io.h>
#include <avr/wdt.h>

#include "Watchdog.h"

// ============================================================================================
#define LED0	PF0
#define LED1	PF1
#define LED2	PF4

#define LED_ON(_LED_)		PORTF &= ~(1 << _LED_)


/*******************************************************************
	Interface functions
*******************************************************************/
void Watchdog_Disable(void)
{
	wdt_disable();
}

void Watchdog_Perform_Reset(void)
{
	wdt_reset();
	wdt_enable(WDTO_30MS);
	
	while(1) { }
}
