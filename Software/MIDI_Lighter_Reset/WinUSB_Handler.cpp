#include "WinUSB_Handler.h"

////////////
// Public //
////////////
HAL::WinUSB_Handler::WinUSB_Handler(std::string guid)
{
	_Device_GUID_String = guid;

	_Device_Handle = NULL;
	_WinUSB_Handle = NULL;
	_Device_Open = false;

	_Number_Of_Endpoints = 0;
	_Endpoints = NULL;
}


HAL::WinUSB_Handler::~WinUSB_Handler()
{
	Close_Device();
}


bool HAL::WinUSB_Handler::Open_Device(unsigned int device_index)
{
	// 0. Create GUID
	std::wstring _Device_GUID_WString = std::wstring(_Device_GUID_String.begin(), _Device_GUID_String.end());
	const wchar_t* _Device_GUID_WChar = _Device_GUID_WString.c_str();

	GUID Device_GUID;
	CLSIDFromString(_Device_GUID_WChar, &Device_GUID);
	LPGUID GUID_Pointer = &Device_GUID;

	// 1. Get Device Path	
	HDEVINFO Device_Info = SetupDiGetClassDevs(GUID_Pointer, NULL, NULL, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
	
	SP_DEVICE_INTERFACE_DATA Interface_Data;
	Interface_Data.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

	//if (SetupDiEnumDeviceInterfaces(Device_Info, NULL, GUID_Pointer, 0, &Interface_Data) == false)
	if (SetupDiEnumDeviceInterfaces(Device_Info, NULL, GUID_Pointer, device_index, &Interface_Data) == false)
	{
		SetupDiDestroyDeviceInfoList(Device_Info);
		return false;
	}

	PSP_DEVICE_INTERFACE_DETAIL_DATA Interface_Detail_Data = NULL;
	ULONG Required_Length;
	if (SetupDiGetDeviceInterfaceDetail(Device_Info, &Interface_Data, NULL, 0, &Required_Length, NULL) == false)
	{
		if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
		{
			SetupDiDestroyDeviceInfoList(Device_Info);
			return false;
		}
	}

	Interface_Detail_Data = (PSP_DEVICE_INTERFACE_DETAIL_DATA)LocalAlloc(LMEM_FIXED, Required_Length);
	if (Interface_Detail_Data == NULL)
	{
		SetupDiDestroyDeviceInfoList(Device_Info);
		LocalFree(Interface_Detail_Data);
		return false;
	}

	Interface_Detail_Data->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
	ULONG Interface_Detail_Data_Length = Required_Length;

	if (SetupDiGetDeviceInterfaceDetail(Device_Info, &Interface_Data, Interface_Detail_Data, Interface_Detail_Data_Length, NULL, NULL) == false)
	{
		SetupDiDestroyDeviceInfoList(Device_Info);
		LocalFree(Interface_Detail_Data);
		return false;
	}

	#ifdef _UNICODE
		size_t Device_Path_Length = wcslen(Interface_Detail_Data->DevicePath);
		PWCHAR Device_Path = (PWCHAR)malloc((Device_Path_Length + 1) * sizeof(WCHAR));
	#else
		size_t Device_Path_Length = strlen(Interface_Detail_Data->DevicePath);
		PCHAR Device_Path = (PCHAR)malloc((Device_Path_Length + 1) * sizeof(CHAR));
	#endif

	if (FAILED(StringCchCopy(Device_Path, Device_Path_Length + 1, Interface_Detail_Data->DevicePath)))
	{
		SetupDiDestroyDeviceInfoList(Device_Info);
		LocalFree(Interface_Detail_Data);
		return false;
	}
	SetupDiDestroyDeviceInfoList(Device_Info);
	LocalFree(Interface_Detail_Data);


	// 2. Get WINusbHandle
	_Device_Handle = CreateFile(Device_Path, GENERIC_WRITE | GENERIC_READ, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
	BOOL Init_Result = WinUsb_Initialize(_Device_Handle, &_WinUSB_Handle);
	free(Device_Path);

	if (Init_Result == 0)
	{
		return false;
	}

	// 3. Create List of Endpoint
	if (Create_Endpoint_List())
	{
		_Device_Open = true;
		return true;
	}
	else
	{
		return false;
	}
}


void HAL::WinUSB_Handler::Close_Device()
{
	if (_Device_Open)
	{
		if (_Endpoints != NULL)
		{
			delete [] _Endpoints;
			_Endpoints = NULL;
		}

		CloseHandle(_Device_Handle);
		WinUsb_Free(_WinUSB_Handle);

		_Device_Open = false;
	}
}


bool HAL::WinUSB_Handler::Open()
{
	return _Device_Open;
}

bool HAL::WinUSB_Handler::Closed()
{
	return !_Device_Open;
}


bool HAL::WinUSB_Handler::Control_Transfer(USB_Device::Setup_Packet setup_packet, uint8_t* data)
{
	if (_Device_Open)
	{
		if (setup_packet.Direction == HAL::USB_Device::Transfer_Direction::Both)
		{
			return false;
		}

		WINUSB_SETUP_PACKET WinUSB_Setup_Packet;


		unsigned int Recipient_Uint;
		unsigned int Type_Uint;
		unsigned int Transfer_Direction_Uint;

		switch (setup_packet.Recipient)
		{
			case HAL::USB_Device::Setup_Packet::Request_Recipient::Device:		Recipient_Uint = 0; break;
			case HAL::USB_Device::Setup_Packet::Request_Recipient::Interface:	Recipient_Uint = 1; break;
			case HAL::USB_Device::Setup_Packet::Request_Recipient::Endpoint:	Recipient_Uint = 2; break;
			case HAL::USB_Device::Setup_Packet::Request_Recipient::Other:		Recipient_Uint = 3; break;
		}

		switch (setup_packet.Type)
		{
			case HAL::USB_Device::Setup_Packet::Request_Type::Standard:		Type_Uint = 0; break;
			case HAL::USB_Device::Setup_Packet::Request_Type::Class:		Type_Uint = 1; break;
			case HAL::USB_Device::Setup_Packet::Request_Type::Vendor:		Type_Uint = 2; break;
		}

		switch (setup_packet.Direction)
		{
			case HAL::USB_Device::Transfer_Direction::Host_to_Device:		Transfer_Direction_Uint = 0; break;
			case HAL::USB_Device::Transfer_Direction::Device_to_Host:		Transfer_Direction_Uint = 1; break;
		}

		WinUSB_Setup_Packet.RequestType = ((Transfer_Direction_Uint & 0x01) << 7) | ((Type_Uint & 0x03) << 5) | ((Recipient_Uint & 0x1F) << 0);
		WinUSB_Setup_Packet.Request = setup_packet.Request;
		WinUSB_Setup_Packet.Value = setup_packet.Value;
		WinUSB_Setup_Packet.Index = setup_packet.Index;
		WinUSB_Setup_Packet.Length = setup_packet.Length;

		ULONG Bytes_Transferred_ULONG;

		BOOL Result = WinUsb_ControlTransfer(_WinUSB_Handle, WinUSB_Setup_Packet, (PUCHAR)data, setup_packet.Length, &Bytes_Transferred_ULONG, NULL);

		if ((Result == 1) && (setup_packet.Length == (unsigned int)Bytes_Transferred_ULONG))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}


bool HAL::WinUSB_Handler::Receive(USB_Device::Endpoint endpoint, uint8_t* data_buffer, unsigned int max_size, unsigned int* bytes_read)
{
	if (_Device_Open)
	{
		ULONG Bytes_Read_ULONG;
		BOOL Result = WinUsb_ReadPipe(_WinUSB_Handle, (char)endpoint.Pipe_ID, (PUCHAR)data_buffer, max_size, &Bytes_Read_ULONG, NULL);

		bytes_read[0] = (int)Bytes_Read_ULONG;
		if (Result == 1)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}


bool HAL::WinUSB_Handler::Send(USB_Device::Endpoint endpoint, uint8_t* data, unsigned int size)
{
	if (_Device_Open)
	{
		ULONG Bytes_Written;
		BOOL Result = WinUsb_WritePipe(_WinUSB_Handle, (char)endpoint.Pipe_ID, (PUCHAR)data, size, &Bytes_Written, NULL);
		if ((Result == 1) && (Bytes_Written == size))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}


void HAL::WinUSB_Handler::Clear(USB_Device::Endpoint endpoint)
{
	if (_Device_Open)
	{
		WinUsb_FlushPipe(_WinUSB_Handle, (char)endpoint.Pipe_ID);
	}
}


unsigned int HAL::WinUSB_Handler::Number_Of_Endpoints()
{
	if(_Device_Open)
	{
		return _Number_Of_Endpoints;
	}
	else
	{
		return 0;
	}
}


unsigned int HAL::WinUSB_Handler::Get_Endpoints(USB_Device::Endpoint* endpoints_buffer)
{
	if (_Device_Open)
	{
		for (unsigned int i = 0; i < _Number_Of_Endpoints; i++)
		{
			endpoints_buffer[i] = _Endpoints[i];
		}
	}

	return _Number_Of_Endpoints;
}



/////////////
// Private //
/////////////
bool HAL::WinUSB_Handler::Create_Endpoint_List()
{
	USB_INTERFACE_DESCRIPTOR USB_Interface_Descriptor;
	UCHAR Speed;
	ULONG Length = sizeof(UCHAR);

	if (WinUsb_QueryDeviceInformation(_WinUSB_Handle, DEVICE_SPEED, &Length, &Speed) == false)
	{
		return false;
	}

	if (WinUsb_QueryInterfaceSettings(_WinUSB_Handle, 0, &USB_Interface_Descriptor) == false)
	{
		return false;
	}

	_Number_Of_Endpoints = USB_Interface_Descriptor.bNumEndpoints + 1;
	_Endpoints = new USB_Device::Endpoint[_Number_Of_Endpoints];

	_Endpoints[0].Type = USB_Device::Pipe_Type::Control;
	_Endpoints[0].Direction = USB_Device::Transfer_Direction::Both;
	_Endpoints[0].Pipe_ID = 0;
	_Endpoints[0].Valid = true;


	WINUSB_PIPE_INFORMATION WinUSB_Pipe_Information;

	for (int i = 1; i < USB_Interface_Descriptor.bNumEndpoints + 1; i++)
	{
		if (WinUsb_QueryPipe(_WinUSB_Handle, 0, (UCHAR)i - 1, &WinUSB_Pipe_Information))
		{
			// Type
			if (WinUSB_Pipe_Information.PipeType == UsbdPipeTypeBulk)
			{
				_Endpoints[i].Type = USB_Device::Pipe_Type::Bulk;
			}
			else if (WinUSB_Pipe_Information.PipeType == UsbdPipeTypeControl)
			{
				_Endpoints[i].Type = USB_Device::Pipe_Type::Control;
			}
			else if (WinUSB_Pipe_Information.PipeType == UsbdPipeTypeInterrupt)
			{
				_Endpoints[i].Type = USB_Device::Pipe_Type::Interrupt;
			}
			else if (WinUSB_Pipe_Information.PipeType == UsbdPipeTypeIsochronous)
			{
				_Endpoints[i].Type = USB_Device::Pipe_Type::Isochronous;
			}

			// Direction
			if (USB_ENDPOINT_DIRECTION_IN(WinUSB_Pipe_Information.PipeId))
			{
				_Endpoints[i].Direction = USB_Device::Transfer_Direction::Device_to_Host;
			}
			else if (USB_ENDPOINT_DIRECTION_OUT(WinUSB_Pipe_Information.PipeId))
			{
				_Endpoints[i].Direction = USB_Device::Transfer_Direction::Host_to_Device;
			}

			_Endpoints[i].Pipe_ID = (uint8_t)WinUSB_Pipe_Information.PipeId;
			_Endpoints[i].Valid = true;
		}
		else
		{
			delete [] _Endpoints;
			_Endpoints = NULL;

			return false;
		}
	}
	
	return true;
}
