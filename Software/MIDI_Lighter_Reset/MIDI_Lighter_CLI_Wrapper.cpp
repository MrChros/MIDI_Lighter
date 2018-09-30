#include <iostream>

#include "MIDI_Lighter_CLI_Wrapper.h"

MIDI_Lighter_Wrapper::MIDI_Lighter_Wrapper()
{
	_MIDI_Lighter = new HAL::MIDI_Lighter();
}

MIDI_Lighter_Wrapper::~MIDI_Lighter_Wrapper()
{
	delete(_MIDI_Lighter);	_MIDI_Lighter = NULL;
}

System::UInt32 MIDI_Lighter_Wrapper::Number_Of_Devices()
{
	return _MIDI_Lighter->Number_Of_Devices();
}

System::Collections::Generic::List<MIDI_Lighter_Wrapper::Device^ >^ MIDI_Lighter_Wrapper::Get_Device_List()
{
	std::vector<HAL::MIDI_Lighter::Device> Device_List = _MIDI_Lighter->Get_Device_List();

	System::Collections::Generic::List<MIDI_Lighter_Wrapper::Device^ >^ Managed_Device_List = gcnew System::Collections::Generic::List<MIDI_Lighter_Wrapper::Device^>;

	for (unsigned int a = 0; a < Device_List.size(); a++)
	{
		MIDI_Lighter_Wrapper::Device^ Device = gcnew MIDI_Lighter_Wrapper::Device;
		Device->Name	= gcnew System::String(Device_List.at(a).Name.c_str());
		Device->Index	= Device_List.at(a).Index;

		Managed_Device_List->Add(Device);
	}

	return Managed_Device_List;
}

System::Boolean MIDI_Lighter_Wrapper::Connect()
{
	return Connect(0);
}

System::Boolean MIDI_Lighter_Wrapper::Connect(System::UInt32 index)
{
	return _MIDI_Lighter->Connect(index);
}

System::Boolean MIDI_Lighter_Wrapper::IsConnected()
{
	return _MIDI_Lighter->IsConnected();
}

System::Void MIDI_Lighter_Wrapper::Disconnect()
{
	_MIDI_Lighter->Disconnect();
}

System::String^ MIDI_Lighter_Wrapper::Get_Device_Name(System::Boolean^ read_success)
{
	bool Success;

	std::string Device_Name = _MIDI_Lighter->Get_Device_Name(&Success);

	read_success = Success;

	return gcnew System::String(Device_Name.c_str());
}

System::Boolean	MIDI_Lighter_Wrapper::Set_Device_Name(System::String^ name)
{
	std::string New_Name;

	SystemString2StdString(name, New_Name);
	
	return _MIDI_Lighter->Set_Device_Name(New_Name);
}

MIDI_Lighter_Wrapper::Configuration_MIDI^ MIDI_Lighter_Wrapper::Get_Configuration_MIDI(System::Boolean^ read_success)
{
	bool Success;
	
	HAL::MIDI_Lighter::Configuration_MIDI Configuration_MIDI = _MIDI_Lighter->Get_Configuration_MIDI(&Success);

	read_success = Success;

	MIDI_Lighter_Wrapper::Configuration_MIDI^ Managed_Configuration_MIDI = gcnew MIDI_Lighter_Wrapper::Configuration_MIDI();

	Managed_Configuration_MIDI->Channel = (MIDI_Lighter_Wrapper::MIDI_Channel)Configuration_MIDI.Channel;
	Managed_Configuration_MIDI->Note[(int)MIDI_Lighter_Wrapper::Color::Red]		= (MIDI_Lighter_Wrapper::MIDI_Note)Configuration_MIDI.Note[(int)HAL::MIDI_Lighter::Color::Red];
	Managed_Configuration_MIDI->Note[(int)MIDI_Lighter_Wrapper::Color::Green]	= (MIDI_Lighter_Wrapper::MIDI_Note)Configuration_MIDI.Note[(int)HAL::MIDI_Lighter::Color::Green];
	Managed_Configuration_MIDI->Note[(int)MIDI_Lighter_Wrapper::Color::Blue]	= (MIDI_Lighter_Wrapper::MIDI_Note)Configuration_MIDI.Note[(int)HAL::MIDI_Lighter::Color::Blue];
	Managed_Configuration_MIDI->Shift			= Configuration_MIDI.Shift;
	Managed_Configuration_MIDI->Skip_Note_Off	= Configuration_MIDI.Skip_Note_Off;

	return Managed_Configuration_MIDI;
}

System::Boolean MIDI_Lighter_Wrapper::Set_Configuration_MIDI(MIDI_Lighter_Wrapper::Configuration_MIDI^ midi)
{
	HAL::MIDI_Lighter::Configuration_MIDI Configuration_MIDI;

	Configuration_MIDI.Channel = (HAL::MIDI_Lighter::MIDI_Channel) midi->Channel;
	Configuration_MIDI.Note[(int)HAL::MIDI_Lighter::Color::Red]		= (HAL::MIDI_Lighter::MIDI_Note) midi->Note[(int)MIDI_Lighter_Wrapper::Color::Red];
	Configuration_MIDI.Note[(int)HAL::MIDI_Lighter::Color::Green]	= (HAL::MIDI_Lighter::MIDI_Note) midi->Note[(int)MIDI_Lighter_Wrapper::Color::Green];
	Configuration_MIDI.Note[(int)HAL::MIDI_Lighter::Color::Blue]	= (HAL::MIDI_Lighter::MIDI_Note) midi->Note[(int)MIDI_Lighter_Wrapper::Color::Blue];
	Configuration_MIDI.Shift			= midi->Shift;
	Configuration_MIDI.Skip_Note_Off	= midi->Skip_Note_Off;

	return _MIDI_Lighter->Set_Configuration_MIDI(Configuration_MIDI);
}

MIDI_Lighter_Wrapper::Configuration_No_Data_Light^ MIDI_Lighter_Wrapper::Get_Configuration_No_Data_Light(System::Boolean^ read_success)
{
	bool Success;

	HAL::MIDI_Lighter::Configuration_No_Data_Light Configuration_No_Data_Light = _MIDI_Lighter->Get_Configuration_No_Data_Light(&Success);

	read_success = Success;

	MIDI_Lighter_Wrapper::Configuration_No_Data_Light^ Managed_Configuration_No_Data_Light = gcnew MIDI_Lighter_Wrapper::Configuration_No_Data_Light();

	Managed_Configuration_No_Data_Light->Color = System::Drawing::Color::FromArgb(	Configuration_No_Data_Light.Color[(int)HAL::MIDI_Lighter::Color::Red],
																					Configuration_No_Data_Light.Color[(int)HAL::MIDI_Lighter::Color::Green],
																					Configuration_No_Data_Light.Color[(int)HAL::MIDI_Lighter::Color::Blue]);
	
	Managed_Configuration_No_Data_Light->Timeout = Configuration_No_Data_Light.Timeout;

	return Managed_Configuration_No_Data_Light;
}

System::Boolean MIDI_Lighter_Wrapper::Set_Configuration_No_Data_Light(MIDI_Lighter_Wrapper::Configuration_No_Data_Light^ no_data_light)
{
	HAL::MIDI_Lighter::Configuration_No_Data_Light Configuration_No_Data_Light;

	Configuration_No_Data_Light.Color[(int)HAL::MIDI_Lighter::Color::Red]	= no_data_light->Color.R;
	Configuration_No_Data_Light.Color[(int)HAL::MIDI_Lighter::Color::Green]	= no_data_light->Color.G;
	Configuration_No_Data_Light.Color[(int)HAL::MIDI_Lighter::Color::Blue]	= no_data_light->Color.B;
	Configuration_No_Data_Light.Timeout = no_data_light->Timeout;

	return _MIDI_Lighter->Set_Configuration_No_Data_Light(Configuration_No_Data_Light);
}

MIDI_Lighter_Wrapper::Configuration_Permanent_Light^ MIDI_Lighter_Wrapper::Get_Configuration_Permanent_Light(System::Boolean^ read_success)
{
	bool Success;

	HAL::MIDI_Lighter::Configuration_Permanent_Light Configuration_Permanent_Light = _MIDI_Lighter->Get_Configuration_Permanent_Light(&Success);

	read_success = Success;

	MIDI_Lighter_Wrapper::Configuration_Permanent_Light^ Managed_Configuration_Permanent_Light = gcnew MIDI_Lighter_Wrapper::Configuration_Permanent_Light();

	Managed_Configuration_Permanent_Light->Color = System::Drawing::Color::FromArgb(Configuration_Permanent_Light.Color[(int)HAL::MIDI_Lighter::Color::Red],
																					Configuration_Permanent_Light.Color[(int)HAL::MIDI_Lighter::Color::Green],
																					Configuration_Permanent_Light.Color[(int)HAL::MIDI_Lighter::Color::Blue]);

	return Managed_Configuration_Permanent_Light;
}

System::Boolean MIDI_Lighter_Wrapper::Set_Configuration_Permanent_Light(MIDI_Lighter_Wrapper::Configuration_Permanent_Light^ permanent_light)
{
	HAL::MIDI_Lighter::Configuration_Permanent_Light Configuration_Permanent_Light;

	Configuration_Permanent_Light.Color[(int)HAL::MIDI_Lighter::Color::Red]		= permanent_light->Color.R;
	Configuration_Permanent_Light.Color[(int)HAL::MIDI_Lighter::Color::Green]	= permanent_light->Color.G;
	Configuration_Permanent_Light.Color[(int)HAL::MIDI_Lighter::Color::Blue]	= permanent_light->Color.B;

	return _MIDI_Lighter->Set_Configuration_Permanent_Light(Configuration_Permanent_Light);
}

System::Byte MIDI_Lighter_Wrapper::Read_EEPROM(System::UInt32 address, System::Boolean^ read_success)
{
	bool Success;
	
	System::Byte Return_Value = _MIDI_Lighter->Read_EEPROM(address, &Success);

	if (!Success)
	{
		std::cout << "EEPROM read failed" << std::endl;
	}

	read_success = Success;

	return Return_Value;
}

System::Boolean	MIDI_Lighter_Wrapper::Write_EEPROM()
{
	return _MIDI_Lighter->Write_EEPROM();
}

System::Void MIDI_Lighter_Wrapper::SystemString2StdString(System::String^ s, std::string& os)
{
	using namespace System::Runtime::InteropServices;
	const char* chars = (const char*)(Marshal::StringToHGlobalAnsi(s)).ToPointer();
	os = chars;
	Marshal::FreeHGlobal(System::IntPtr((void*)chars));
}