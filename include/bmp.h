#include <stdio.h>

unsigned char data[2000*2000*3];
extern void readbmp(const char *path, int *width, int *height, unsigned char Pic[2000][2000][3]);
extern void savebmp(unsigned char Pic[2000][2000][3], int width, int height, const char *path);