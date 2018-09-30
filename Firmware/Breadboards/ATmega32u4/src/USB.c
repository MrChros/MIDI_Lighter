#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#include "USB.h"
#include "USB_Descriptor.h"
#include "WinUSB.h"


#define FALSE	0
#define TRUE	1

volatile struct BUFFER_s
{
	uint8_t data[USB_BUFFER_SIZE];
	uint8_t	read;
	uint8_t	write;
} _RX_Buffer, _TX_Buffer;

// =========================== Global Library Variables ===========================
volatile uint8_t	_Used_Configuration;
volatile uint8_t	_USB_Data_Received_Once;
volatile uint8_t	_USB_In_Data_Available;

// =========================== Internal Library Functions Declaration ===========================
void USB_Init_Endpoint			(uint8_t number, uint8_t type, uint8_t direction, uint8_t size, uint8_t bank);
void USB_Send_Descriptor		(uint8_t descriptor[], uint16_t descriptorSize);
void USB_Endpoint_0_Setup		(void);
void USB_Endpoint_IN_Handler	(void);
void USB_Endpoint_OUT_Handler	(void);

// =========================== Internal Library Functions ===========================
void USB_Init_Endpoint(uint8_t number, uint8_t type, uint8_t direction, uint8_t size, uint8_t bank)
{
	// See Register Details -> Datasheet: Section 22.18.2
	UENUM	 = number;										// Select Endpoint
	UECONX  |= (1 << EPEN);									// Enable selected Endpoint
	UECFG0X  = (type << EPTYPE0) | (direction << EPDIR);	// Set Type and Direction
	UECFG1X  = (size << EPSIZE0) | (bank	  << EPBK0);	// Set Size and Banks
	UECFG1X |= (1 << ALLOC);								// Allocate Memory for this Endpoint (Clear this Bit to free the memory)
	
	// Check if Endpoint initialization is not correct
	if((UESTA0X & (1 << CFGOK)) == 0)
	{
		// Can add an error handling here...
	}
}

void USB_Send_Descriptor(uint8_t descriptor[], uint16_t descriptorSize)
{
	uint8_t FIFO_Counter = 0;
	
	for(uint16_t i=1;i<=descriptorSize;i++)
	{
		if((UEINTX & (1 << RXOUTI)) == (1 << RXOUTI))
		{
			return;	// Abort here (direction from Host)
		}
		
		UEDATX = pgm_read_byte(&descriptor[i-1]);
		FIFO_Counter++;
		
		if(FIFO_Counter == ENDPOINT_0_FIFO_SIZE)
		{
			UEINTX &= ~(1 << TXINI);							// Send data and clear FIFO
			while(!(UEINTX & ((1 << RXOUTI) | (1 << TXINI))));	// Wait until bank is free or ACK from Host
			
			FIFO_Counter = 0;
		}
	}
	
	if(!(UEINTX & (1 << RXOUTI)))							// Check if leftover
	{
		UEINTX &= ~(1 << TXINI);							// Send data and clear FIFO
		while(!(UEINTX & ((1 << RXOUTI) | (1 << TXINI))));	// Wait until bank is free or ACK from Host
	}	
	UEINTX &= ~(1 << RXOUTI);								// Clear interrupt flag
}

void USB_Endpoint_0_Setup(void)
{
	// Descriptors were here before...
		
	uint8_t bmRequestType;
	uint8_t bRequest;
	uint8_t wValueH, wValueL;
	uint8_t wIndexH, wIndexL;
	uint8_t wLengthH, wLengthL;
	
	uint8_t descriptorType;
	uint8_t descriptorIndex;
	uint16_t descriptorSize;
	
	// Read received bytes from FIFO
	bmRequestType 	= UEDATX;
	bRequest 		= UEDATX;
	wValueL 		= UEDATX;
	wValueH 		= UEDATX;
	wIndexL 		= UEDATX;
	wIndexH 		= UEDATX;
	wLengthL 		= UEDATX;
	wLengthH 		= UEDATX;
	
	// Acknowledge the interrupt
	UEINTX = ~((1 << RXSTPI) | (1 << RXOUTI));	// Data sheet: Section 22.12
	
	////////////////////////
	// WinUSB Descriptors //
	////////////////////////
	if (WinUSB_Request_for_OS_String_Descriptor(bmRequestType, bRequest, wValueL, wValueH, wIndexL, wIndexH, wLengthL, wLengthH) == TRUE)
	{
		uint16_t wLength = (((uint16_t)wLengthH) << 8) | ((uint16_t)wLengthL & 0x00FF);
		USB_Send_Descriptor((uint8_t*) WinUSB_OS_String_Descriptor, wLength);
		return;
	}
	
	if (WinUSB_Request_for_OS_Feature_Descriptor(bmRequestType, bRequest, wValueL, wValueH, wIndexL, wIndexH, wLengthL, wLengthH) == TRUE)
	{
		uint16_t wLength = (((uint16_t)wLengthH) << 8) | ((uint16_t)wLengthL & 0x00FF);
		USB_Send_Descriptor((uint8_t*) WinUSB_OS_Feature_Descriptor, wLength);
		return;
	}
	
	// MS Extended Properties Descriptor
	if (WinUSB_Request_for_OS_Extended_Properties_Descriptor(bmRequestType, bRequest, wValueL, wValueH, wIndexL, wIndexH, wLengthL, wLengthH) == TRUE)
	{
		uint16_t wLength = (((uint16_t)wLengthH) << 8) | ((uint16_t)wLengthL & 0x00FF);
		USB_Send_Descriptor((uint8_t*) WinUSB_OS_Extended_Properties_Descriptor, wLength);
		return;
	}
	//////////////////////////
	// !WinUSB Descriptors! //
	//////////////////////////	
	
	
	// Parse the received data
	if((bmRequestType & BM_REQUEST_TYPE_TYPE_MASK) == BM_REQUEST_TYPE_TYPE_STANDARD)
	{
		// More information about all the requests: http://www.usbmadesimple.co.uk/ums_4.htm
		switch(bRequest)
		{
			case B_REQUEST_GET_STATUS:
				if((bmRequestType & 0x0F) == 0x00)	// Device
				{
					UEDATX  = 0x01;	// Provide real status here...
					UEDATX  = 0x00;	// Provide real status here...
				}
				else if((bmRequestType & 0x0F) == 0x01)	// Interface
				{
					UEDATX  = 0;	// Provide real status here...
					UEDATX  = 0;	// Provide real status here...
				}
				else if((bmRequestType & 0x0F) == 0x02)	// Endpoint
				{
					UEDATX  = 0;	// Provide real status here...
					UEDATX  = 0;	// Provide real status here...
				}
				UEINTX &= ~(1 << TXINI);				// Send data and clear FIFO
				while (!(UEINTX & (1 << RXOUTI))) {  }  // Wait for Zero-Length-Packet from Host
				UEINTX &= ~(1 << RXOUTI);				// Clear interrupt flag
				break;
				
			case B_REQUEST_CLEAR_FEATURE:
				// Not implemented yet ...
				break;
				
			case B_REQUEST_SET_FEATURE:
				// Not implemented yet ...
				break;
			
			case B_REQUEST_SET_ADDRESS:
			
				UDADDR |= (wValueL & B_REQUEST_SET_ADDRESS_MASK);	// Set the new address but keep MSB at 0 (the mask takes care of that)
				UEINTX &= ~(1 << TXINI);							// Send data and clear FIFO
				while (!(UEINTX & (1 << TXINI))) {  }				// Wait for Zero-Length-Packet transit to be finished
				UDADDR |= (1 << ADDEN);								// Set MSB in UDADDR -> activate new address
				break;
				
			case B_REQUEST_GET_DESCRIPTOR:
				descriptorType	= wValueH;
				descriptorIndex = wValueL;
				
				switch(descriptorType)
				{
					case DESCRIPTOR_TYPE_DEVICE:
						descriptorSize = pgm_read_byte(&deviceDescriptor[0]);
						USB_Send_Descriptor((uint8_t*) deviceDescriptor, descriptorSize);
						break;
						
					case DESCRIPTOR_TYPE_CONFIGURATION:
						descriptorSize = wLengthL;
						// In case a longer length is expected -> correct it
						if((wLengthH > 0) || (wLengthL > W_TOTAL_LENGTH) || (wLengthL == 0))
						{
							descriptorSize = W_TOTAL_LENGTH;
						}
						USB_Send_Descriptor((uint8_t*) configurationDescriptor, descriptorSize);
						break;
						
					case DESCRIPTOR_TYPE_STRING:
						switch(descriptorIndex)
						{
							case LANGUAGE_INDEX:
								descriptorSize = pgm_read_byte(&languageDescriptor[0]);
								USB_Send_Descriptor((uint8_t*) languageDescriptor, descriptorSize);
								break;
								
							case MANUFACTURER_INDEX:
								descriptorSize = pgm_read_byte(&manufacturerDescriptor[0]);
								USB_Send_Descriptor((uint8_t*) manufacturerDescriptor, descriptorSize);
								break;
							
							case PRODUCT_INDEX:
								descriptorSize = pgm_read_byte(&productDescriptor[0]);
								USB_Send_Descriptor((uint8_t*) productDescriptor, descriptorSize);
								break;
							
							case SERIAL_NUMBER_INDEX:
								descriptorSize = pgm_read_byte(&serialNumberDescriptor[0]);
								USB_Send_Descriptor((uint8_t*) serialNumberDescriptor, descriptorSize);
								break;
								
							default:
								break;
						}
						break;
						
					default:
						break;
				}
				break;
				
			case B_REQUEST_SET_DESCRIPTOR:
				UECONX |= (1 << STALLRQ);
				break;
				
			case B_REQUEST_GET_CONFIGURATION:
				UECONX |= (1 << STALLRQ);
				break;
				
			case B_REQUEST_SET_CONFIGURATION:
				for (uint8_t i=NUMBER_ENDPOINTS;i==1;i--) // Deactivate used Endpoints
				{                                
					UENUM	 = i;				// Select Endpoint
					UECONX	&= ~(1 << EPEN);	// Disable Endpoint
					UECFG1X &= ~(1 << ALLOC);	// Free memory
				}
				
				// From here: Initialize every Endpoint
				USB_Init_Endpoint(ENDPOINT_1, ENDPOINT_1_TYPE, ENDPOINT_1_DIRECTION, ENDPOINT_1_SIZE, ENDPOINT_1_BANK);
				UEIENX |= (1 << NAKINE);	// Enable NAK Interrupt for Endpoints with direction IN
				
				USB_Init_Endpoint(ENDPOINT_2, ENDPOINT_2_TYPE, ENDPOINT_2_DIRECTION, ENDPOINT_2_SIZE, ENDPOINT_2_BANK);
				UEIENX |= (1 << RXOUTE);	// Enable RX OUT Interrupt for Endpoints with direction OUT

				USB_Init_Endpoint(ENDPOINT_3, ENDPOINT_3_TYPE, ENDPOINT_3_DIRECTION, ENDPOINT_3_SIZE, ENDPOINT_3_BANK);
				UEIENX |= (1 << NAKINE);	// Enable NAK Interrupt for Endpoints with direction IN
				
				USB_Init_Endpoint(ENDPOINT_4, ENDPOINT_4_TYPE, ENDPOINT_4_DIRECTION, ENDPOINT_4_SIZE, ENDPOINT_4_BANK);
				UEIENX |= (1 << RXOUTE);	// Enable RX OUT Interrupt for Endpoints with direction OUT
				
				// In case of more Endpoints -> add them here...

				UENUM = ENDPOINT_0; 				// Select Endpoint 0
				UEINTX	&= ~(1 << TXINI);			// Send Packet with no data (Zero Length Packet) and clear the Endpoint 0 Bank
				while(!(UEINTX & (1<<TXINI))) { }	// Wait until finished...
					
				_Used_Configuration = wValueL;
					
				break;	
			
			default:
				UECONX |= (1 << STALLRQ);
				break;			
		}
	}
	else if((bmRequestType & BM_REQUEST_TYPE_TYPE_MASK) == BM_REQUEST_TYPE_TYPE_VENDOR)
	{
		// Nothing here yet...
		UECONX |= (1 << STALLRQ);
	}
}

void USB_Endpoint_IN_Handler(void)
{
	if((UEINTX & (1 << NAKINI)) == (1 << NAKINI))		// IN Request from the host has been received
	{
		UEINTX &= ~(1 << NAKINI);						// Clear the interrupt flag
		if((UEINTX & (1 << TXINI)) == (1 << TXINI))		// Check if bank free
		{
			UEINTX &= ~(1 << TXINI);					// Occupy the bank
			while(_TX_Buffer.read != _TX_Buffer.write)
			{
				// Check if it is allowed to write in FIFO
				if((UEINTX & (1 << RWAL)) == 0)
				{
					// If not, FIFO must be full -> data send
					UEINTX &= ~(1 << FIFOCON);			// Send the data
					// Wait until FIFO is empty
					while((UEINTX & (1 << RWAL)) == 0) { }
				}
				
				UEDATX = _TX_Buffer.data[_TX_Buffer.read++];	// Write data to FIFO
				
				if(_TX_Buffer.read==USB_BUFFER_SIZE)
				{
					_TX_Buffer.read = 0;
				}
			}
			
			UEINTX &= ~(1 << FIFOCON);					// Send the data
		}
	}
}

void USB_Endpoint_OUT_Handler(void)
{
	uint8_t Received_Byte;
	
	if((UEINTX & (1 << RXOUTI)) == (1 << RXOUTI))		// Check if data is available
	{
		UEINTX &= ~(1 << RXOUTI);						// Confirm, send ACK
		for(int i=0;i<ENDPOINT_2_FIFO_SIZE;i++)
		{
			if((UEINTX & (1 << RWAL)) == (1 << RWAL))	// Check if data can be read from FIFO
			{
				Received_Byte = UEDATX;					// Read from FIFO
				
				if(_RX_Buffer.write+1 == _RX_Buffer.read)
				{
					// Overflow will happen
					_RX_Buffer.data[_RX_Buffer.read] = Received_Byte;
				}
				else
				{
					// Everything ok...
					_RX_Buffer.data[_RX_Buffer.write++] = Received_Byte;
					if(_RX_Buffer.write == USB_BUFFER_SIZE)
					{
						_RX_Buffer.write = 0;
					}
					_USB_Data_Received_Once = TRUE;
				}
			}
			else
			{
				break;
			}
		}
		UEINTX &= ~(1 << FIFOCON);						// Free current bank
	}
}

// =========================== Interrupt Functions ===========================
ISR(USB_GEN_vect)
{
	// End of Reset has been triggered
	if((UDINT & (1 << EORSTI)) == (1 << EORSTI))
	{
		UDINT  &= ~(1 << EORSTI);	// Clear the Interrupt Bit
		USB_Init_Endpoint(ENDPOINT_0, ENDPOINT_0_TYPE, ENDPOINT_0_DIRECTION, ENDPOINT_0_SIZE, ENDPOINT_0_BANK); // Init Endpoint 0
		UEIENX |=  (1 << RXSTPE);	// Enable the Interrupt for the case a Setup Event
		_Used_Configuration = 0;
	}
	else
	{
		UDINT = 0;
	}
}

ISR(USB_COM_vect)
{
	// Endpoint interrupt has been triggered
	// The UEINT register is one-hot-coded for each endpoint
	// Data sheet: Section 22.18.2 (last register)
	
	// UENUM = ENDPOINT_x (Select the Endpoint, which has triggered the interrupt)
	switch(UEINT)
	{
		case 1:		// Endpoint 0
			UENUM = ENDPOINT_0;
			// If interrupt flag for SETUP Event is enabled
			if((UEINTX & (1 << RXSTPI)) == (1 << RXSTPI))
			{
				// Run the Setup Function
				USB_Endpoint_0_Setup();
			}
			else
			{
				UEINTX = 0;
			}
			break;
			
		case 2:		// Endpoint 1
			UENUM = ENDPOINT_1;
			USB_Endpoint_IN_Handler();	// Transmit Data
			break;
		
		case 4:		// Endpoint 2
			UENUM = ENDPOINT_2;
			USB_Endpoint_OUT_Handler();	// Receive Data
			break;

		case 8:		// Endpoint 3
			UENUM = ENDPOINT_3;
			break;
			
		case 16:	// Endpoint 4
			UENUM = ENDPOINT_4;
			break;
			
		case 32:	// Endpoint 5
			UENUM = ENDPOINT_5;
			break;
			
		case 64:	// Endpoint 6
			UENUM = ENDPOINT_6;
			break;

		default:
		break;
	}
}

// =========================== Interface Functions ===========================

void USB_Init(void)
{
	UHWCON =  (1 << UVREGE); 				// Enable the PAD Regulator for the D+ and D- line
	USBCON = ((1 << USBE) | (1 << FRZCLK));
	
	// PLL
	PLLCSR	= PLL_PRESCALER_REGISTER;		// Set PLL Prescaler depending on the CPU frequency
	PLLCSR |= (1 << PLLE);  				// Start PLL (PLLEnable = 1)
	while (!(PLLCSR & (1 << PLOCK))) {  }	// Wait until PLL is running
		
	USBCON = ((1 << USBE) | (1 << OTGPADE));
	UDCON &= ~(1 << LSM);					// Select Pull-Up for D+ -> USB Full Speed
	
	UDCON &= ~(1 << DETACH); 				// Disable the detach bit in the UDCON Register -> Pull-up to the D+ and D- line will be connected (Datasheet: Section 21.8)
	UDIEN  =  (1 << EORSTE);  				// Enable Interrupt for -> End Of Reset Interrupt (see "ISR(USB_GEN_vect)" above)
	
	_Used_Configuration = 0;
	
	// Initialize Receive (RX) Buffer
	USB_Buffer_RX_Reset();
	
	// Initialize Transmit (TX) Buffer
	USB_Buffer_TX_Reset();
	
	_USB_Data_Received_Once	= FALSE;
	_USB_In_Data_Available	= FALSE;
}

uint8_t USB_Configured(void)
{
	return _Used_Configuration;
}

uint8_t	USB_Data_Received_Once(void)
{
	return _USB_Data_Received_Once;
}

uint8_t USB_Data_Arrived(void)
{
	return _USB_In_Data_Available;
}

uint8_t USB_Plugged(void)
{
	return (uint8_t)(USBSTA & (1 << VBUS));
}

uint16_t USB_Data_Available(void)
{
	if(_RX_Buffer.write >= _RX_Buffer.read)
		return (uint16_t)(_RX_Buffer.write-_RX_Buffer.read);
	else
		return (uint16_t)((_RX_Buffer.write+USB_BUFFER_SIZE)-_RX_Buffer.read);
}

char USB_Get_Char(void)
{
	uint8_t Return_Value = 0;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		Return_Value = _RX_Buffer.data[_RX_Buffer.read++];
	}
	
	if(_RX_Buffer.read == USB_BUFFER_SIZE)
	{
		_RX_Buffer.read = 0;
	}
		
	return Return_Value;
}

// =========================== Send Functions ===========================

uint8_t USB_Send_Char(char data)
{
	uint32_t Send_Timeout = 0;
	
	while((_TX_Buffer.write+1 == _TX_Buffer.read) || (_TX_Buffer.write+1 == USB_BUFFER_SIZE && _TX_Buffer.read == 0))
	{
		// Buffer full...
		if(Send_Timeout==USB_SEND_TIMEOUT)
		{
			// Abort...
			return FALSE;
		}
		
		Send_Timeout++;
	}
	
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		_TX_Buffer.data[_TX_Buffer.write++] = data;
		
		if(_TX_Buffer.write == USB_BUFFER_SIZE)
		{
			_TX_Buffer.write = 0;
		}
	}
	return TRUE;
}

uint8_t USB_Send_String(char* data, int length)
{
	int i;
	uint8_t Send_Success = FALSE;
	for(i=0;i<length;i++)
	{
		Send_Success = USB_Send_Char(data[i]);
		if(Send_Success == FALSE)
		{
			return FALSE;
		}
	}
	return TRUE;
}

uint8_t USB_Send_Int(int val, int numbers)
{
	int i, num;
	int factor = 1;
	uint8_t Send_Success = FALSE;
	
	for(i=0;i<numbers-1;i++)
	{
		factor *= 10;
	}
	for(i=0;i<numbers;i++)
	{
		num = (int)(val / factor);
		Send_Success = USB_Send_Char((char)('0'+num));
		if(Send_Success == FALSE)
		{
			return FALSE;
		}
		
		val %= factor;
		factor /= 10;
	}
	return TRUE;
}

// =========================== Overflow Functions ===========================

void USB_Buffer_RX_Reset(void)
{
	_RX_Buffer.read		= 0;
	_RX_Buffer.write	= 0;
}

uint16_t USB_Buffer_TX_FillCount(void)
{
	if(_TX_Buffer.write >= _TX_Buffer.read)
		return (uint16_t)(_TX_Buffer.write-_TX_Buffer.read);
	else
		return (uint16_t)((_TX_Buffer.write+USB_BUFFER_SIZE)-_TX_Buffer.read);
}

void USB_Buffer_TX_Reset(void)
{
	_TX_Buffer.read		= 0;
	_TX_Buffer.write	= 0;
}