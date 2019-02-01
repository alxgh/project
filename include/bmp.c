#include "bmp.h"

extern void readbmp(const char *path, int *w, int *h, unsigned char Pic[2000][2000][3])
{
	int width, height;
	FILE* file = fopen(path, "rb");
	unsigned char info[54];
	fread(info, sizeof(unsigned char), 54, file);
	
	width  = *(int *)&info[18];
	height = *(int *)&info[22];
	for(long long i=0; i< 2000*2000*3; i++)
	{
	    data[i] = 0;
    }
	int size = 3 * (height) * (width);
	fread(data, sizeof(unsigned char), size, file);

	fclose(file);
	for(int i = 0; i < height; i++) {
		for(int j = 0; j < width; j++) {
			for(int k = 0; k < 3; k++) {
				Pic[height - i - 1][j][k] = data[3 * (i*(width) + j) + k];
			}
		}
	}
	*w = width;
	*h = height;
}

extern void savebmp(unsigned char Pic[2000][2000][3], int width, int height, const char *path)
{
	FILE *file;
	
	
	int filesize = 54 + 3 * width * height; 
	for(int i=0; i< 2000; i++)
	{
	    for(int j=0; j < 2000; j++)
	    {
            for(int k = 0; k < 3; k++) {
				data[3 * (i*width + j) + k] = 0;
			}
        }
    }
	for(int i=0; i< height; i++)
	{
	    for(int j=0; j < width; j++)
	    {
	    	for(int k = 0; k < 3; k++) {
				data[3 * (i*width + j) + k] = Pic[i][j][k];
			}
	    }
	}
	unsigned char bmpfileheader[14] = {'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0};
	unsigned char bmpinfoheader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0};
	unsigned char bmppad[3] = {0,0,0};
	
	bmpfileheader[ 2] = (unsigned char)(filesize    );
	bmpfileheader[ 3] = (unsigned char)(filesize>> 8);
	bmpfileheader[ 4] = (unsigned char)(filesize>>16);
	bmpfileheader[ 5] = (unsigned char)(filesize>>24);
	
	bmpinfoheader[ 4] = (unsigned char)(width    );
	bmpinfoheader[ 5] = (unsigned char)(width >> 8);
	bmpinfoheader[ 6] = (unsigned char)(width >> 16);
	bmpinfoheader[ 7] = (unsigned char)(width >> 24);
	bmpinfoheader[ 8] = (unsigned char)(height    );
	bmpinfoheader[ 9] = (unsigned char)(height >> 8);
	bmpinfoheader[10] = (unsigned char)(height >> 16);
	bmpinfoheader[11] = (unsigned char)(height >> 24);
	
	file = fopen(path, "wb");
	fwrite(bmpfileheader, 1, 14, file);
	fwrite(bmpinfoheader, 1, 40, file);
	
	for(int i = 0; i < height; i++)
	{
	    fwrite(data + (width*(height - i - 1)*3), 3, width, file);
	    fwrite(bmppad, 1, (4 - (width * 3) % 4) % 4, file);
	}
	
	fclose(file);
}
