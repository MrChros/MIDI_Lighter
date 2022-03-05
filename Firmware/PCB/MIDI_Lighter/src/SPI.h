/*
 * SPI.h
 *
 * Created: 07/10/2018 13:51:59
 *  Author: Chris
 */ 


#ifndef SPI_H_
#define SPI_H_


#define SPI_CONST_STRING( _str_ )	SPI_Send_String( _str_ , sizeof( _str_ )-1)


void SPI_Init(void);
void SPI_Send_String(char* data, int length);



#endif /* SPI_H_ */