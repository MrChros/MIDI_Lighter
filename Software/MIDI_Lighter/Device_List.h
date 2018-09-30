#pragma once

#include "MIDI_Lighter_CLI_Wrapper.h"

namespace MIDI_Lighter
{
	public ref class Device_List : public System::Windows::Forms::UserControl
	{
	public:
		Device_List();

	protected:
		~Device_List() { };

	private:
		MIDI_Lighter_Wrapper^					_MIDI_Lighter;
		System::Resources::ResourceManager^		_Resources;
		

		System::Windows::Forms::Button^			_Button_Refresh;
		System::Windows::Forms::DataGridView^	_DataGrid_Devices;
		System::Windows::Forms::Label^			_Label_Number_Devices;

		System::Void Button_Refresh_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void Label_Number_Devices_Update();
	};
}

