#pragma once

#include "MIDI_Lighter_CLI_Wrapper.h"

namespace MIDI_Lighter
{
	public delegate void Update_Values_Configuration_No_Data_Light(MIDI_Lighter_Wrapper::Configuration_No_Data_Light^ configuration_no_data_light);
	
	public ref class Configuration_No_Data_Light : public System::Windows::Forms::UserControl
	{
	public:
		Configuration_No_Data_Light();

		event Update_Values_Configuration_No_Data_Light^ Update_Values;

		System::Void Update(MIDI_Lighter_Wrapper::Configuration_No_Data_Light^ configuration_no_data_light);
		System::Void Value_Changed(System::Object^ sender, System::EventArgs^ e);

	protected:
		~Configuration_No_Data_Light() { }

	private:
		System::Resources::ResourceManager^		_Resources;

		System::Windows::Forms::GroupBox^		_GroupBox_Main;
		System::Windows::Forms::Label^			_Label_Color;
		System::Windows::Forms::ColorDialog^	_ColorDialog;
		System::Windows::Forms::NumericUpDown^	_Numeric_Timeout;

		System::Boolean							_Internal_Update;

		System::Void Button_Color_Click(System::Object^ sender, System::EventArgs^ e);
	};
}