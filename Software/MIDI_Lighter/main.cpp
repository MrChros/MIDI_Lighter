#include "MIDI_Lighter_GUI.h"

[System::STAThread]
void Main(array<System::String^>^ args)
{
	System::Windows::Forms::Application::EnableVisualStyles();
	System::Windows::Forms::Application::SetCompatibleTextRenderingDefault(false);

	MIDI_Lighter_GUI::MIDI_Lighter_GUI MIDI_Lighter_GUI;
	System::Windows::Forms::Application::Run(%MIDI_Lighter_GUI);
}