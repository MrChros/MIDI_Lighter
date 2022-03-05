/*
 * WinUSB.h
 *
 * Created: 10.01.2016 12:25:18
 *  Author: jan
 */ 


#ifndef WINUSB_H_
#define WINUSB_H_

#include "UART.h"

#define VENDOR_CODE	0x1A

#define LED0_PORT	PORTF
#define LED1_PORT	PORTF
#define LED2_PORT	PORTB

#define LED0_PIN	PF6
#define LED1_PIN	PF7
#define LED2_PIN	PB6

#define LED_ON( _PORT_, _PIN_)		_PORT_ &= ~(1 << _PIN_)
#define LED_OFF( _PORT_, _PIN_)		_PORT_ |= (1 << _PIN_)
#define LED_TOGGLE( _PORT_, _PIN_)	_PORT_ ^= _BV(_PIN_)

uint8_t WinUSB_Request_for_OS_String_Descriptor(uint8_t bmRequestType, uint8_t bRequest, uint8_t wValueL, uint8_t wValueH, uint8_t wIndexL, uint8_t wIndexH, uint8_t wLengthL, uint8_t wLengthH);
uint8_t WinUSB_Request_for_OS_Feature_Descriptor(uint8_t bmRequestType, uint8_t bRequest, uint8_t wValueL, uint8_t wValueH, uint8_t wIndexL, uint8_t wIndexH, uint8_t wLengthL, uint8_t wLengthH);
uint8_t WinUSB_Request_for_OS_Extended_Properties_Descriptor(uint8_t bmRequestType, uint8_t bRequest, uint8_t wValueL, uint8_t wValueH, uint8_t wIndexL, uint8_t wIndexH, uint8_t wLengthL, uint8_t wLengthH);


// OS String Descriptor
static const uint8_t PROGMEM WinUSB_OS_String_Descriptor[] =
{
	18,						// bLength = 18 (0x12), Size of Descriptor in Byte
	0x03,					// bDescriptorType
	0x4D, 0x00,				// qwSignature 'MSFT100'
	0x53, 0x00,
	0x46, 0x00,
	0x54, 0x00,
	0x31, 0x00,
	0x30, 0x00,
	0x30, 0x00,
	VENDOR_CODE,			// bMS_VendorCode
	0x00					// bPad
};


// OS Feature Descriptor
static const uint8_t PROGMEM WinUSB_OS_Feature_Descriptor[] =
{
	0x28, 0x00, 0x00, 0x00,							// dwLength		0x0028
	0x00, 0x01,										// bcdVersion	0x0100
	0x04, 0x00,										// wIndex		0x0004
	0x01, 											// bCount
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,		// Reserved
	0x00,											// bFristInterfaceNumber
	0x01,											// Reserved
	0x57, 0x49, 0x4E, 0x55, 0x53, 0x42, 0x00, 0x00,	// compatibleID 'WINUSB'
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // subCompatibleID
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00				// Reserved
};


// OS Extended Properties Descriptor
static const uint8_t PROGMEM WinUSB_OS_Extended_Properties_Descriptor[] =
{
	0x8E, 0x00, 0x00, 0x00,		// dwLength		142 bytes
	0x00, 0x01,					// bcdVersion	0x0100
	0x05, 0x00,					// wIndex		0x0005
	0x01, 0x00,					// wCount		0x0002
	
	0x84, 0x00, 0x00, 0x00,		// dwSize 132 bytes
	0x01, 0x00, 0x00, 0x00,		// dwPropertyDataType
	
	0x28, 0x00,					// wPropertyNameLength 40 bytes
	// bPropertyName "DeviceInterfaceGUID"
	0x44, 0x00, 0x65, 0x00, 0x76, 0x00, 0x69, 0x00, 0x63, 0x00, 0x65, 0x00, 
	0x49, 0x00, 0x6E, 0x00, 0x74, 0x00, 0x65, 0x00, 0x72, 0x00, 0x66, 0x00,
	0x61, 0x00, 0x63, 0x00, 0x65, 0x00, 0x47, 0x00, 0x55, 0x00, 0x49, 0x00,
	0x44, 0x00, 0x00, 0x00,
	
	0x4E, 0x00, 0x00, 0x00,		// dwPropertyDataLength 78 bytes
	// bPropertyData "{0049DC4F-75DF-419F-B3EA-4BBDC1E08253}"
	0x7B, 0x00, 0x30, 0x00, 0x30, 0x00, 0x34, 0x00, 0x39, 0x00, 0x44, 0x00,
	0x43, 0x00, 0x34, 0x00, 0x46, 0x00, 0x2D, 0x00, 0x37, 0x00, 0x35, 0x00,
	0x44, 0x00, 0x46, 0x00, 0x2D, 0x00, 0x34, 0x00, 0x31, 0x00, 0x39, 0x00,
	0x46, 0x00, 0x2D, 0x00, 0x42, 0x00, 0x33, 0x00, 0x45, 0x00, 0x41, 0x00,
	0x2D, 0x00, 0x34, 0x00, 0x42, 0x00, 0x42, 0x00, 0x44, 0x00, 0x43, 0x00,
	0x31, 0x00, 0x45, 0x00, 0x30, 0x00, 0x38, 0x00, 0x32, 0x00, 0x35, 0x00,
	0x33, 0x00, 0x7D, 0x00, 0x00, 0x00
};


uint8_t WinUSB_Request_for_OS_String_Descriptor(uint8_t bmRequestType, uint8_t bRequest, uint8_t wValueL, uint8_t wValueH, uint8_t wIndexL, uint8_t wIndexH, uint8_t wLengthL, uint8_t wLengthH)
{
	if (bmRequestType == 128)
	{
		if (bRequest == 0x06)
		{
			if (wValueH == 0x03)
			{
				if (wValueL == 0xEE)
				{
					if ((wIndexL == 0x00) && (wIndexH == 0x00))
					{
						return 1;
					}
				}
			}
		}
	}
	return 0;
}

uint8_t WinUSB_Request_for_OS_Feature_Descriptor(uint8_t bmRequestType, uint8_t bRequest, uint8_t wValueL, uint8_t wValueH, uint8_t wIndexL, uint8_t wIndexH, uint8_t wLengthL, uint8_t wLengthH)
{
	if (bmRequestType == 0xC0)
	{
		if (bRequest == VENDOR_CODE)
		{
			if (wValueH == 0x00)
			{
				if ((wIndexL == 0x04) && (wIndexH == 0x00))
				{
					return 1;
				}
			}
		}
	}
	return 0;
}

uint8_t WinUSB_Request_for_OS_Extended_Properties_Descriptor(uint8_t bmRequestType, uint8_t bRequest, uint8_t wValueL, uint8_t wValueH, uint8_t wIndexL, uint8_t wIndexH, uint8_t wLengthL, uint8_t wLengthH)
{
	if (bmRequestType == 0xC1)
	{
		if (bRequest == VENDOR_CODE)
		{
			if ((wValueH == 0x00) && (wValueL == 0x00))
			{
				if ((wIndexL == 0x05) && (wIndexH == 0x00))
				{
					return 1;
				}
			}
		}
	}
	return 0;
}


#endif // WINUSB_H_