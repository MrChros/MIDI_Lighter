#pragma once

#include "Device.h"

namespace MIDI_Lighter
{
	public ref class Device_Edit : public System::Windows::Forms::UserControl
	{
	public:
		Device_Edit();

	protected:
		~Device_Edit() { };

	private:
		System::Resources::ResourceManager^			_Resources;

		System::Windows::Forms::VScrollBar^			_VScrollBar;
		System::Windows::Forms::TableLayoutPanel^	_Table_Layout_Devices;
	};
}