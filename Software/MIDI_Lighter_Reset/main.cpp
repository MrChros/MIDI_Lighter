#include <iostream>

#include "MIDI_Lighter.h"

int Main(array<System::String^>^ args)
{
	HAL::MIDI_Lighter MIDI_Lighter;

	MIDI_Lighter.Connect();

	if (MIDI_Lighter.IsConnected())
	{
		if (MIDI_Lighter.Perform_Reset())
		{
			std::cout << "Success" << std::endl;
		}
		else
		{
			std::cout << "Reset Failed" << std::endl;
			return 1;
		}
	}
	else
	{
		std::cout << "Could not connect" << std::endl;
		return 1;
	}
	MIDI_Lighter.Disconnect();

	return 0;
}