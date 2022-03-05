/*
 * UART.h
 *
 * Created: 02.06.2018 12:48:33
 *  Author: Chris
 */ 


#ifndef UART_H_
#define UART_H_


#define UART_CONST_STRING( _str_ )	UART_Send_String( _str_ , sizeof( _str_ )-1)


void	UART_Init			(void);
void	UART_Send_Char		(char data);
void	UART_Send_String	(char* data, int length);
uint8_t UART_Data_Available	(void);
uint8_t	UART_Buffer_Read	(void);


#endif /* UART_H_ */