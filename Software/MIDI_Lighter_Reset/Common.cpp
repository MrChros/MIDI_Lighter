
#include "Common.h"

/**
 * Function to convert an integer number to the corresponding ASCII char. 
 * Numbers between 10 and 15 are converted to the hex representation.
 * Larger numbers do not return a valid ASCII representation.
 * \n \n Examples:
 * \n in: 9 - out: '9'
 * \n in: 10 - out: 'a' or 'A'
 * @param [in] value Integer input value.
 * @param [in] use_uppercase Choose whether numbers between 10 and 15 should result in upper- or lowercase hex representations.
 * @return ASCII char.
 */
char HAL::Common::Ascii(unsigned int value, bool use_uppercase)
{
	if ((value >= 0 && value <= 15))
	{
		if (value >= 0 && value <= 9)
		{
			return value + '0';
		}
		else
		{
			if(use_uppercase)
			{
				return value + 'A' - 10;
			}
			else
			{
				return value + 'a' - 10;
			}
		}
	}
	else
	{
		return 0;
	}
}


/**
 * Function to convert a ASCII char number between '0' and '9', 'a' and 'f' or 'A' and 'F' into the corresponding integer representation.
 * Chars between 'a' and 'f' or 'A' and 'F' are interpreted as ASCII hex chars.
 * Other chars do not return a valid number.
 * \n \n Examples:
 * \n in: '9' - out: 9
 * \n in: 'a' or 'A' - out: 10
 * @param [in] ascii ASCII char to be converted.
 * @return Integer representation.
 */
unsigned int HAL::Common::Int_From_Ascii(char ascii)
{
	if (ascii >= '0' && ascii <= '9')
	{
		return (unsigned int)(ascii - '0');
	}
	else if (ascii >= 'A' && ascii <= 'F')
	{
		return (unsigned int)(ascii - 'A') + 10;
	}
	else if (ascii >= 'a' && ascii <= 'f')
	{
		return (unsigned int)(ascii - 'a') + 10;
	}

	return 0;
}


/**
 * Function to convert an array of ASCII char numbers into the corresponding integer representation. The input is interpreted as a decimal number (base 10).
 * Each char should be between '0' and '9'.
 * Chars between 'a' and 'f' or 'A' and 'F' are interpreted as ASCII hex chars, BUT(!) this function does not interpret the input char array as to the base of 16!
 * This might lead to unexpected results when the input char contains hex chars!
 * \n \n Examples:
 * \n in: "19" - out: 19
 * \n in: "5942" - out: 5942
 * @param [in] ascii ASCII char array to be converted.
 * @param [in] size Size of the input ASCII char array.
 * @return Integer representation.
 */
unsigned int HAL::Common::Int_From_Ascii(char* ascii, unsigned int size)
{
	unsigned int Value = 0;

	for (unsigned int i = 0;i < size;i++)
	{
		Value += HAL::Common::Int_From_Ascii(ascii[size - i - 1]) * (unsigned int)pow(10, i);
	}

	return Value;
}


/**
 * Function to convert an array of ASCII char numbers into the corresponding integer representation. The input is interpreted as a hexadecimal number (base 16).
 * Each char should be between '0' and '9', 'a' and 'f' or 'A' and 'F'.
 * \n \n Examples:
 * \n in: "19" - out: 25
 * \n in: "5942" - out: 22850
 * \n in: "ff" - out: 255
 * \n in: "2F8a" - out: 12170
 * @param [in] ascii ASCII char array to be converted.
 * @param [in] size Size of the input ASCII char array.
 * @return Integer representation.
 */
unsigned int HAL::Common::Int_From_Ascii_Hex(char* ascii, unsigned int size)
{
	unsigned int Value = 0;

	for (unsigned int i = 0;i < size;i++)
	{
		Value += HAL::Common::Int_From_Ascii(ascii[size - i - 1]) * (unsigned int)pow(16, i);
	}

	return Value;
}


/**
 * Function to convert an integer number to a hexadecimal string representation, with some format options.
 * \n \n Examples:
 * \n value = 11, positions = 1, add_0x = true, use_uppercase = true - out: "0xB"
 * \n value = 412, positions = 2, add_0x = false, use_uppercase = false - out: "9c"
 * \n value = 412, positions = 3, add_0x = false, use_uppercase = false - out: "19c"
 * \n value = 412, positions = 4, add_0x = false, use_uppercase = true - out: "019C"
 * \n value = 412, positions = 4, add_0x = true, use_uppercase = true - out: "0x019C"
 * @param [in] value An integer number.
 * @param [in] positions Number of hexadecimal positions of the input number to convert.
 * @param [in] add_0x Add a leading "0x" to the resulting string.
 * @param [in] use_uppercase Choose whether numbers between 10 and 15 should result in upper- or lowercase hex representations.
 * @return Hexadecimal string representation.
 */
std::string HAL::Common::String_Hex(unsigned int value, unsigned int positions, bool add_0x, bool use_uppercase)
{
	std::string Hex_String = "";

	if (add_0x)
	{
		Hex_String = "0x";
	}
	
	for (unsigned int i = 0; i<positions; i++)
	{
		Hex_String += HAL::Common::Ascii((value >> ((positions - i - 1) * 4)) & 0x0F, use_uppercase);
	}

	return Hex_String;
}


/**
 * Function to calculate the maximum of two inputs.
 * @param [in] a Input A.
 * @param [in] b Input B.
 * @return Larger number.
 */
unsigned int HAL::Common::Max(unsigned int a, int b)
{
	if ((int)a > b)
	{
		return a;
	}
	else
	{
		return (unsigned int)b;
	}
}


/**
 * Function to calculate the maximum of two inputs.
 * @param [in] a Input A.
 * @param [in] b Input B.
 * @return Larger number.
 */
unsigned int HAL::Common::Max(int a, unsigned int b)
{
	if (a > (int)b)
	{
		return (unsigned int)a;
	}
	else
	{
		return b;
	}
}


/**
 * Function to calculate the maximum of two inputs.
 * @param [in] a Input A.
 * @param [in] b Input B.
 * @return Larger number.
 */
int	HAL::Common::Max(int a, int b)
{
	if (a > b)
	{
		return a;
	}
	else
	{
		return b;
	}
}


/**
 * Function to calculate the maximum of three inputs.
 * @param [in] a Input A.
 * @param [in] b Input B.
 * @param [in] c Input C.
 * @return Largest number.
 */
int	HAL::Common::Max(int a, int b, int c)
{
	return HAL::Common::Max(HAL::Common::Max(a, b), c);
}


/**
 * Function to calculate the maximum of two inputs.
 * @param [in] a Input A.
 * @param [in] b Input B.
 * @return Larger number.
 */
unsigned int HAL::Common::Max(unsigned int a, unsigned int b)
{
	if (a > b)
	{
		return a;
	}
	else
	{
		return b;
	}
}


/**
 * Function to calculate the maximum of three inputs.
 * @param [in] a Input A.
 * @param [in] b Input B.
 * @param [in] c Input C.
 * @return Largest number.
 */
unsigned int HAL::Common::Max(unsigned int a, unsigned int b, unsigned int c)
{
	return HAL::Common::Max(HAL::Common::Max(a, b), c);
}


/**
 * Function to calculate the maximum of an array of inputs.
 * @param [in] data Data array with numbers.
 * @param [in] size Size of the data array.
 * @return Largest number.
 */
unsigned int HAL::Common::Max(unsigned int* data, unsigned int size)
{
	if (size > 0)
	{
		unsigned int Max_Value = data[0];

		for (unsigned int i = 1;i < size;i++)
		{
			if (data[i] > Max_Value)
			{
				Max_Value = data[i];
			}
		}

		return Max_Value;
	}
	else
	{
		return 0;
	}
}


/**
 * Function to calculate the maximum of an array of inputs.
 * @param [in] data Data array with numbers.
 * @param [in] size Size of the data array.
 * @return Largest number.
 */
int	HAL::Common::Max(int* data, unsigned int size)
{
	if (size > 0)
	{
		int Max_Value = data[0];

		for (unsigned int i = 1;i < size;i++)
		{
			if (data[i] > Max_Value)
			{
				Max_Value = data[i];
			}
		}

		return Max_Value;
	}
	else
	{
		return 0;
	}
}


/**
 * Function to calculate the maximum of two inputs.
 * @param [in] a Input A.
 * @param [in] b Input B.
 * @return Larger number.
 */
float HAL::Common::Max(float a, float b)
{
	if (a > b)
	{
		return a;
	}
	else
	{
		return b;
	}
}


/**
 * Function to calculate the maximum of three inputs.
 * @param [in] a Input A.
 * @param [in] b Input B.
 * @param [in] c Input C.
 * @return Largest number.
 */
float HAL::Common::Max(float a, float b, float c)
{
	return HAL::Common::Max(HAL::Common::Max(a, b), c);
}


/**
 * Function to calculate the maximum of an array of inputs.
 * @param [in] data Data array with numbers.
 * @param [in] size Size of the data array.
 * @return Largest number.
 */
float HAL::Common::Max(float* data, unsigned int size)
{
	if (size > 0)
	{
		float Max_Value = data[0];

		for (unsigned int i = 1;i < size;i++)
		{
			if (data[i] > Max_Value)
			{
				Max_Value = data[i];
			}
		}

		return Max_Value;
	}
	else
	{
		return 0;
	}
}


/**
 * Function to calculate the minimum of two inputs.
 * @param [in] a Input A.
 * @param [in] b Input B.
 * @return Smaller number.
 */
int	HAL::Common::Min(unsigned int a, int b)
{
	if ((int)a > b)
	{
		return b;
	}
	else
	{
		return (int)a;
	}
}


/**
 * Function to calculate the minimum of two inputs.
 * @param [in] a Input A.
 * @param [in] b Input B.
 * @return Smaller number.
 */
int HAL::Common::Min(int a, unsigned int b)
{
	if (a > (int)b)
	{
		return (int)b;
	}
	else
	{
		return a;
	}
}


/**
 * Function to calculate the minimum of two inputs.
 * @param [in] a Input A.
 * @param [in] b Input B.
 * @return Smaller number.
 */
int HAL::Common::Min(int a, int b)
{
	if (a > b)
	{
		return b;
	}
	else
	{
		return a;
	}
}


/**
 * Function to calculate the minimum of three inputs.
 * @param [in] a Input A.
 * @param [in] b Input B.
 * @param [in] c Input C.
 * @return Smallest number.
 */
int	HAL::Common::Min(int a, int b, int c)
{
	return HAL::Common::Min(HAL::Common::Min(a, b), c);
}


/**
 * Function to calculate the minimum of two inputs.
 * @param [in] a Input A.
 * @param [in] b Input B.
 * @return Smaller number.
 */
unsigned int HAL::Common::Min(unsigned int a, unsigned int b)
{
	if (a > b)
	{
		return b;
	}
	else
	{
		return a;
	}
}


/**
 * Function to calculate the minimum of three inputs.
 * @param [in] a Input A.
 * @param [in] b Input B.
 * @param [in] c Input C.
 * @return Smallest number.
 */
unsigned int HAL::Common::Min(unsigned int a, unsigned int b, unsigned int c)
{
	return HAL::Common::Min(HAL::Common::Min(a, b), c);
}


/**
 * Function to calculate the minimum of an array of inputs.
 * @param [in] data Data array with numbers.
 * @param [in] size Size of the data array.
 * @return Smallest number.
 */
unsigned int HAL::Common::Min(unsigned int* data, unsigned int size)
{
	if (size > 0)
	{
		unsigned int Min_Value = data[0];

		for (unsigned int i = 1;i < size;i++)
		{
			if (data[i] < Min_Value)
			{
				Min_Value = data[i];
			}
		}

		return Min_Value;
	}
	else
	{
		return 0;
	}
}


/**
 * Function to calculate the minimum of an array of inputs.
 * @param [in] data Data array with numbers.
 * @param [in] size Size of the data array.
 * @return Smallest number.
 */
int	HAL::Common::Min(int* data, unsigned int size)
{
	if (size > 0)
	{
		int Min_Value = data[0];

		for (unsigned int i = 1;i < size;i++)
		{
			if (data[i] < Min_Value)
			{
				Min_Value = data[i];
			}
		}

		return Min_Value;
	}
	else
	{
		return 0;
	}
}


/**
 * Function to calculate the minimum of two inputs.
 * @param [in] a Input A.
 * @param [in] b Input B.
 * @return Smaller number.
 */
float HAL::Common::Min(float a, float b)
{
	if (a > b)
	{
		return b;
	}
	else
	{
		return a;
	}
}


/**
 * Function to calculate the minimum of three inputs.
 * @param [in] a Input A.
 * @param [in] b Input B.
 * @param [in] c Input C.
 * @return Smallest number.
 */
float HAL::Common::Min(float a, float b, float c)
{
	return HAL::Common::Min(HAL::Common::Min(a, b), c);
}


/**
 * Function to calculate the minimum of an array of inputs.
 * @param [in] data Data array with numbers.
 * @param [in] size Size of the data array.
 * @return Smallest number.
 */
float HAL::Common::Min(float* data, unsigned int size)
{
	if (size > 0)
	{
		float Min_Value = data[0];

		for (unsigned int i = 1;i < size;i++)
		{
			if (data[i] < Min_Value)
			{
				Min_Value = data[i];
			}
		}

		return Min_Value;
	}
	else
	{
		return 0;
	}
}


/**
 * Function to set the current thread to sleep.
 * @param [in] usec Sleep duration in microseconds.
 */
void HAL::Common::u_Sleep(unsigned int usec)
{
	#ifdef __linux__
		usleep(usec);
	#elif _WIN32
		HANDLE			SleepTimer;
		LARGE_INTEGER	SleepTime;

		SleepTime.QuadPart = -(10 * (int)usec);

		SleepTimer = CreateWaitableTimer(NULL, TRUE, NULL);
		SetWaitableTimer(SleepTimer, &SleepTime, 0, NULL, NULL, 0);

		WaitForSingleObject(SleepTimer, INFINITE);

		CloseHandle(SleepTimer);
	#endif
}