/*
 * Pin.h
 *
 * Created: 05/10/2018 22:15:22
 *  Author: Chris
 */ 


#ifndef PIN_H_
#define PIN_H_


/////////////////
// LED Defines //
/////////////////
#define LED0				PINF0
#define LED1				PINF1
#define LED2				PINF4
#define LEDS				PINF5

#define LED_ON(_LED_)		PORTF &= ~(1 << _LED_)
#define LED_OFF(_LED_)		PORTF |= (1 << _LED_)
#define LED_TOGGLE(_LED_)	PORTF ^= _BV(_LED_)


//////////////////
// Strip Enable //
//////////////////
#define STRIP_PIN_EN		PINF7
#define STRIP_ENABLE()		PORTF |= (1 << STRIP_PIN_EN)
#define STRIP_DISABLE()		PORTF &= ~(1 << STRIP_PIN_EN)


////////////////
// Debug Pins //
////////////////
#define DEBUG_PIN_PB4		PINB4
#define DEBUG_PIN_MISO		PINB3
#define DEBUG_PIN_MOSI		PINB2
#define DEBUG_PIN_SCK		PINB1
#define DEBUG_PIN_SS		PINB0
#define DEBUG_PIN_TXD		PIND3
#define DEBUG_PIN_SDA		PIND1
#define DEBUG_PIN_SCL		PIND0

#define DEBUG_PORT_PB4		PORTB
#define DEBUG_PORT_MISO		PORTB
#define DEBUG_PORT_MOSI		PORTB
#define DEBUG_PORT_SCK		PORTB
#define DEBUG_PORT_SS		PORTB
#define DEBUG_PORT_TXD		PORTD
#define DEBUG_PORT_SDA		PORTD
#define DEBUG_PORT_SCL		PORTD

#define DEBUG_DDR_PB4		DDRB
#define DEBUG_DDR_MISO		DDRB
#define DEBUG_DDR_MOSI		DDRB
#define DEBUG_DDR_SCK		DDRB
#define DEBUG_DDR_SS		DDRB
#define DEBUG_DDR_TXD		DDRD
#define DEBUG_DDR_SDA		DDRD
#define DEBUG_DDR_SCL		DDRD


#define DEBUG_SET_OUTPUT(_DDR_, _PIN_)	_DDR_ |= (1 << _PIN_)

#define DEBUG_ON(_PORT_, _PIN_)			_PORT_ |= (1 << _PIN_)
#define DEBUG_OFF(_PORT_, _PIN_)		_PORT_ &= ~(1 << _PIN_)
#define DEBUG_TOGGLE(_PORT_, _PIN_)		_PORT_ ^= _BV(_PIN_)

#endif /* PIN_H_ */