#pragma once

#include "MIDI_Lighter_CLI_Wrapper.h" 

namespace MIDI_Lighter
{
	public delegate void Update_Values_Configuration_MIDI(MIDI_Lighter_Wrapper::Configuration_MIDI^ configuration_midi);
	
	public ref class Configuration_MIDI : public System::Windows::Forms::UserControl
	{
	public:
		Configuration_MIDI();

		event Update_Values_Configuration_MIDI^ Update_Values;

		System::Void Update(MIDI_Lighter_Wrapper::Configuration_MIDI^ configuration_midi);
		System::Void Value_Changed(System::Object^ sender, System::EventArgs^ e);

	protected:
		~Configuration_MIDI() { };

	private:
		System::Windows::Forms::GroupBox^			_GroupBox_Main;
		System::Windows::Forms::ComboBox^			_ComboBox_Channel;
		array<System::Windows::Forms::ComboBox^>^	_ComboBox_Notes;
		System::Windows::Forms::TrackBar^			_TrackBar_Factor;
		System::Windows::Forms::Label^				_Label_Factor;
		System::Windows::Forms::CheckBox^			_CheckBox_Skip_Note_Off;

		System::Boolean								_Internal_Update;

		void Label_Factor_Update(unsigned int value);
	};
}
