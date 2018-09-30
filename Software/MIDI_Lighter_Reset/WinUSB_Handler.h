#ifndef WINUSB_HANDLER_H_
#define WINUSB_HANDLER_H_

#include <string>
#include <stdint.h>
#include <stdio.h>

#include "USB_Device.h"

// Windows
#include <Windows.h>
#include <tchar.h>
#include <strsafe.h>

// WinUSB
#include "winusb.h"
#include "usb100.h"
#include "usb200.h"
#include "Setupapi.h"

namespace HAL
{
	class WinUSB_Handler
	{
		public:
			WinUSB_Handler(std::string guid);
			~WinUSB_Handler();

			bool Open_Device(unsigned int device_index);
			void Close_Device();

			bool Open();
			bool Closed();

			bool Control_Transfer(USB_Device::Setup_Packet setup_packet, uint8_t* data);
			bool Receive(USB_Device::Endpoint endpoint, uint8_t* data_buffer, unsigned int max_size, unsigned int* bytes_read);
			bool Send(USB_Device::Endpoint endpoint, uint8_t* data, unsigned int size);
			void Clear(USB_Device::Endpoint endpoint);

			unsigned int Number_Of_Endpoints();
			unsigned int Get_Endpoints(USB_Device::Endpoint* endpoints_buffer);

		private:
			std::string _Device_GUID_String;

			HANDLE _Device_Handle;
			WINUSB_INTERFACE_HANDLE _WinUSB_Handle;
			bool _Device_Open;

			unsigned int _Number_Of_Endpoints;
			USB_Device::Endpoint* _Endpoints;

			bool Create_Endpoint_List();
	};
}

#endif