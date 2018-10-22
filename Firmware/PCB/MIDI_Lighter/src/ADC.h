/*
 * ADC.h
 *
 * Created: 30/09/2018 20:19:50
 *  Author: Chris
 */ 


#ifndef ADC_H_
#define ADC_H_

#include <avr/io.h>

void			ADC_Init(void);
uint32_t	ADC_Get_Value(void);


#endif /* ADC_H_ */