#pragma once

#include "Device_Name.h"
#include "Status_Bar.h"
#include "Connection.h"
#include "Configuration_MIDI.h"
#include "Configuration_RGB_Order.h"
#include "Configuration_No_Data_Light.h"
#include "Configuration_Permanent_Light.h"

#include "Debug.h"

namespace MIDI_Lighter
{
	public ref class Device : public System::Windows::Forms::UserControl
	{
	public:
		Device();

	protected:
		~Device() { };

	private:
		System::Resources::ResourceManager^				_Resources;

		MIDI_Lighter::Connection^						_Connection;
		MIDI_Lighter::Configuration_MIDI^				_Configuration_MIDI;
		MIDI_Lighter::Configuration_No_Data_Light^		_Configuration_No_Data_Light;
		MIDI_Lighter::Configuration_Permanent_Light^	_Configuration_Permanent_Light;
		MIDI_Lighter::Device_Name^						_Device_Name;
		MIDI_Lighter::Configuration_RGB_Order^			_Configuration_RGB_Order;

		System::Windows::Forms::Button^					_Button_Write_EEPROM;

		MIDI_Lighter::Status_Bar^						_Status_Bar;

		MIDI_Lighter::Debug^							_Debug;
		LogProgress::LogProgress^						_LogProgress;

		System::Boolean									_EEPROM_Write_Pending;


		System::Void Connection_Connection_Changed(MIDI_Lighter::USB_CONNECTION status);
		System::Void Sync_Status_Changed(MIDI_Lighter::SYNC_STATUS status);

		System::Void Configuration_Changed(System::Boolean pending);

		System::Void Button_Read_EEPROM_Click(System::Object^ sender, System::EventArgs^ e);
	};
}
