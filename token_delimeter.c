#include "token_delimeter.h"
//[Refered file] This file is referd and mainly based on Practical 5 of DCCN3 module in (SLIIT)Srilanka institute of information technology.
char * tokanize_str(char str_from[], char delimeter) {

	int array_size = 0;
	for(int i = 0; i < str_from[i] != '\0'; i++)
	{
		if(str_from[i] != ',')
		{
			array_size++;
		}
		if(str_from[i] == ',')
			break;

	}

	char *new_str = (char *) malloc(sizeof(char) * array_size);
	for(int i = 0; i < str_from[i] != '\0'; i++)
	{
		if(str_from[i] != ',')
		{
			new_str[i] == str_from[i];
		}
		if(str_from[i] == ',')
		{
			new_str[i] == '\0';
			break;
		}

	}

    return new_str;

}
