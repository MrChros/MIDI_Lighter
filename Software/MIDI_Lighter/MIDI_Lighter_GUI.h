#pragma once

#include <iostream>

#include "Device.h"
#include "Device_List.h"
#include "Status_Bar.h"
#include "Configuration_MIDI.h"
#include "Configuration_No_Data_Light.h"
#include "Configuration_Permanent_Light.h"

#include "Debug.h"

namespace MIDI_Lighter_GUI
{
	public ref class MIDI_Lighter_GUI : public System::Windows::Forms::Form
	{
	private:
		System::Resources::ResourceManager^			_Resources;
		
		System::Windows::Forms::SplitContainer^		_Split_Container;
		
		MIDI_Lighter::Device_List^					_Device_List;
		MIDI_Lighter::Device^						_Device;
		MIDI_Lighter::Debug^						_Debug;
		MIDI_Lighter::Status_Bar^					_Status_Bar;

		System::Void Split_Container_SplitterMoved	(System::Object^ sender, System::Windows::Forms::SplitterEventArgs^ e);
		System::Void Connection_Connection_Changed	(MIDI_Lighter::USB_CONNECTION status);
		System::Void Sync_Status_Changed			(MIDI_Lighter::SYNC_STATUS status);

		System::Void Configuration_Changed			(System::Boolean pending);
	
	protected:
		~MIDI_Lighter_GUI();

	public:
		MIDI_Lighter_GUI(void);
	};
}

