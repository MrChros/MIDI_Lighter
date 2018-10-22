#pragma once


#include <vector>

#include "Common.h"
#include "USB_Link.h"


namespace HAL
{
	class MIDI_Lighter
	{
	public:
		enum class Command
		{
			Terminator				= 0x0D,	
			
			Set_Device_Name			= 'A',
			Get_Device_Name			= 'a',

			Set_MIDI				= 'B',
			Get_MIDI				= 'b',

			Set_No_Data_Light		= 'C',
			Get_No_Data_Light		= 'c',

			Set_Permanent_Light		= 'D',
			Get_Permanent_Light		= 'd',

			Set_RGB_Order			= 'E',
			Get_RGB_Order			= 'e',

			Get_ADC					= 'f',

			Set_Timer1_Top			= 'V',
			
			Set_Timer4_Prescaler	= 'W',

			Write_EEPROM			= 'Y',
			Read_EEPROM				= 'y'
		};

		enum class No_Data_Light_Deactivate
		{
			ANY_TRAFFIC,
			CHANNEL_MATCH,
			EVENT_MATCH,
			CHANNEL_AND_EVENT_MATCH
		};
		
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

		enum class Timer4_Presacler
		{
			PRESCALER_1 = 0x1,
			PRESCALER_2 = 0x2,
			PRESCALER_4 = 0x3,
			PRESCALER_8 = 0x4,
			PRESCALER_16 = 0x5,
			PRESCALER_32 = 0x6,
			PRESCALER_64 = 0x7,
			PRESCALER_128 = 0x8,
			PRESCALER_256 = 0x9,
			PRESCALER_512 = 0xA,
			PRESCALER_1024 = 0xB,
			PRESCALER_2048 = 0xC,
			PRESCALER_4096 = 0xD,
			PRESCALER_8192 = 0xE,
			PRESCALER_16384 = 0xF
		};

		struct Device
		{
			uint8_t Index;
			std::string Name;

			Device()
			{
				Index = 0;
				Name = "";
			}
		};

		struct Configuration_MIDI
		{
			HAL::MIDI_Lighter::MIDI_Channel Channel;
			HAL::MIDI_Lighter::MIDI_Note	Note[3];
			uint8_t							Factor;
			bool							Skip_Note_Off;

			Configuration_MIDI()
			{
				Channel = HAL::MIDI_Lighter::MIDI_Channel::CH01;
				Note[(int)HAL::MIDI_Lighter::Color::Red]	= HAL::MIDI_Lighter::MIDI_Note::Cn1;
				Note[(int)HAL::MIDI_Lighter::Color::Green]	= HAL::MIDI_Lighter::MIDI_Note::Cn1;
				Note[(int)HAL::MIDI_Lighter::Color::Blue]	= HAL::MIDI_Lighter::MIDI_Note::Cn1;
				Factor			= 100;
				Skip_Note_Off	= false;
			}
		};

		struct Configuration_No_Data_Light
		{
			uint8_t										Color[3];
			uint8_t										Timeout;
			HAL::MIDI_Lighter::No_Data_Light_Deactivate Deactivate;

			Configuration_No_Data_Light()
			{
				Color[(int)HAL::MIDI_Lighter::Color::Red]	= 0;
				Color[(int)HAL::MIDI_Lighter::Color::Green] = 0;
				Color[(int)HAL::MIDI_Lighter::Color::Blue]	= 0;

				Timeout = 1;

				Deactivate = HAL::MIDI_Lighter::No_Data_Light_Deactivate::ANY_TRAFFIC;
			}
		};

		struct Configuration_Permanent_Light
		{
			uint8_t Color[3];

			Configuration_Permanent_Light()
			{
				Color[(int)HAL::MIDI_Lighter::Color::Red]	= 0;
				Color[(int)HAL::MIDI_Lighter::Color::Green] = 0;
				Color[(int)HAL::MIDI_Lighter::Color::Blue]	= 0;
			}
		};

		struct Configuration_RGB_Order
		{
			HAL::MIDI_Lighter::RGB_Order RGB_Order;

			Configuration_RGB_Order()
			{
				RGB_Order = HAL::MIDI_Lighter::RGB_Order::RGB;
			}
		};
		
		
		MIDI_Lighter();
		~MIDI_Lighter();

		uint32_t Number_Of_Devices();
		std::vector<HAL::MIDI_Lighter::Device> Get_Device_List();
		
		bool Connect(unsigned int index = 0U);
		bool IsConnected();
		void Disconnect();

		std::string Get_Device_Name(bool* read_success);
		bool		Set_Device_Name(std::string name);

		HAL::MIDI_Lighter::Configuration_MIDI				Get_Configuration_MIDI(bool* read_success);
		bool												Set_Configuration_MIDI(HAL::MIDI_Lighter::Configuration_MIDI midi);

		HAL::MIDI_Lighter::Configuration_No_Data_Light		Get_Configuration_No_Data_Light(bool* read_success);
		bool												Set_Configuration_No_Data_Light(HAL::MIDI_Lighter::Configuration_No_Data_Light no_data_light);

		HAL::MIDI_Lighter::Configuration_Permanent_Light	Get_Configuration_Permanent_Light(bool* read_success);
		bool												Set_Configuration_Permanent_Light(HAL::MIDI_Lighter::Configuration_Permanent_Light permanent_light);

		HAL::MIDI_Lighter::Configuration_RGB_Order			Get_Configuration_RGB_Order(bool* read_success);
		bool												Set_Configuration_RGB_Order(HAL::MIDI_Lighter::Configuration_RGB_Order rgb_order);

		uint32_t											Get_ADC(bool* read_success);


		bool												Set_Timer1_Top(uint16_t top);
		bool												Set_Timer4_Prescaler(HAL::MIDI_Lighter::Timer4_Presacler prescaler);

		uint8_t												Read_EEPROM(uint32_t address, bool* read_success);
		bool												Write_EEPROM();

	private:
		const std::string _GUID = "{0049DC4F-75DF-419F-B3EA-4BBDC1E08253}";
		
		USB_Link _USB_Link;
		USB_Device::Endpoint _Endpoint_RX;
		USB_Device::Endpoint _Endpoint_TX;
	};
}