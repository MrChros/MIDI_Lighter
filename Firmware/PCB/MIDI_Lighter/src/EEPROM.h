/*
 * EEPROM.h
 *
 * Created: 13/07/2018 14:54:53
 *  Author: Chris
 */ 


#ifndef EEPROM_H_
#define EEPROM_H_

#include <stdint.h>

void	EEPROM_Write_Byte(uint32_t address, uint8_t data);
void	EEPROM_Write_Block(uint32_t address, uint8_t* write_data, uint32_t length);

uint8_t EEPROM_Read_Byte(uint32_t address);
void	EEPROM_Read_Block(uint32_t address, uint8_t* read_data, uint32_t length);



#endif /* EEPROM_H_ */