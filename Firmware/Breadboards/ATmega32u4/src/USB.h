#ifndef USB_H_
#define USB_H_

#include <avr/io.h>
#include <stdint.h>
#include <util/atomic.h>

#define LOW_BYTE(x)   ((x) & 0xFF)
#define HIGH_BYTE(x)  (((x)>>8) & 0xFF)

// Set the PLL Prescaler Value for the PLLCSR-Register
// The PLL is used to generate the clock for the USB-Module 
// The PLL-input frequency needs to be 8 MHz -> Datasheet: Section 9.6
#if F_CPU == 8000000
	#define PLL_PRESCALER_REGISTER	0x00
#else
	#define PLL_PRESCALER_REGISTER	0x10
#endif

// =========================== Endpoint Defines ===========================
#define NUMBER_ENDPOINTS			2	// Number without Endpoint 0

//	TYPE:        0 = Control, 1 = Isochron, 2 = Bulk 3 = Interrupt
//	DIRECTION:   0 = OUT, 1 = IN (except Control)
//	SIZE:        0 = 8 Bytes, 1 = 16 Bytes, 2 = 32 Bytes, 3 = 64 Bytes, 4 = 128 Bytes (only EP1), 5 = 256 Bytes (EP1), further reserved
//	BANK:        0 = 1 Bank, 1 = 2 Banks, further reserved
#define ENDPOINT_TYPE_CONTROL		0
#define ENDPOINT_TYPE_ISOCHRON		1
#define ENDPOINT_TYPE_BULK			2
#define ENDPOINT_TYPE_INTERRUPT		3

#define ENDPOINT_DIRECTION_OUT		0	// View from the Host -> MCU receives at an OUT-Endpoint
#define ENDPOINT_DIRECTION_IN		1	// View from the Host -> MCU send over an IN-Endpoint

#define ENDPOINT_SIZE_8				0
#define ENDPOINT_SIZE_16			1
#define ENDPOINT_SIZE_32			2
#define ENDPOINT_SIZE_64			3

#define ENDPOINT_BANKS_1			0
#define ENDPOINT_BANKS_2			1


// Endpoint 0 (needs to be of control type)
#define ENDPOINT_0					0
#define ENDPOINT_0_TYPE				ENDPOINT_TYPE_CONTROL
#define ENDPOINT_0_DIRECTION		ENDPOINT_DIRECTION_OUT
#define ENDPOINT_0_SIZE				ENDPOINT_SIZE_8
#define ENDPOINT_0_BANK				ENDPOINT_BANKS_1
#define ENDPOINT_0_FIFO_SIZE		8

// Endpoint 1 (Bulk / Input)
#define ENDPOINT_1					1
#define ENDPOINT_1_TYPE				ENDPOINT_TYPE_BULK
#define ENDPOINT_1_DIRECTION		ENDPOINT_DIRECTION_IN
#define ENDPOINT_1_SIZE				ENDPOINT_SIZE_64
#define ENDPOINT_1_BANK				ENDPOINT_BANKS_1
#define ENDPOINT_1_FIFO_SIZE		64

// Endpoint 2 (Bulk / Output)
#define ENDPOINT_2					2
#define ENDPOINT_2_TYPE				ENDPOINT_TYPE_BULK
#define ENDPOINT_2_DIRECTION		ENDPOINT_DIRECTION_OUT
#define ENDPOINT_2_SIZE				ENDPOINT_SIZE_64
#define ENDPOINT_2_BANK				ENDPOINT_BANKS_1
#define ENDPOINT_2_FIFO_SIZE		64

#define ENDPOINT_3					3
#define ENDPOINT_3_TYPE				ENDPOINT_TYPE_INTERRUPT
#define ENDPOINT_3_DIRECTION		ENDPOINT_DIRECTION_IN
#define ENDPOINT_3_SIZE				ENDPOINT_SIZE_64
#define ENDPOINT_3_BANK				ENDPOINT_BANKS_1
#define ENDPOINT_3_FIFO_SIZE		64

#define ENDPOINT_4					4
#define ENDPOINT_4_TYPE				ENDPOINT_TYPE_INTERRUPT
#define ENDPOINT_4_DIRECTION		ENDPOINT_DIRECTION_OUT
#define ENDPOINT_4_SIZE				ENDPOINT_SIZE_64
#define ENDPOINT_4_BANK				ENDPOINT_BANKS_1
#define ENDPOINT_4_FIFO_SIZE		64

#define ENDPOINT_5					5
#define ENDPOINT_6					6

// ...
#define W_TOTAL_LENGTH				9+9+(7*NUMBER_ENDPOINTS)

// =========================== Device Descriptor Content Defines ===========================
#define DESCRIPTOR_DEVICE_ID_VENDOR		0x03EB		// Atmel Code -> given by usb.org
#define DESCRIPTOR_DEVICE_ID_PRODUCT	0x0001		

// =========================== Interface Descriptor Content Defines ===========================
#define DESCRIPTOR_INTERFACE_INTERFACE_CLASS		0xFF
#define DESCRIPTOR_INTERFACE_INTERFACE_SUB_CLASS	0xFF
#define DESCRIPTOR_INTERFACE_INTERFACE_PROTOCOL		0xFF

// =========================== Descriptor Type Defines ===========================
#define DESCRIPTOR_TYPE_DEVICE			1
#define DESCRIPTOR_TYPE_CONFIGURATION	2
#define DESCRIPTOR_TYPE_INTERFACE		4
#define DESCRIPTOR_TYPE_ENDPOINT		5
#define DESCRIPTOR_TYPE_STRING			3

// =========================== Descriptor Index Defines ===========================
#define LANGUAGE_INDEX					0   // LanguageDescriptorIndex
#define MANUFACTURER_INDEX				1   // ManufacturerStringIndex
#define PRODUCT_INDEX					2   // ProductStringIndex
#define SERIAL_NUMBER_INDEX				3   // SerialNumberStringIndex
#define INTERFACE_INDEX					2   // InterfaceStringIndex

// =========================== Setup Package Defines ===========================
#define BM_REQUEST_TYPE_TYPE_MASK		0x60
#define BM_REQUEST_TYPE_TYPE_STANDARD	0
#define BM_REQUEST_TYPE_TYPE_VENDOR		2

#define B_REQUEST_GET_STATUS			0x00
#define B_REQUEST_CLEAR_FEATURE 		0x01
#define B_REQUEST_SET_FEATURE 			0x03
#define B_REQUEST_SET_ADDRESS	 		0x05
#define B_REQUEST_GET_DESCRIPTOR  		0x06
#define B_REQUEST_SET_DESCRIPTOR  		0x07
#define B_REQUEST_GET_CONFIGURATION 	0x08
#define B_REQUEST_SET_CONFIGURATION 	0x09

#define B_REQUEST_SET_ADDRESS_MASK		0x7F

// =========================== Buffer Defines ===========================
#define USB_BUFFER_SIZE					64

// =========================== Send Data Defines ===========================
#define USB_SEND_TIMEOUT				1000000		

// =========================== Interface Functions ===========================
void		USB_Init(void);
uint8_t		USB_Configured(void);
uint8_t		USB_Data_Received_Once(void);
uint8_t		USB_Data_Arrived(void);

uint8_t		USB_Plugged(void);
uint16_t	USB_Data_Available(void);
char		USB_Get_Char(void);



uint8_t		USB_Send_Char(char data);
uint8_t		USB_Send_String(char* data, int length);
uint8_t		USB_Send_Int(int val, int numbers);

void		USB_Buffer_RX_Reset(void);
uint16_t	USB_Buffer_TX_FillCount(void);
void		USB_Buffer_TX_Reset(void);

#endif // USB_H_