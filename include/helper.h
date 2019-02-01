#include <ctype.h>
#include <stdlib.h>
#include <string.h>

void copy(unsigned char s[3], unsigned char n[3]);

void free_arr(unsigned char given[2000][2000][3]);

void clear();

void uppercase(char *word);

void get_mix(unsigned char firstColor[3], unsigned char secondColor[3], double fP, double sP, unsigned char finalColor[3]);

void reverse(char* str);