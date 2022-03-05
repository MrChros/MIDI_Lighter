#ifndef COMMON_H_
#define COMMON_H_

#include <math.h>
#include <string>

#ifdef __linux__
	#include <unistd.h>
#elif _WIN32
	#include <Windows.h>
#endif

namespace HAL
{
	/**
	 * Namespace for general purpose functions.
	 */
	namespace Common
	{
		char Ascii(unsigned int value, bool use_uppercase = false);
		unsigned int Int_From_Ascii(char ascii);
		unsigned int Int_From_Ascii(char* ascii, unsigned int size);
		unsigned int Int_From_Ascii_Hex(char* ascii, unsigned int size);

		std::string String_Hex(unsigned int value, unsigned int positions, bool add_0x = false, bool use_uppercase = false);

		unsigned int	Max(unsigned int a, int b);
		unsigned int	Max(int a, unsigned int b);
		int				Max(int a, int b);
		int				Max(int a, int b, int c);
		unsigned int	Max(unsigned int a, unsigned int b);
		unsigned int	Max(unsigned int a, unsigned int b, unsigned int c);
		unsigned int	Max(unsigned int* data, unsigned int size);
		int				Max(int* data, unsigned int size);
		float			Max(float a, float b);
		float			Max(float a, float b, float c);
		float			Max(float* data, unsigned int size);

		int				Min(unsigned int a, int b);
		int				Min(int a, unsigned int b);
		int				Min(int a, int b);
		int				Min(int a, int b, int c);
		unsigned int	Min(unsigned int a, unsigned int b);
		unsigned int	Min(unsigned int a, unsigned int b, unsigned int c);
		unsigned int	Min(unsigned int* data, unsigned int size);
		int				Min(int* data, unsigned int size);
		float			Min(float a, float b);
		float			Min(float a, float b, float c);
		float			Min(float* data, unsigned int size);
		
		void u_Sleep(unsigned int usec);
	}
}

#endif