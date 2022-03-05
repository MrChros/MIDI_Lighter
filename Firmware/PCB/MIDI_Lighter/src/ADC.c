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
#define	OFF				99
#define	TEST			2
#define	CURRENT			1
#define	TEMPERATURE		0

#define MMA_SHIFT		7

// ============================================================================================
volatile uint32_t	_ADC_Value;
volatile uint32_t	_Moving_Average_Value;
volatile uint32_t	_Moving_Average_Sum;

volatile uint16_t	_Initial_Value_Counter;

/*******************************************************************
	Interrupt Service Routines
*******************************************************************/
ISR(ADC_vect)
{
	_ADC_Value  = ADCL;
	_ADC_Value |= (((uint32_t)ADCH) << 8);

	if(_Initial_Value_Counter < (uint16_t)(1 << MMA_SHIFT))
	{
		_Moving_Average_Sum	   += _ADC_Value;
	}
	else
	{
		_Moving_Average_Sum	   -= _Moving_Average_Value;
		_Moving_Average_Sum	   += _ADC_Value;
		_Moving_Average_Value	= _Moving_Average_Sum >> MMA_SHIFT;
	}
}

/*******************************************************************
	Initialization
*******************************************************************/
void ADC_Init(void)
{
	_ADC_Value				= 0;
	_Moving_Average_Sum		= 0;
	_Moving_Average_Value	= 0;

	_Initial_Value_Counter	= 0;


	uint8_t Value_To_Measure = OFF;
	
	if(Value_To_Measure == OFF)
	{
		return;
	}

	if(Value_To_Measure == CURRENT)
	{
		ADMUX =  (0b00		<< REFS0) |	// Voltage Reference Selection = AREF, Internal Vref turned off
				 (0b0		<< ADLAR) | // Result bits will be right adjusted
				 (0b000110	<< MUX0);	// Select ADC6 as input for the ADC MUX (won't change or should not...), internal Temperature Sensor would be another option


		ADCSRB = (0b0101	<< ADTS0) |	// Select Timer1 Compare Match B as Trigger for ADC Conversion => ADC Conversion every 1ms, 1000 Measurements/s)
				 (0b0		<< MUX5);	// Needs to be Zero for ADC6
	}
	else if(Value_To_Measure == TEST)
	{
//		ADMUX =  (0b00		<< REFS0) |	// Voltage Reference Selection = AREF, Internal Vref turned off
//				 (0b0		<< ADLAR) | // Result bits will be right adjusted
//				 (0b000011	<< MUX0);	// Select ADC11 as input for the ADC MUX
//
//
//		ADCSRB = (0b0011	<< ADTS0) |	// Select Timer0 as Trigger for ADC Conversion => ADC Conversion every 5ms, 200 Measurements/s)
//				 (0b1		<< MUX5);	// Needs to be One for ADC11
	}
	else if(Value_To_Measure == TEMPERATURE)
	{
		ADMUX =  (0b11		<< REFS0) |	// 
				 (0b0		<< ADLAR) | // Result bits will be right adjusted
				 (0b000111	<< MUX0);	// Temperature Sensor (MUX5 in other register)

		ADCSRB = (0b0011	<< ADTS0) |	// Select Timer0 as Trigger for ADC Conversion => ADC Conversion every 5ms, 200 Measurements/s)
				 (0b1		<< MUX5);	// Edit this line for not Temperature Sensor anymore

	}

	ADCSRA = (0b1		<< ADEN)  |		// Enable ADC
			 (0b1		<< ADATE) |		// Auto Trigger enable (Trigger is set in ADCSRB above)
			 (0b1		<< ADIE)  |		// Enable "Conversion Complete" Interrupt
			 (0b001		<< ADPS0);		// ADC Prescaler Selection: Current Selection results in "2"
										// (not really sure what this is about, for more information check table 24-5 on page 311)
}

/*******************************************************************
	Interface functions
*******************************************************************/
uint32_t ADC_Get_Value(void)
{
	return _Moving_Average_Sum;
}