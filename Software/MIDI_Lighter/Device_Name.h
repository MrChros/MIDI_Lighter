#pragma once

#include "MIDI_Lighter_CLI_Wrapper.h"

namespace MIDI_Lighter
{
	public delegate void Update_Values_Device(MIDI_Lighter_Wrapper::Device^ device);
	
	public ref class Device_Name : public System::Windows::Forms::UserControl
	{
	public:
		Device_Name();

		event Update_Values_Device^ Update_Values;

		System::Void Update(MIDI_Lighter_Wrapper::Device^ device);

	protected:
		~Device_Name() { }

	private:
		System::Windows::Forms::GroupBox^	_GroupBox_Main;
		System::Windows::Forms::TextBox^	_TextBox_Name;

		System::Void TextBox_Name_KeyPress(System::Object^ sender, System::Windows::Forms::KeyPressEventArgs^ e);
		System::Void Button_Set_Name_Click(System::Object^ sender, System::EventArgs^ e);
	};
}