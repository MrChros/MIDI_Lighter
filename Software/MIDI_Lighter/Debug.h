#pragma once

#include "Connection.h"
#include "LogProgress.h"

namespace MIDI_Lighter
{
	public ref class Debug : System::Object
	{
	public:
		Debug(LogProgress::LogProgress^ logprogress, MIDI_Lighter::Connection^ connection);

		System::ComponentModel::BackgroundWorker^ Background_Worker_Read_EEPROM();
	
	protected:
		~Debug() { }

	private:
		LogProgress::LogProgress^	_LogProgress;
		MIDI_Lighter::Connection^	_Connection;

		System::ComponentModel::BackgroundWorker^ _Background_Worker_Read_EEPROM;

		System::Void Background_Worker_ProgressChanged(System::Object^ sender, System::ComponentModel::ProgressChangedEventArgs^ e);
		System::Void Background_Worker_RunWorkerCompleted(System::Object^ sender, System::ComponentModel::RunWorkerCompletedEventArgs^ e);
		System::Void Background_Worker_Read_EEPROM(System::Object^ sender, System::ComponentModel::DoWorkEventArgs^ e);
	};
}