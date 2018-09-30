/*
 * ADC.c
 *
 * Created: 30/09/2018 20:19:41
 *  Author: Chris
 */ 

// ============================================================================================
#include <avr/io.h>
#include <avr/interrupt.h>

#include "ADC.h"

// ============================================================================================


// ============================================================================================
volatile uint16_t	_ADC_Value;

/*******************************************************************
	Interrupt Service Routines
*******************************************************************/
ISR(ADC_vect)
{
	_ADC_Value = 0 | (ADCH << 8) | (ADCL << 0);
}

/*******************************************************************
	Initialization
*******************************************************************/
void ADC_Init(void)
{
	_ADC_Value = 0;
	
	ADMUX =  (0b00		<< REFS0) |	// Voltage Reference Selection = AREF, Internal Vref turned off
			 (0b0		<< ADLAR) | // Result bits will be right adjusted
			 (0b000110	<< MUX0);	// Select ADC6 as input for the ADC MUX (won't change or should not...), internal Temperature Sensor would be another option

	ADCSRB = (0b0011	<< ADTS0);	// Select Timer0 as Trigger for ADC Conversion => ADC Conversion every 5ms, 200 Measurements/s)

	ADCSRA = (0b1		<< ADEN)  |	// Enable ADC
			 (0b1		<< ADATE) | // Auto Trigger enable (Trigger is set in ADCSRB above)
			 (0b1		<< ADIE)  | // Enable "Conversion Complete" Interrupt
			 (0b000		<< ADPS0);	// ADC Prescaler Selection: Current Selection results in "2"
									// (not really sure what this is about, for more information check table 24-5 on page 311)
}

/*******************************************************************
	Interface functions
*******************************************************************/
uint16_t ADC_Get_Value(void)
{
	return _ADC_Value;
}