/*
 * USB_Descriptor.h
 *
 * Created: 16/06/2018 10:51:24
 *  Author: Chris
 */ 


#ifndef USB_DESCRIPTOR_H_
#define USB_DESCRIPTOR_H_

// =========================== Descriptors (saved in flash memory)
// Device Descriptor
static const uint8_t PROGMEM deviceDescriptor[] =
{
	18,																				// bLength = 18 (0x12), Size of Descriptor in Byte
	DESCRIPTOR_TYPE_DEVICE,															// bDescriptorType = 0x01, Device Constant = 1
	0x00, 0x02,																		// bcdUSB = 0x0200, USB_Spec2_0
	0xFF,																			// bDeviceClass = 0x00, Class-code: defined on Interface Level
	0x00,																			// bDeviceSubClass = 0x00
	0x00,																			// bDeviceProtocoll = 0x00
	ENDPOINT_0_FIFO_SIZE,															// bMaxPacketSize0 = EP0FS, max. Package size of Endpoint 0 (here 8 Byte)
	LOW_BYTE(DESCRIPTOR_DEVICE_ID_VENDOR), HIGH_BYTE(DESCRIPTOR_DEVICE_ID_VENDOR),	// idVendor, Vendor ID
	LOW_BYTE(DESCRIPTOR_DEVICE_ID_PRODUCT), HIGH_BYTE(DESCRIPTOR_DEVICE_ID_PRODUCT),// idProduct, Product ID
	0x01, 0x00,																		// bcdDevice = 0x0100, Release Number Device
	MANUFACTURER_INDEX,																// iManufacturer = Index for String-Descriptor Manufacturer
	PRODUCT_INDEX,																	// iProduct = Index for String-Descriptor Product
	SERIAL_NUMBER_INDEX,															// iSerialNumber = Index for String-Descriptor Serial Number
	0x01																			// bNumConfigurations = 1, Number Configurations
};
	
// Configurations Descriptor
static const uint8_t PROGMEM configurationDescriptor[] =
{
	9,																	// bLength = 0x09, Size of descriptor in Byte
	DESCRIPTOR_TYPE_CONFIGURATION,										// bDescriptorType = 0x02, Configuration Constant = 2
	LOW_BYTE(W_TOTAL_LENGTH), HIGH_BYTE(W_TOTAL_LENGTH),				// wTotalLength, Length of Configuration Descriptor + Interface and Endpoint descriptors.
	0x01,																// bNumInterfaces = 1
	0x01,																// bConfigurationValue = 1, Number of this configuration (not allowed to be 0)
	0,																	// iConfiguration = 0, Index for String-Descriptor of the Configuration -> 0 = no String-Descriptor available
	0x80,																// bmAttributes = 0x80, bus-powered, no remote wakeup -> Bit 7 = 1
	250,																// MaxPower = 250 (decimal), in 2mA Steps -> 500mA

	// Interface Descriptor
	9,																	// bLength = 0x09, Size of Descriptor in Byte
	DESCRIPTOR_TYPE_INTERFACE,											// bDescriptorType = 0x04, Interface Constant = 4
	0,																	// bInterfaceNumber = 0;
	0,																	// bAlternateSetting = 0;
	NUMBER_ENDPOINTS,													// bNumEndpoints = USB_Endpoints;
	DESCRIPTOR_INTERFACE_INTERFACE_CLASS,								// bInterfaceClass = 0xFF, Class-code -> here: 0xFF
	DESCRIPTOR_INTERFACE_INTERFACE_SUB_CLASS,							// bInterfaceSubClass = 0xFF, Subclass-code -> here: 0xFF
	DESCRIPTOR_INTERFACE_INTERFACE_PROTOCOL,							// bInterfaceProtocol = 0xFF, Protocol-code -> here: 0xFF
	INTERFACE_INDEX,													// iInterface = 0, Index for String-Descriptor of the Interface

	// Endpoint 1 Descriptor
	7,																	// bLength = 0x07, Size of Descriptor in Byte
	DESCRIPTOR_TYPE_ENDPOINT,											// bDescriptorType = 0x05, Endpoint Constant = 5
	(ENDPOINT_1_DIRECTION << 7) | (ENDPOINT_1),							// bEndpointAddress = 0x81, Bit 7 = 1 (IN) Bit 0-3 Endpoint number
	ENDPOINT_1_TYPE,													// bmAttributes = 0x02, Type = Bulk
	LOW_BYTE(ENDPOINT_1_FIFO_SIZE), HIGH_BYTE(ENDPOINT_1_FIFO_SIZE),	// wMaxPacketSize = Endpoint 1 FIFO Size;
	0,																	// bInterval = 0, Bulk type -> no polling

	// Endpoint 2 Descriptor
	7,																	// bLength = 0x07, Size of Descriptor in Byte
	DESCRIPTOR_TYPE_ENDPOINT,											// bDescriptorType = 0x05, Endpoint Constant = 5
	(ENDPOINT_2_DIRECTION << 7) | (ENDPOINT_2),							// bEndpointAddress = 0x81, Bit 7 = 0 (OUT) Bit 0-3 Endpoint number
	ENDPOINT_2_TYPE,													// bmAttributes = 0x02, Type = Bulk
	LOW_BYTE(ENDPOINT_2_FIFO_SIZE), HIGH_BYTE(ENDPOINT_2_FIFO_SIZE),	// wMaxPacketSize = Endpoint 1 FIFO Size;
	0																	// bInterval = 0, Bulk type -> no polling
		
	// Endpoint 3 Descriptor
//	7,																	// bLength = 0x07, Size of Descriptor in Byte
//	DESCRIPTOR_TYPE_ENDPOINT,											// bDescriptorType = 0x05, Endpoint Constant = 5
//	(ENDPOINT_3_DIRECTION << 7) | (ENDPOINT_3),							// bEndpointAddress = 0x81, Bit 7 = 0 (OUT) Bit 0-3 Endpoint number
//	ENDPOINT_3_TYPE,													// bmAttributes = 0x02, Type = Bulk
//	LOW_BYTE(ENDPOINT_3_FIFO_SIZE), HIGH_BYTE(ENDPOINT_3_FIFO_SIZE),	// wMaxPacketSize = Endpoint 1 FIFO Size;
//	2,																	// bInterval = 0
		
	// Endpoint 4 Descriptor
//	7,																	// bLength = 0x07, Size of Descriptor in Byte
//	DESCRIPTOR_TYPE_ENDPOINT,											// bDescriptorType = 0x05, Endpoint Constant = 5
//	(ENDPOINT_4_DIRECTION << 7) | (ENDPOINT_4),							// bEndpointAddress = 0x81, Bit 7 = 0 (OUT) Bit 0-3 Endpoint number
//	ENDPOINT_4_TYPE,													// bmAttributes = 0x02, Type = Bulk
//	LOW_BYTE(ENDPOINT_4_FIFO_SIZE), HIGH_BYTE(ENDPOINT_4_FIFO_SIZE),	// wMaxPacketSize = Endpoint 1 FIFO Size;
//	2																	// bInterval = 0
};
	
// Language Descriptor
static const uint8_t PROGMEM languageDescriptor[] =
{
	4,						// bLength = 0x04, Size of Descriptor in Byte
	DESCRIPTOR_TYPE_STRING,	// bDescriptorType = 0x03, String Constant = 3
	0x09, 0x04				// wLANGID[0] = 0x0409 = English USA (Supported Lang. Code 0)
};
	
// Manufacturer Descriptor
static const uint8_t PROGMEM manufacturerDescriptor[] =
{
	22,						// bLength = 18, Size of Descriptor in Byte
	DESCRIPTOR_TYPE_STRING,	// bDescriptorType = 0x03, String Constant = 3
	'C', 0,					// bString = Unicode Encoded String (2 Byte per Character -> Unicode 16 Bit encoding)
	'h', 0,
	'r', 0,
	'o', 0,
	's', 0,
	'L', 0,
	'i', 0,
	'g', 0,
	'h', 0,
	't', 0
};
	
// Product Descriptor
static const uint8_t PROGMEM productDescriptor[] =
{
	26,						// bLength = 16, Size of Descriptor in Byte
	DESCRIPTOR_TYPE_STRING,	// bDescriptorType = 0x03, String Constant = 3
	'M', 0,					// bString = Unicode Encoded String (2 Byte per Character -> Unicode 16 Bit encoding)
	'I', 0,
	'D', 0,
	'I', 0,
	' ', 0,
	'L', 0,
	'i', 0,
	'g', 0,
	'h', 0,
	't', 0,
	'e', 0,
	'r', 0
};
	
// Serial Number Descriptor
static const uint8_t PROGMEM serialNumberDescriptor[] =
{
	12,						// bLength = 12, Size of Descriptor in Byte
	DESCRIPTOR_TYPE_STRING,	// bDescriptorType = 0x03, String Constant = 3
	'v', 0,					// bString = Unicode Encoded String (2 Byte per Character -> Unicode 16 Bit encoding)
	'0', 0,
	'.', 0,
	'0', 0,
	'1', 0
};



#endif /* USB_DESCRIPTOR_H_ */