#ifndef USB_DEVICE_H_
#define USB_DEVICE_H_

namespace HAL
{
	namespace USB_Device
	{
		enum class Transfer_Direction
		{
			Host_to_Device	= 0,
			Device_to_Host	= 1,
			Both			= 2
		};
		
		enum class Pipe_Type
		{
			Bulk		= 0,
			Control		= 1,
			Interrupt	= 2,
			Isochronous = 3
		};

		struct Endpoint
		{
			HAL::USB_Device::Pipe_Type Type;
			HAL::USB_Device::Transfer_Direction Direction;
			uint8_t Pipe_ID;
			bool Valid;
		};


		struct Setup_Packet
		{
			enum class Request_Type
			{
				Standard	= 0,
				Class		= 1,
				Vendor		= 2
			};

			enum class Request_Recipient
			{
				Device		= 0,
				Interface	= 1,
				Endpoint	= 2,
				Other		= 3
			};

			HAL::USB_Device::Transfer_Direction Direction;
			HAL::USB_Device::Setup_Packet::Request_Type Type;
			HAL::USB_Device::Setup_Packet::Request_Recipient Recipient;
			unsigned int Request;
			unsigned int Value;
			unsigned int Index;
			unsigned int Length;
		};
	}
}
	
#endif