#pragma once

#include "Device_List.h"
#include "LogProgress.h"

namespace MIDI_Lighter
{
	public ref class Debug : System::Windows::Forms::UserControl
	{
	public:
		Debug(MIDI_Lighter::Device_List^ device_list);

	protected:
		~Debug() { }

	private:
		LogProgress::LogProgress^				_LogProgress;
		MIDI_Lighter::Device_List^				_Device_List;

		System::Windows::Forms::Button^			_Button_Read_ADC;
		System::Windows::Forms::Timer^			_Timer_Read_ADC;
		System::UInt32							_Read_ADC_Counter;

		System::Windows::Forms::Button^			_Button_Measurement;
		System::Windows::Forms::Timer^			_Timer_Measurement;


		System::Void Button_Read_EEPROM_Click	(System::Object^ sender, System::EventArgs^ e);
		System::Void Button_Read_ADC_Click		(System::Object^ sender, System::EventArgs^ e);
		System::Void Timer_Read_ADC_Tick		(System::Object^ sender, System::EventArgs^ e);
		System::Void Button_Measurement_Click	(System::Object^ sender, System::EventArgs^ e);
		System::Void Timer_Measurement_Tick		(System::Object^ sender, System::EventArgs^ e);

		
		System::UInt32																_Measurement_Per_Config;
		System::Collections::Generic::List<MIDI_Lighter_Wrapper::Timer4_Presacler>^	_List_Measurement_Prescaler;
		System::Collections::Generic::List<System::UInt16>^							_List_Measurement_Top;

		System::UInt32																_Measurement_Current_Measurement;
		System::Int32																_Measurement_Current_Prescaler;
		System::Int32																_Measurement_Current_Top;

		System::IO::StreamWriter^													_Measurement_Writer;


		/////////////////////////////////
		// Debug_Background_EEPROM.cpp //
		/////////////////////////////////
		System::ComponentModel::BackgroundWorker^			_Background_Worker_Read_EEPROM;
		System::Void Background_Worker_ProgressChanged		(System::Object^ sender, System::ComponentModel::ProgressChangedEventArgs^ e);
		System::Void Background_Worker_RunWorkerCompleted	(System::Object^ sender, System::ComponentModel::RunWorkerCompletedEventArgs^ e);
		System::Void Background_Worker_Read_EEPROM			(System::Object^ sender, System::ComponentModel::DoWorkEventArgs^ e);
	};
}