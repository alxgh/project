#include "helper.h"

void uppercase(char *word)
{
	for(int i = 0; word[i]; i++)
	{
		word[i] = toupper(word[i]);
	}
}

void free_arr(unsigned char given[2000][2000][3])
{
	for(int i = 0; i < 2000; i++) {
		for(int j = 0; j < 2000; j++) {
			for(int o = 0; o< 3; o++) {
				given[i][j][o] = 0;
			}
		}
	}
}

void copy(unsigned char s[3], unsigned char n[3])
{
	
	for(int i = 0; i < 3; i++)
	{
		n[i] = s[i];
	}
	
}

void get_mix(unsigned char firstColor[3], unsigned char secondColor[3], double fP, double sP, unsigned char finalColor[3])
{
	for(int i = 0; i < 3; i++)
	{
		finalColor[i] = ((double)firstColor[i] * fP) + ((double)secondColor[i] * sP);
	}
}

void clear()
{
	#if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
        system("clear");
    #endif

    #if defined(_WIN32) || defined(_WIN64)
        system("cls");
    #endif
}

void reverse(char* str) 
{ 
    int n = strlen(str); 
	char c;
    // Swap character starting from two 
    // corners 
    for (int i = 0; i < n / 2; i++) {
		c = str[i];
		str[i] = str[n - i -1];
		str[n - i -1] = c;
	}
} 