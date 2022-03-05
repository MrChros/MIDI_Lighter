#pragma once

#include "Device_List.h"
#include "Device_Name.h"
#include "Configuration_MIDI.h"
#include "Configuration_RGB_Order.h"
#include "Configuration_No_Data_Light.h"
#include "Configuration_Permanent_Light.h"

#include "Debug.h"

namespace MIDI_Lighter
{
	public delegate void Changed_Configuration_MIDI				(MIDI_Lighter_Wrapper::Configuration_MIDI^				configuration_midi);
	public delegate void Changed_Configuration_No_Data_Light	(MIDI_Lighter_Wrapper::Configuration_No_Data_Light^		configuration_no_data_light);
	public delegate void Changed_Configuration_Permanent_Light	(MIDI_Lighter_Wrapper::Configuration_Permanent_Light^	configuration_permantent_light);
	public delegate void Changed_Device_Name					(MIDI_Lighter_Wrapper::Device^							device);
	public delegate void Changed_Configuration_RGB_Order		(MIDI_Lighter_Wrapper::Configuration_RGB_Order^			configuration_rgb_order);
	public delegate void EEPROM_Save							();

	public ref class Device : public System::Windows::Forms::UserControl
	{
	public:
		Device();

		event Changed_Configuration_MIDI^					Changed_Configuration_MIDI;
		event Changed_Configuration_No_Data_Light^			Changed_Configuration_No_Data_Light;
		event Changed_Configuration_Permanent_Light^		Changed_Configuration_Permanent_Light;
		event Changed_Device_Name^							Changed_Device_Name;
		event Changed_Configuration_RGB_Order^				Changed_Configuration_RGB_Order;
		event EEPROM_Save^									EEPROM_Save;

		System::Void Connection_Connection_Changed			(MIDI_Lighter::USB_CONNECTION status);
		System::Void Set_Configuration_MIDI					(MIDI_Lighter_Wrapper::Configuration_MIDI^				configuration_midi);
		System::Void Set_Configuration_No_Data_Light		(MIDI_Lighter_Wrapper::Configuration_No_Data_Light^		configuration_no_data_light);
		System::Void Set_Configuration_Permanent_Light		(MIDI_Lighter_Wrapper::Configuration_Permanent_Light^	configuration_permanent_light);
		System::Void Set_Configuration_Device				(MIDI_Lighter_Wrapper::Device^							device);
		System::Void Set_Configuration_RGB_Order			(MIDI_Lighter_Wrapper::Configuration_RGB_Order^			configuration_rgb_order);

		System::Void Configuration_Changed					(System::Boolean pending);

	protected:
		~Device() { };

	private:
		System::Resources::ResourceManager^					_Resources;

		System::Windows::Forms::Label^						_Label_Header;
		
		MIDI_Lighter::Configuration_MIDI^					_Configuration_MIDI;
		MIDI_Lighter::Configuration_No_Data_Light^			_Configuration_No_Data_Light;
		MIDI_Lighter::Configuration_Permanent_Light^		_Configuration_Permanent_Light;
		MIDI_Lighter::Device_Name^							_Device_Name;
		MIDI_Lighter::Configuration_RGB_Order^				_Configuration_RGB_Order;

		System::Windows::Forms::Button^						_Button_Write_EEPROM;

		System::Boolean										_EEPROM_Write_Pending;

		System::Void Update_Configuration_MIDI				(MIDI_Lighter_Wrapper::Configuration_MIDI^				configuration_midi);
		System::Void Update_Configuration_No_Data_Light		(MIDI_Lighter_Wrapper::Configuration_No_Data_Light^		configuration_no_data_light);
		System::Void Update_Configuration_Permanent_Light	(MIDI_Lighter_Wrapper::Configuration_Permanent_Light^	configuration_permanent_light);
		System::Void Update_Device							(MIDI_Lighter_Wrapper::Device^							device);
		System::Void Update_Configuration_RGB_Order			(MIDI_Lighter_Wrapper::Configuration_RGB_Order^			configuration_rgb_order);


		System::Void Button_Save_EEPROM_Click				(System::Object^ sender, System::EventArgs^ e);
		System::Void Update_Header							(System::String^ device_name);
	};
}
