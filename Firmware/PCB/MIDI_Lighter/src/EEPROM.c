/*
 * EEPROM.c
 *
 * Created: 13/07/2018 14:54:42
 *  Author: Chris
 */ 
#define F_CPU	16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <util/atomic.h>

#include "EEPROM.h"


void EEPROM_Write_Byte(uint32_t address, uint8_t data)
{
	while(EECR & (1<<EEPE));	// Wait for completion of previous write
	
	EEAR = address;				// Set up address and Data Registers 
	EEDR = data;
	
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		EECR |= (1<<EEMPE);		// Write logical one to EEMPE
		EECR |= (1<<EEPE);		// Start EEPROM write by setting EEPE
	}
}

void EEPROM_Write_Block(uint32_t address, uint8_t* write_data, uint32_t length)
{
	uint32_t i;
	for(i=0;i<length;i++)
	{
		EEPROM_Write_Byte(address + i, write_data[i]);
	}
}

uint8_t EEPROM_Read_Byte(uint32_t address)
{
	while(EECR & (1<<EEPE));	// Wait for completion of previous write
	
	EEAR = address;				// Set up address register
	EECR |= (1<<EERE);			// Start EEPROM read by writing EERE
	
	return EEDR;				// Return data from Data Register
}

void EEPROM_Read_Block(uint32_t address, uint8_t* read_data, uint32_t length)
{
	uint32_t i;
	for(i=0;i<length;i++)
	{
		read_data[i] = EEPROM_Read_Byte(address + i);
	}
}