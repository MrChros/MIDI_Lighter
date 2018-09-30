#pragma once

#include "MIDI_Lighter_CLI_Wrapper.h"

namespace MIDI_Lighter
{
	public delegate void Update_Values_Configuration_RGB_Order(MIDI_Lighter_Wrapper::Configuration_RGB_Order^ configuration_rgb_order);

	public ref class Configuration_RGB_Order : public System::Windows::Forms::UserControl
	{
	public:
		Configuration_RGB_Order();

		event Update_Values_Configuration_RGB_Order^ Update_Values;

		System::Void Update(MIDI_Lighter_Wrapper::Configuration_RGB_Order^ configuration_rgb_order);

	protected:
		~Configuration_RGB_Order() { }

	private:
		System::Windows::Forms::GroupBox^	_GroupBox_Main;
		System::Windows::Forms::ComboBox^	_ComboBox_RGB_Order;

		System::Boolean						_Internal_Update;

		System::Void Value_Changed(System::Object^ sender, System::EventArgs^ e);
	};
}
