/*
 * Help.c
 *
 * Created: 27.12.2015 15:22:20
 *  Author: Chris
 */ 

#include "Help.h"

int Char_To_Number(char input)
{
	int Result_Number = 0;
	
		 if(input>='a' && input<='f') { Result_Number = (input-'a'+10); }
	else if(input>='A' && input<='F') { Result_Number = (input-'A'+10); }
	else if(input>='0' && input<='9') { Result_Number = (input-'0');	}
	
	return Result_Number;
}

int CharArray_To_Number(volatile char input[], int length)
{
	int i;
	int Result_Number = 0;
	
	for(i=0;i<length;i++)
	{
		Result_Number += (Char_To_Number(input[i]) << (4*(length-1-i)));
	}
	
	return Result_Number;
}