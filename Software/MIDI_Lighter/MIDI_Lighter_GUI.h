#pragma once

#include <iostream>

#include "Device.h"
#include "Status_Bar.h"
#include "Connection.h"
#include "Configuration_MIDI.h"
#include "Configuration_No_Data_Light.h"
#include "Configuration_Permanent_Light.h"

#include "Debug.h"

#include "Device_List.h"
#include "Device_Edit.h"

namespace MIDI_Lighter_GUI
{
	public ref class MIDI_Lighter_GUI : public System::Windows::Forms::Form
	{
	private:
		System::Resources::ResourceManager^	_Resources;
		
		System::Windows::Forms::SplitContainer^ _Split_Container;
		


		void Split_Container_SplitterMoved(System::Object^ sender, System::Windows::Forms::SplitterEventArgs^ e);
		
		
		MIDI_Lighter::Connection^						_Connection;
		MIDI_Lighter::Configuration_MIDI^				_Configuration_MIDI;
		MIDI_Lighter::Configuration_No_Data_Light^		_Configuration_No_Data_Light;
		MIDI_Lighter::Configuration_Permanent_Light^	_Configuration_Permanent_Light;
		MIDI_Lighter::Device^							_Device;

		System::Windows::Forms::Button^					_Button_Write_EEPROM;

		MIDI_Lighter::Status_Bar^						_Status_Bar;

		MIDI_Lighter::Debug^							_Debug;
		LogProgress::LogProgress^						_LogProgress;

		System::Boolean									_EEPROM_Write_Pending;


		System::Void Connection_Connection_Changed(MIDI_Lighter::USB_CONNECTION status);
		System::Void Sync_Status_Changed(MIDI_Lighter::SYNC_STATUS status);

		System::Void Configuration_Changed(System::Boolean pending);
	
		System::Void Button_Read_EEPROM_Click(System::Object^ sender, System::EventArgs^ e);

	protected:
		~MIDI_Lighter_GUI();

	public:
		MIDI_Lighter_GUI(void);
	};
}

