#pragma once

#include "MIDI_Lighter_CLI_Wrapper.h"

namespace MIDI_Lighter
{
	public delegate void Update_Values_Configuration_Permanent_Light(MIDI_Lighter_Wrapper::Configuration_Permanent_Light^ configuration_permanent_light);
	
	public ref class Configuration_Permanent_Light : public System::Windows::Forms::UserControl
	{
	public:
		Configuration_Permanent_Light();

		event Update_Values_Configuration_Permanent_Light^ Update_Values;

		System::Void Update(MIDI_Lighter_Wrapper::Configuration_Permanent_Light^ configuration_permanent_light);

	protected:
		~Configuration_Permanent_Light() { }

	private:
		System::Resources::ResourceManager^		_Resources;
		
		System::Windows::Forms::GroupBox^		_GroupBox_Main;
		System::Windows::Forms::Label^			_Label_Color;
		System::Windows::Forms::ColorDialog^	_ColorDialog;

		System::Boolean							_Internal_Update;

		System::Void Label_Color_Click(System::Object^ sender, System::EventArgs^ e);
	};
}
