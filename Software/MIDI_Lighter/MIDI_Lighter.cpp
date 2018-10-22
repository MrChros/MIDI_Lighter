#include "MIDI_Lighter.h"


HAL::MIDI_Lighter::MIDI_Lighter()
{

}

HAL::MIDI_Lighter::~MIDI_Lighter() { }

uint32_t HAL::MIDI_Lighter::Number_Of_Devices()
{
	return _USB_Link.Number_of_Devices(_GUID);
}

std::vector<HAL::MIDI_Lighter::Device> HAL::MIDI_Lighter::Get_Device_List()
{
	std::vector<HAL::MIDI_Lighter::Device> Device_List;

	for (unsigned int Current_Index = 0; Current_Index < 256; Current_Index++)
	{
		if (this->Connect(Current_Index))
		{
			bool Get_Name_Success;
			std::string Current_Name = this->Get_Device_Name(&Get_Name_Success);

			if (Get_Name_Success == true)
			{
				HAL::MIDI_Lighter::Device Device;
				Device.Index	= Current_Index;
				Device.Name		= Current_Name;
				Device_List.push_back(Device);
			}

			this->Disconnect();
		}
	}

	return Device_List;
}

bool HAL::MIDI_Lighter::Connect(unsigned int index)
{
	if(_USB_Link.Open())
	{
		return true;
	}
	
	if (_USB_Link.Open_Device(_GUID, 2048, index) == false)
	{
		return false;
	}

	_Endpoint_TX = _USB_Link.Find_Endpoint(HAL::USB_Device::Transfer_Direction::Host_to_Device, HAL::USB_Device::Pipe_Type::Bulk);
	_Endpoint_RX = _USB_Link.Find_Endpoint(HAL::USB_Device::Transfer_Direction::Device_to_Host, HAL::USB_Device::Pipe_Type::Bulk);

	if (!_Endpoint_TX.Valid || !_Endpoint_RX.Valid)
	{
		return false;
	}

	return true;
}

bool HAL::MIDI_Lighter::IsConnected()
{
	return _USB_Link.Open();
}

void HAL::MIDI_Lighter::Disconnect()
{
	if (_USB_Link.Open() == true)
	{
		_USB_Link.Close_Device();
	}
}

std::string HAL::MIDI_Lighter::Get_Device_Name(bool* read_success)
{
	if (_USB_Link.Open() == false)
	{
		if (read_success != NULL)
		{
			read_success[0] = false;
		}
		return "";
	}

	_USB_Link.Buffer_Clear(_Endpoint_RX);

	// Name
	uint8_t Data[3];

	Data[0] = (uint8_t)HAL::MIDI_Lighter::Command::Terminator;
	Data[1] = (uint8_t)HAL::MIDI_Lighter::Command::Get_Device_Name;
	Data[2] = (uint8_t)HAL::MIDI_Lighter::Command::Terminator;

	bool Write_Success = _USB_Link.Buffer_Write(_Endpoint_TX, Data, 3);

	if (!Write_Success)
	{
		if (read_success != NULL)
		{
			read_success[0] = false;
		}
		return "";
	}

	// Get answer
	std::string Device_Name;

	// Max. 31 symbols + Terminator
	for (unsigned int i = 0; i < 32; i++)
	{
		uint8_t Answer;

		if (!_USB_Link.Buffer_Read(_Endpoint_RX, &Answer, 1))
		{
			if (read_success != NULL)
			{
				read_success[0] = false;
			}
			return "";
		}

		if (Answer != (uint8_t)HAL::MIDI_Lighter::Command::Terminator)
		{
			if (i < 32)
			{
				Device_Name.append((char*)&Answer, 1);
			}
		}
		else
		{
			if (read_success != NULL)
			{
				read_success[0] = true;
			}
			return Device_Name;
		}
	}

	if (read_success != NULL)
	{
		read_success[0] = false;
	}

	return "";
}

bool HAL::MIDI_Lighter::Set_Device_Name(std::string name)
{
	if (_USB_Link.Open() == false)
	{
		return false;
	}

	unsigned int Actual_Size = HAL::Common::Min((unsigned int)name.size(), 32);

	uint8_t* Data = new uint8_t[1 + 1 + Actual_Size + 1];

	Data[0] = (uint8_t)HAL::MIDI_Lighter::Command::Terminator;
	Data[1] = (uint8_t)HAL::MIDI_Lighter::Command::Set_Device_Name;
	for (unsigned int i = 0; i< Actual_Size; i++)
	{
		Data[2 + i] = (uint8_t)(name.c_str()[i]);
	}
	Data[2 + Actual_Size] = (uint8_t)HAL::MIDI_Lighter::Command::Terminator;

	bool Success = _USB_Link.Buffer_Write(_Endpoint_TX, Data, 1 + 1 + Actual_Size + 1);

	delete[] Data;

	return Success;
}

HAL::MIDI_Lighter::Configuration_MIDI HAL::MIDI_Lighter::Get_Configuration_MIDI(bool* read_success)
{
	if (_USB_Link.Open() == false)
	{
		if (read_success != NULL)
		{
			read_success[0] = false;
		}
		return HAL::MIDI_Lighter::Configuration_MIDI();
	}

	_USB_Link.Buffer_Clear(_Endpoint_RX);

	uint8_t Data[3];

	Data[0] = (uint8_t)HAL::MIDI_Lighter::Command::Terminator;
	Data[1] = (uint8_t)HAL::MIDI_Lighter::Command::Get_MIDI;
	Data[2] = (uint8_t)HAL::MIDI_Lighter::Command::Terminator;

	bool Write_Success = _USB_Link.Buffer_Write(_Endpoint_TX, Data, 3);

	if (!Write_Success)
	{
		if (read_success != NULL)
		{
			read_success[0] = false;
		}
		return HAL::MIDI_Lighter::Configuration_MIDI();
	}

	// wait for answer
	uint8_t Answer[11];

	bool Read_Success = _USB_Link.Buffer_Read(_Endpoint_RX, Answer, 11);

	if (!Read_Success)
	{
		// timeout
		if (read_success != NULL)
		{
			read_success[0] = false;
		}
		return HAL::MIDI_Lighter::Configuration_MIDI();
	}

	if (Answer[10] != (uint8_t)HAL::MIDI_Lighter::Command::Terminator)
	{
		// something is wrong
		if (read_success != NULL)
		{
			read_success[0] = false;
		}
		return HAL::MIDI_Lighter::Configuration_MIDI();
	}

	HAL::MIDI_Lighter::Configuration_MIDI Current_Configuration_MIDI;

	Current_Configuration_MIDI.Channel										= (HAL::MIDI_Lighter::MIDI_Channel) (HAL::Common::Int_From_Ascii_Hex((char*)&Answer[0], 1) % 16);
	Current_Configuration_MIDI.Note[(int)HAL::MIDI_Lighter::Color::Red]		= (HAL::MIDI_Lighter::MIDI_Note)	(HAL::Common::Int_From_Ascii_Hex((char*)&Answer[1], 2) % 120);
	Current_Configuration_MIDI.Note[(int)HAL::MIDI_Lighter::Color::Green]	= (HAL::MIDI_Lighter::MIDI_Note)	(HAL::Common::Int_From_Ascii_Hex((char*)&Answer[3], 2) % 120);
	Current_Configuration_MIDI.Note[(int)HAL::MIDI_Lighter::Color::Blue]	= (HAL::MIDI_Lighter::MIDI_Note)	(HAL::Common::Int_From_Ascii_Hex((char*)&Answer[5], 2) % 120);
	Current_Configuration_MIDI.Factor										= (uint8_t)							(HAL::Common::Int_From_Ascii_Hex((char*)&Answer[7], 2));
	Current_Configuration_MIDI.Skip_Note_Off								= !(&Answer[9] == 0);

	if (read_success != NULL)
	{
		read_success[0] = true;
	}
	return Current_Configuration_MIDI;
}

bool HAL::MIDI_Lighter::Set_Configuration_MIDI(HAL::MIDI_Lighter::Configuration_MIDI midi)
{
	if (_USB_Link.Open() == false)
	{
		return false;
	}

	uint8_t Data[1 + 1 + 10 + 1];

	Data[0] = (uint8_t)HAL::MIDI_Lighter::Command::Terminator;
	Data[1] = (uint8_t)HAL::MIDI_Lighter::Command::Set_MIDI;
	Data[2] = HAL::Common::Ascii( (uint8_t)midi.Channel);
	Data[3] = HAL::Common::Ascii(((uint8_t)midi.Note[(int)HAL::MIDI_Lighter::Color::Red]	& 0xF0) >> 4);
	Data[4] = HAL::Common::Ascii( (uint8_t)midi.Note[(int)HAL::MIDI_Lighter::Color::Red]	& 0x0F);
	Data[5] = HAL::Common::Ascii(((uint8_t)midi.Note[(int)HAL::MIDI_Lighter::Color::Green]	& 0xF0) >> 4);
	Data[6] = HAL::Common::Ascii( (uint8_t)midi.Note[(int)HAL::MIDI_Lighter::Color::Green]	& 0x0F);
	Data[7] = HAL::Common::Ascii(((uint8_t)midi.Note[(int)HAL::MIDI_Lighter::Color::Blue]	& 0xF0) >> 4);
	Data[8] = HAL::Common::Ascii( (uint8_t)midi.Note[(int)HAL::MIDI_Lighter::Color::Blue]	& 0x0F);
	Data[9]	 = HAL::Common::Ascii(((uint8_t)midi.Factor	& 0xF0) >> 4);
	Data[10] = HAL::Common::Ascii( (uint8_t)midi.Factor	& 0x0F);
	Data[11] = HAL::Common::Ascii( (uint8_t)midi.Skip_Note_Off);
	Data[12] = (uint8_t)HAL::MIDI_Lighter::Command::Terminator;

	return _USB_Link.Buffer_Write(_Endpoint_TX, Data, 1 + 1 + 10 + 1);
}

HAL::MIDI_Lighter::Configuration_No_Data_Light HAL::MIDI_Lighter::Get_Configuration_No_Data_Light(bool* read_success)
{
	if (_USB_Link.Open() == false) { if (read_success != NULL) { read_success[0] = false; } return HAL::MIDI_Lighter::Configuration_No_Data_Light(); }

	_USB_Link.Buffer_Clear(_Endpoint_RX);

	uint8_t Data[3];

	Data[0] = (uint8_t)HAL::MIDI_Lighter::Command::Terminator;
	Data[1] = (uint8_t)HAL::MIDI_Lighter::Command::Get_No_Data_Light;
	Data[2] = (uint8_t)HAL::MIDI_Lighter::Command::Terminator;

	bool Write_Success = _USB_Link.Buffer_Write(_Endpoint_TX, Data, 3);

	if (!Write_Success) { if (read_success != NULL) { read_success[0] = false; } return HAL::MIDI_Lighter::Configuration_No_Data_Light(); }

	// wait for answer
	uint8_t Answer[9];

	bool Read_Success = _USB_Link.Buffer_Read(_Endpoint_RX, Answer, 10);

	// timeout
	if (!Read_Success) { if (read_success != NULL) { read_success[0] = false; } return HAL::MIDI_Lighter::Configuration_No_Data_Light(); }

	if (Answer[9] != (uint8_t)HAL::MIDI_Lighter::Command::Terminator)
	{
		// something is wrong
		if (read_success != NULL) { read_success[0] = false; }
		return HAL::MIDI_Lighter::Configuration_No_Data_Light();
	}

	HAL::MIDI_Lighter::Configuration_No_Data_Light Current_Configuration_No_Data_Light;

	Current_Configuration_No_Data_Light.Color[(int)HAL::MIDI_Lighter::Color::Red]	= (HAL::Common::Int_From_Ascii_Hex((char*)&Answer[0], 2));
	Current_Configuration_No_Data_Light.Color[(int)HAL::MIDI_Lighter::Color::Green] = (HAL::Common::Int_From_Ascii_Hex((char*)&Answer[2], 2));
	Current_Configuration_No_Data_Light.Color[(int)HAL::MIDI_Lighter::Color::Blue]	= (HAL::Common::Int_From_Ascii_Hex((char*)&Answer[4], 2));
	Current_Configuration_No_Data_Light.Timeout		= (HAL::Common::Int_From_Ascii_Hex((char*)&Answer[6], 2) % 120);
	Current_Configuration_No_Data_Light.Deactivate	= (HAL::MIDI_Lighter::No_Data_Light_Deactivate)(HAL::Common::Int_From_Ascii_Hex((char*)&Answer[8], 1));

	if (read_success != NULL) { read_success[0] = true; }

	return Current_Configuration_No_Data_Light;
}

bool HAL::MIDI_Lighter::Set_Configuration_No_Data_Light(HAL::MIDI_Lighter::Configuration_No_Data_Light no_data_light)
{
	if (_USB_Link.Open() == false)
	{
		return false;
	}

	uint8_t Data[1 + 1 + 9 + 1];

	Data[0] = (uint8_t)HAL::MIDI_Lighter::Command::Terminator;
	Data[1] = (uint8_t)HAL::MIDI_Lighter::Command::Set_No_Data_Light;
	Data[2] = HAL::Common::Ascii(((uint8_t)no_data_light.Color[(int)HAL::MIDI_Lighter::Color::Red]		& 0xF0) >> 4);
	Data[3] = HAL::Common::Ascii( (uint8_t)no_data_light.Color[(int)HAL::MIDI_Lighter::Color::Red]		& 0x0F);
	Data[4] = HAL::Common::Ascii(((uint8_t)no_data_light.Color[(int)HAL::MIDI_Lighter::Color::Green]	& 0xF0) >> 4);
	Data[5] = HAL::Common::Ascii( (uint8_t)no_data_light.Color[(int)HAL::MIDI_Lighter::Color::Green]	& 0x0F);
	Data[6] = HAL::Common::Ascii(((uint8_t)no_data_light.Color[(int)HAL::MIDI_Lighter::Color::Blue]		& 0xF0) >> 4);
	Data[7] = HAL::Common::Ascii( (uint8_t)no_data_light.Color[(int)HAL::MIDI_Lighter::Color::Blue]		& 0x0F);
	Data[8] = HAL::Common::Ascii(((uint8_t)no_data_light.Timeout & 0xF0) >> 4);
	Data[9] = HAL::Common::Ascii( (uint8_t)no_data_light.Timeout & 0x0F);
	Data[10] = HAL::Common::Ascii((uint8_t)no_data_light.Deactivate & 0x0F);
	Data[11] = (uint8_t)HAL::MIDI_Lighter::Command::Terminator;

	return _USB_Link.Buffer_Write(_Endpoint_TX, Data, 1 + 1 + 9 + 1);
}

HAL::MIDI_Lighter::Configuration_Permanent_Light HAL::MIDI_Lighter::Get_Configuration_Permanent_Light(bool* read_success)
{
	if (_USB_Link.Open() == false)
	{
		if (read_success != NULL)
		{
			read_success[0] = false;
		}
		return HAL::MIDI_Lighter::Configuration_Permanent_Light();
	}

	_USB_Link.Buffer_Clear(_Endpoint_RX);

	uint8_t Data[3];

	Data[0] = (uint8_t)HAL::MIDI_Lighter::Command::Terminator;
	Data[1] = (uint8_t)HAL::MIDI_Lighter::Command::Get_Permanent_Light;
	Data[2] = (uint8_t)HAL::MIDI_Lighter::Command::Terminator;

	bool Write_Success = _USB_Link.Buffer_Write(_Endpoint_TX, Data, 3);

	if (!Write_Success)
	{
		if (read_success != NULL)
		{
			read_success[0] = false;
		}
		return HAL::MIDI_Lighter::Configuration_Permanent_Light();
	}

	// wait for answer
	uint8_t Answer[7];

	bool Read_Success = _USB_Link.Buffer_Read(_Endpoint_RX, Answer, 7);

	if (!Read_Success)
	{
		// timeout
		if (read_success != NULL)
		{
			read_success[0] = false;
		}
		return HAL::MIDI_Lighter::Configuration_Permanent_Light();
	}

	if (Answer[6] != (uint8_t)HAL::MIDI_Lighter::Command::Terminator)
	{
		// something is wrong
		if (read_success != NULL)
		{
			read_success[0] = false;
		}
		return HAL::MIDI_Lighter::Configuration_Permanent_Light();
	}

	HAL::MIDI_Lighter::Configuration_Permanent_Light Current_Configuration_Permanent_Light;

	Current_Configuration_Permanent_Light.Color[(int)HAL::MIDI_Lighter::Color::Red]		= (HAL::Common::Int_From_Ascii_Hex((char*)&Answer[0], 2));
	Current_Configuration_Permanent_Light.Color[(int)HAL::MIDI_Lighter::Color::Green]	= (HAL::Common::Int_From_Ascii_Hex((char*)&Answer[2], 2));
	Current_Configuration_Permanent_Light.Color[(int)HAL::MIDI_Lighter::Color::Blue]	= (HAL::Common::Int_From_Ascii_Hex((char*)&Answer[4], 2));

	if (read_success != NULL)
	{
		read_success[0] = true;
	}
	return Current_Configuration_Permanent_Light;
}

bool HAL::MIDI_Lighter::Set_Configuration_Permanent_Light(HAL::MIDI_Lighter::Configuration_Permanent_Light permanent_light)
{
	if (_USB_Link.Open() == false)
	{
		return false;
	}

	uint8_t Data[1 + 1 + 6 + 1];

	Data[0] = (uint8_t)HAL::MIDI_Lighter::Command::Terminator;
	Data[1] = (uint8_t)HAL::MIDI_Lighter::Command::Set_Permanent_Light;
	Data[2] = HAL::Common::Ascii(((uint8_t)permanent_light.Color[(int)HAL::MIDI_Lighter::Color::Red]	& 0xF0) >> 4);
	Data[3] = HAL::Common::Ascii( (uint8_t)permanent_light.Color[(int)HAL::MIDI_Lighter::Color::Red]	& 0x0F);
	Data[4] = HAL::Common::Ascii(((uint8_t)permanent_light.Color[(int)HAL::MIDI_Lighter::Color::Green]	& 0xF0) >> 4);
	Data[5] = HAL::Common::Ascii( (uint8_t)permanent_light.Color[(int)HAL::MIDI_Lighter::Color::Green]	& 0x0F);
	Data[6] = HAL::Common::Ascii(((uint8_t)permanent_light.Color[(int)HAL::MIDI_Lighter::Color::Blue]	& 0xF0) >> 4);
	Data[7] = HAL::Common::Ascii( (uint8_t)permanent_light.Color[(int)HAL::MIDI_Lighter::Color::Blue]	& 0x0F);
	Data[8] = (uint8_t)HAL::MIDI_Lighter::Command::Terminator;

	return _USB_Link.Buffer_Write(_Endpoint_TX, Data, 1 + 1 + 6 + 1);
}

HAL::MIDI_Lighter::Configuration_RGB_Order HAL::MIDI_Lighter::Get_Configuration_RGB_Order(bool* read_success)
{
	if (_USB_Link.Open() == false)
	{
		if (read_success != NULL)
		{
			read_success[0] = false;
		}
		return HAL::MIDI_Lighter::Configuration_RGB_Order();
	}

	_USB_Link.Buffer_Clear(_Endpoint_RX);

	uint8_t Data[3];

	Data[0] = (uint8_t)HAL::MIDI_Lighter::Command::Terminator;
	Data[1] = (uint8_t)HAL::MIDI_Lighter::Command::Get_RGB_Order;
	Data[2] = (uint8_t)HAL::MIDI_Lighter::Command::Terminator;

	bool Write_Success = _USB_Link.Buffer_Write(_Endpoint_TX, Data, 3);

	if (!Write_Success)
	{
		if (read_success != NULL)
		{
			read_success[0] = false;
		}
		return HAL::MIDI_Lighter::Configuration_RGB_Order();
	}

	// wait for answer
	uint8_t Answer[2];

	bool Read_Success = _USB_Link.Buffer_Read(_Endpoint_RX, Answer, 2);

	if (!Read_Success)
	{
		// timeout
		if (read_success != NULL)
		{
			read_success[0] = false;
		}
		return HAL::MIDI_Lighter::Configuration_RGB_Order();
	}

	if (Answer[1] != (uint8_t)HAL::MIDI_Lighter::Command::Terminator)
	{
		// something is wrong
		if (read_success != NULL)
		{
			read_success[0] = false;
		}
		return HAL::MIDI_Lighter::Configuration_RGB_Order();
	}

	HAL::MIDI_Lighter::Configuration_RGB_Order Current_Configuration_RGB_Order;

	Current_Configuration_RGB_Order.RGB_Order = (HAL::MIDI_Lighter::RGB_Order)(HAL::Common::Int_From_Ascii_Hex((char*)&Answer[0], 1));

	if (read_success != NULL)
	{
		read_success[0] = true;
	}
	return Current_Configuration_RGB_Order;
}

bool HAL::MIDI_Lighter::Set_Configuration_RGB_Order(HAL::MIDI_Lighter::Configuration_RGB_Order rgb_order)
{
	if (_USB_Link.Open() == false)
	{
		return false;
	}

	uint8_t Data[1 + 1 + 1 + 1];

	Data[0] = (uint8_t)HAL::MIDI_Lighter::Command::Terminator;
	Data[1] = (uint8_t)HAL::MIDI_Lighter::Command::Set_RGB_Order;
	Data[2] = HAL::Common::Ascii((uint8_t)rgb_order.RGB_Order);
	Data[3] = (uint8_t)HAL::MIDI_Lighter::Command::Terminator;

	return _USB_Link.Buffer_Write(_Endpoint_TX, Data, 1 + 1 + 1 + 1);
}

uint32_t HAL::MIDI_Lighter::Get_ADC(bool* read_success)
{
	if (_USB_Link.Open() == false) { if (read_success != NULL) { read_success[0] = false; } return 0; }

	_USB_Link.Buffer_Clear(_Endpoint_RX);
	uint8_t Data[8];

	Data[0] = (uint8_t)HAL::MIDI_Lighter::Command::Terminator;
	Data[1] = (uint8_t)HAL::MIDI_Lighter::Command::Get_ADC;
	Data[2] = (uint8_t)HAL::MIDI_Lighter::Command::Terminator;

	bool Write_Success = _USB_Link.Buffer_Write(_Endpoint_TX, Data, 3);

	if (!Write_Success) { if (read_success != NULL) { read_success[0] = false; } return 0; }

	// wait for answer
	uint8_t Answer[9];

	bool Read_Success = _USB_Link.Buffer_Read(_Endpoint_RX, Answer, 9);

	// timeout
	if (!Read_Success) { if (read_success != NULL) { read_success[0] = false; } return 0; }

	// Terminator not received on expected position
	if (Answer[8] != (uint8_t)HAL::MIDI_Lighter::Command::Terminator) { if (read_success != NULL) { read_success[0] = false; } return 0; }


	uint32_t Return_Value;
	Return_Value	= HAL::Common::Int_From_Ascii_Hex((char*)&Answer[0], 8);	// Sum

	if (read_success != NULL) { read_success[0] = true; }
	return Return_Value;
}

bool HAL::MIDI_Lighter::Set_Timer1_Top(uint16_t top)
{
	if (_USB_Link.Open() == false)
	{
		return false;
	}

	uint8_t Data[1 + 1 + 4 + 1];

	Data[0] = (uint8_t)HAL::MIDI_Lighter::Command::Terminator;
	Data[1] = (uint8_t)HAL::MIDI_Lighter::Command::Set_Timer1_Top;
	Data[2] = HAL::Common::Ascii((top & 0xF000) >> 12);
	Data[3] = HAL::Common::Ascii((top & 0x0F00) >>  8);
	Data[4] = HAL::Common::Ascii((top & 0x00F0) >>  4);
	Data[5] = HAL::Common::Ascii((top & 0x000F) >>  0);
	Data[6] = (uint8_t)HAL::MIDI_Lighter::Command::Terminator;

	return _USB_Link.Buffer_Write(_Endpoint_TX, Data, 1 + 1 + 4 + 1);
}

bool HAL::MIDI_Lighter::Set_Timer4_Prescaler(HAL::MIDI_Lighter::Timer4_Presacler prescaler)
{
	if (_USB_Link.Open() == false)
	{
		return false;
	}

	uint8_t Data[1 + 1 + 1 + 1];

	Data[0] = (uint8_t)HAL::MIDI_Lighter::Command::Terminator;
	Data[1] = (uint8_t)HAL::MIDI_Lighter::Command::Set_Timer4_Prescaler;
	Data[2] = HAL::Common::Ascii((uint8_t)prescaler);
	Data[3] = (uint8_t)HAL::MIDI_Lighter::Command::Terminator;

	return _USB_Link.Buffer_Write(_Endpoint_TX, Data, 1 + 1 + 1 + 1);
}

uint8_t	HAL::MIDI_Lighter::Read_EEPROM(uint32_t address, bool* read_success)
{
	if (_USB_Link.Open() == false) { if (read_success != NULL) { read_success[0] = false; } return 0; }

	_USB_Link.Buffer_Clear(_Endpoint_RX);

	uint8_t Data[6];

	Data[0] = (uint8_t)HAL::MIDI_Lighter::Command::Terminator;
	Data[1] = (uint8_t)HAL::MIDI_Lighter::Command::Read_EEPROM;
	Data[2] = HAL::Common::Ascii((address & 0x300) >> 8);
	Data[3] = HAL::Common::Ascii((address & 0x0F0) >> 4);
	Data[4] = HAL::Common::Ascii((address & 0x00F) >> 0);
	Data[5] = (uint8_t)HAL::MIDI_Lighter::Command::Terminator;

	bool Write_Success = _USB_Link.Buffer_Write(_Endpoint_TX, Data, 6);

	if (!Write_Success) { if (read_success != NULL) { read_success[0] = false; } return 0; }

	// wait for answer
	uint8_t Answer[3];

	bool Read_Success = _USB_Link.Buffer_Read(_Endpoint_RX, Answer, 3);

	// timeout
	if (!Read_Success) { if (read_success != NULL) { read_success[0] = false; } return 0; }

	// Terminator not received on expected position
	if (Answer[2] != (uint8_t)HAL::MIDI_Lighter::Command::Terminator) { if (read_success != NULL) { read_success[0] = false; } return 0; }

	
	uint8_t Return_Value = HAL::Common::Int_From_Ascii_Hex((char*)&Answer[0], 2);

	if (read_success != NULL) { read_success[0] = true; }
	return Return_Value;
}

bool HAL::MIDI_Lighter::Write_EEPROM()
{
	if (_USB_Link.Open() == false) { return false; }

	uint8_t Data[1 + 1 + 1];

	Data[0] = (uint8_t)HAL::MIDI_Lighter::Command::Terminator;
	Data[1] = (uint8_t)HAL::MIDI_Lighter::Command::Write_EEPROM;
	Data[2] = (uint8_t)HAL::MIDI_Lighter::Command::Terminator;

	return _USB_Link.Buffer_Write(_Endpoint_TX, Data, 1 + 1 + 1);
}