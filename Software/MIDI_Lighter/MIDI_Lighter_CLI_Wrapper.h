#pragma once

#include "MIDI_Lighter.h"

public ref class MIDI_Lighter_Wrapper
{
public:
	
		enum class RGB_Order
		{
			RGB = 0,
			RBG = 1,
			GRB = 2,
			GBR = 3,
			BRG = 4,
			BGR = 5
		};
		
		enum class Color
		{
			Red		= 0,
			Green	= 1,
			Blue	= 2
		};

		enum class MIDI_Channel
		{
			CH01 = 0,
			CH02 = 1,
			CH03 = 2,
			CH04 = 3,
			CH05 = 4,
			CH06 = 5,
			CH07 = 6,
			CH08 = 7,
			CH09 = 8,
			CH10 = 9,
			CH11 = 10,
			CH12 = 11,
			CH13 = 12,
			CH14 = 13,
			CH15 = 14,
			CH16 = 15
		};

		enum class MIDI_Note
		{
			Cn1	= 0,
			Cmn1= 1,
			Dn1	= 2,
			Ebn1= 3,
			En1	= 4,
			Fn1	= 5,
			Fmn1= 6,
			Gn1	= 7,
			Gmn1= 8,
			An1	= 9,
			Bbn1= 10,
			Bn1	= 11,
			C0	= 12,
			Cm0	= 13,
			D0	= 14,
			Eb0	= 15,
			E0	= 16,
			F0	= 17,
			Fm0	= 18,
			G0	= 19,
			Gm0	= 20,
			A0	= 21,
			Bb0	= 22,
			B0	= 23,
			C1	= 24,
			Cm1	= 25,
			D1	= 26,
			Eb1	= 27,
			E1	= 28,
			F1	= 29,
			Fm1	= 30,
			G1	= 31,
			Gm1	= 32,
			A1	= 33,
			Bb1	= 34,
			B1	= 35,
			C2	= 36,
			Cm2	= 37,
			D2	= 38,
			Eb2	= 39,
			E2	= 40,
			F2	= 41,
			Fm2	= 42,
			G2	= 43,
			Gm2	= 44,
			A2	= 45,
			Bb2	= 46,
			B2	= 47,
			C3	= 48,
			Cm3	= 49,
			D3	= 50,
			Eb3	= 51,
			E3	= 52,
			F3	= 53,
			Fm3	= 54,
			G3	= 55,
			Gm3	= 56,
			A3	= 57,
			Bb3	= 58,
			B3	= 59,
			C4	= 60,
			Cm4	= 61,
			D4	= 62,
			Eb4	= 63,
			E4	= 64,
			F4	= 65,
			Fm4	= 66,
			G4	= 67,
			Gm4	= 68,
			A4	= 69,
			Bb4	= 70,
			B4	= 71,
			C5	= 72,
			Cm5	= 73,
			D5	= 74,
			Eb5	= 75,
			E5	= 76,
			F5	= 77,
			Fm5	= 78,
			G5	= 79,
			Gm5	= 80,
			A5	= 81,
			Bb5	= 82,
			B5	= 83,
			C6	= 84,
			Cm6	= 85,
			D6	= 86,
			Eb6	= 87,
			E6	= 88,
			F6	= 89,
			Fm6	= 90,
			G6	= 91,
			Gm6	= 92,
			A6	= 93,
			Bb6	= 94,
			B6	= 95,
			C7	= 96,
			Cm7	= 97,
			D7	= 98,
			Eb7	= 99,
			E7	= 100,
			F7	= 101,
			Fm7	= 102,
			G7	= 103,
			Gm7	= 104,
			A7	= 105,
			Bb7	= 106,
			B7	= 107,
			C8	= 108,
			Cm8	= 109,
			D8	= 110,
			Eb8	= 111,
			E8	= 112,
			F8	= 113,
			Fm8	= 114,
			G8	= 115,
			Gm8	= 116,
			A8	= 117,
			Bb8	= 118,
			B8	= 119
		};

	ref struct Device
	{
		System::UInt32 Index;
		System::String^ Name;

		Device()
		{
			Index = 0;
			Name = "";
		}
	};

	ref struct Configuration_MIDI
	{
		MIDI_Lighter_Wrapper::MIDI_Channel		Channel;
		array<MIDI_Lighter_Wrapper::MIDI_Note>^	Note;
		System::Byte							Factor;
		System::Boolean							Skip_Note_Off;

		Configuration_MIDI()
		{
			Channel = MIDI_Lighter_Wrapper::MIDI_Channel::CH01;
			Note	= gcnew array<MIDI_Lighter_Wrapper::MIDI_Note>(3);
			Note[(int)MIDI_Lighter_Wrapper::Color::Red]		= MIDI_Lighter_Wrapper::MIDI_Note::Cn1;
			Note[(int)MIDI_Lighter_Wrapper::Color::Green]	= MIDI_Lighter_Wrapper::MIDI_Note::Cn1;
			Note[(int)MIDI_Lighter_Wrapper::Color::Blue]	= MIDI_Lighter_Wrapper::MIDI_Note::Cn1;
			Factor			= 100;
			Skip_Note_Off	= false;
		}
	};

	ref struct Configuration_No_Data_Light
	{
		System::Drawing::Color	Color;
		System::Byte			Timeout;

		Configuration_No_Data_Light()
		{
			Color	= System::Drawing::Color::Black;
			Timeout = 1;
		}
	};

	ref struct Configuration_Permanent_Light
	{
		System::Drawing::Color	Color;

		Configuration_Permanent_Light()
		{
			Color = System::Drawing::Color::Black;
		}
	};

	ref struct Configuration_RGB_Order
	{
		MIDI_Lighter_Wrapper::RGB_Order RGB_Order;

		Configuration_RGB_Order()
		{
			RGB_Order = MIDI_Lighter_Wrapper::RGB_Order::RGB;
		}
	};

	MIDI_Lighter_Wrapper();
	~MIDI_Lighter_Wrapper();

	System::UInt32 Number_Of_Devices();
	System::Collections::Generic::List<MIDI_Lighter_Wrapper::Device^ >^ Get_Device_List();

	System::Boolean Connect();
	System::Boolean Connect(System::UInt32 index);
	System::Boolean IsConnected();
	System::Void	Disconnect();

	System::String^ Get_Device_Name(System::Boolean^ read_success);
	System::Boolean	Set_Device_Name(System::String^ name);

	MIDI_Lighter_Wrapper::Configuration_MIDI^				Get_Configuration_MIDI(System::Boolean^ read_success);
	System::Boolean											Set_Configuration_MIDI(MIDI_Lighter_Wrapper::Configuration_MIDI^ midi);

	MIDI_Lighter_Wrapper::Configuration_No_Data_Light^		Get_Configuration_No_Data_Light(System::Boolean^ read_success);
	System::Boolean											Set_Configuration_No_Data_Light(MIDI_Lighter_Wrapper::Configuration_No_Data_Light^ no_data_light);

	MIDI_Lighter_Wrapper::Configuration_Permanent_Light^	Get_Configuration_Permanent_Light(System::Boolean^ read_success);
	System::Boolean											Set_Configuration_Permanent_Light(MIDI_Lighter_Wrapper::Configuration_Permanent_Light^ permanent_light);

	MIDI_Lighter_Wrapper::Configuration_RGB_Order^			Get_Configuration_RGB_Order(System::Boolean^ read_success);
	System::Boolean											Set_Configuration_RGB_Order(MIDI_Lighter_Wrapper::Configuration_RGB_Order^ rgb_order);

	System::UInt16											Get_ADC(System::Boolean^ read_success);

	System::Byte											Read_EEPROM(System::UInt32 address, System::Boolean^ read_success);
	System::Boolean											Write_EEPROM();

private:
	HAL::MIDI_Lighter* _MIDI_Lighter;

	System::Void SystemString2StdString(System::String^ s, std::string& os);
};

