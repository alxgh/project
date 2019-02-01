#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <time.h>
#include <Windows.h>

#include "include/helper.h"
#include "include/ui.h"
#include "include/bmp.h"
#include <bmpio.h>
#define KEY_UP        72
#define KEY_DOWN      80
#define KEY_LEFT      75
#define KEY_RIGHT     77
#define KEY_ENTER     13
#define KEY_BACKSPACE 8

#define CASE_LEARNING 1
#define CASE_DETECT   2
#define CASE_EXIT     3

#define COLOR_BLUE   1
#define COLOR_GREEN  2
#define COLOR_CYAN   3
#define COLOR_RED    4
#define COLOR_PURPLE 5
#define COLOR_YELLOW 6
#define COLOR_WHITE  7
#define COLOR_GRAY   8
#define COLOR_ORANGE 12

#define WORDS_COUNT 7

unsigned char pic[2000][2000][3], 
	cropped[2000][2000][3],
	samples[41][2000][2000][3],
	sample[7][2000][2000][3],
	final[2000][2000][3];

long average[500][500][3];

char list[7][2] = {
	{'A', 32},
	{'B', 41},
	{'E', 32},
	{'I', 15},
	{'O', 25},
	{'R', 17},
	{'S', 20}
};

int current_option = 1,
    width = 120,
	height = 800,
	learning_complete = 0;

int pic_center[2];

char title[8][2] = {
	{'R', COLOR_BLUE},
	{'E', COLOR_CYAN},
	{'A', COLOR_GREEN},
	{'B', COLOR_PURPLE},
	{'I', COLOR_GRAY},
	{'S', COLOR_YELLOW},
	{'O', COLOR_WHITE},
};

const int main_color = COLOR_YELLOW;
const int secondary_color = COLOR_WHITE;

typedef struct{
	char c;
	int n;
	int x;
	int y;
} Word;

void crop(unsigned char given[2000][2000][3], unsigned char newp[2000][2000][3], int *height, int *width);

void resize(unsigned char given[2000][2000][3], unsigned char scaled[2000][2000][3], int height, int width, double newh, double neww);

void make_average();

char detect_word(unsigned char given[2000][2000][3]);

void grid(int vertical_lines[12], int horizontal_lines[12], int *vc, int *hc, int height, int width);

int find_the_word(Word words[12][12], int vc, int hc, const char *word);

void printEdges();

void printFullLine();

void printEdge();

void printInCenter();

void header();

void footer();

void printMainMenu();

void learningPart();

void detectPart();

void mainMenu();

int main()
{
	int h, w;
	hideCursor();
	readBMP("main.bmp", &w, &h, pic);
	for(int i = 0; i < 30; i++) {
		for(int j = 0; j < 60; j++) {	
			setTextColor((pic[i][j][0] ), 0);
			printf("%c%c", ' ', ' ');
		}
	}
	setTextColor(main_color, 0);
	gotoxy(0,0);
	
	Sleep(1000);
	mainMenu();
}
void printMainMenu()
{
	header();
	if(current_option == CASE_LEARNING) {
		printInCenter("Start Learning", secondary_color, main_color);
	} else {
		printInCenter("Start Learning", main_color, 0);	
	}
	
	setTextColor(main_color,0);
	if(current_option == CASE_DETECT) {
		printInCenter("Detect", secondary_color, main_color);
	} else {
		printInCenter("Detect", main_color, 0);	
	}
	
	setTextColor(main_color,0);
	if(current_option == CASE_EXIT) {
		printInCenter("Exit", secondary_color, main_color);
	} else {
		printInCenter("Exit", main_color, 0);	
	}
	setTextColor(main_color,0);
	footer();
}


void learningPart(){
	int s =  8;
	header();
	printInCenter("Making average files...", main_color, 0);
	printEdges(14);
	footer();
	
	make_average();
	
	gotoxy(s+(7*2), 0);
	learning_complete = 1;
	printInCenter("Press any key to continue", main_color, 0);
	getch();
	mainMenu();
}

void detectPart(){
	
	int l = 0, s = 9, hc = 0, vc = 0;
	int width, height, vertical_lines[12] = {0}, horizontal_lines[12] = {0}, wc, fwc;
	char entery[100], output[100], word[13] = "boarsreorasi", e, t;
	FILE *file, *result;
	Word words[12][12];
	
	
	header();
	printInCenter("Enter the path of entery file:", main_color, 0);
	printEdges(15);
	footer();
	
	while((t = getch()) != KEY_ENTER) {
		if(t == KEY_BACKSPACE) {
			if(l == 0) continue;
			entery[--l] = NULL;
		} else {
			entery[l++] = t;	
		}
		gotoxy(7,0);
		printInCenter(entery, COLOR_ORANGE, 0);
	}
	
	readBMP(entery, &width, &height, pic);	
	
	grid(vertical_lines, horizontal_lines, &vc, &hc, height, width);

	int p = 0, o = 0, n = 0;
	
	for(int i = 0; i < hc; i++) {
		gotoxy(9+((12-hc) / 2)+(i), 47 + ((12-vc)));
		for(int j = 0; j < vc; j++) {
			free_arr(samples[0]);
			p = 0;
			o = 0;
			for(int x = horizontal_lines[i]; x < (i == hc-1 ? height : horizontal_lines[i+1]); x++) {
				o = 0;
				for(int k = vertical_lines[j]; k < (j == vc-1 ? width : vertical_lines[j+1]); k++) {
					copy(pic[x][k], cropped[p][o++]);
				}
				p++;
			}

			crop(cropped, samples[0], &p, &o);
			free_arr(cropped);
			resize(samples[0], cropped, p, o, 500, 500);

			words[i][j].c = detect_word(cropped);
			words[i][j].x = vertical_lines[j] + pic_center[1];
			words[i][j].y = horizontal_lines[i] + pic_center[0];

			printf("%c ", words[i][j].c);

			free_arr(cropped);
			n++;
		}
	}

	file   = fopen("dictionary.txt",    "r");
	result = fopen("result/reuslt.txt", "w");
	fscanf(file, "%d", &wc);
	
	for(int i = 0; i < wc; i++)
	{
		fscanf(file, "%s", word);
		
		uppercase(word);

		readBMP(entery, &width, &height, pic);
		
		fwc =  find_the_word(words, vc, hc, word);
		reverse(word);
		fwc += find_the_word(words, vc, hc, word);
		reverse(word);
		sprintf(output, "result/final-%s.bmp", word);
		saveBMP(pic, width, height, output);

		fprintf(result, "%s: %d\n", word, fwc);
	}
	fclose(result);
	fclose(file);
	gotoxy(9+13, 0);
	printInCenter("The results are in \"result\" folder.", main_color, 0);
	gotoxy(9+14, 0);
	printInCenter("Press any key to continue...", main_color, 0);
	getch();
	mainMenu();
}

void mainMenu()
{
	printMainMenu();
	int loop = 1;
	while(loop) {
		switch(getch()) {
			case KEY_UP:
				if(current_option > 1) 
				{
					if(learning_complete == 0) current_option = 1;
					else current_option--;
					printMainMenu();
				}
				break;
			
			case KEY_DOWN:
				if(current_option < 3) {
					if(learning_complete == 0) current_option = 3;
					else current_option++;
					printMainMenu();	
				}
				break;
			
			case KEY_ENTER:
				loop = 0;
				switch(current_option) {
					case CASE_LEARNING:
						learningPart();
						break;
					
					case CASE_DETECT:
						detectPart();
						break;
					
					case CASE_EXIT:
						break;
				}
				break;
		}
	}
}

int find_the_word(Word words[12][12], int vc, int hc, const char* word)
{
	int n = 0;
	int word_len = strlen(word);
	int find = 0, y, x;
	double m;
	for(int i = 0; i < hc; i++) {
		for(int j = 0; j < vc-word_len+1; j++) {
			if(words[i][j].c == word[0]) {
				find = 1;
				for(int k = 1; k < word_len; k++) {
					if(words[i][j+k].c != word[k]) {
						find = 0;
					}
				}
				if(find == 1) {
					for(int p = 0; p < word_len-1; p++) {
						m = (double)(words[i][j+p+1].y - words[i][j+p].y) / (words[i][j+p+1].x - words[i][j+p].x);
						for(x = words[i][j+p].x; x <= words[i][j+p+1].x; x++) {
							y = ceil((m*(x - words[i][j+p].x)) + words[i][j+p].y);
							pic[y][x][0] = 255;
							pic[y][x][1] = 0;
							pic[y][x][2] = 0;
						}
					}
					n++;
					find = 0;
				}
			}
		}
	}
	for(int j = 0; j < vc; j++) {
		for(int i = 0; i < hc-word_len+1; i++) {
			if(words[i][j].c == word[0]) {
				find = 1;
				for(int k = 1; k < word_len; k++) {
					if(words[i+k][j].c != word[k]) {
						find = 0;
					}
				}
				if(find == 1) {
					for(int p = 0; p < word_len-1; p++) {
						m = (double)(words[i+p+1][j].y - words[i+p][j].y) / (words[i+p+1][j].x - words[i+p][j].x);
						for(y = words[i+p][j].y; y <= words[i+p+1][j].y; y++) {
							x = ceil(((y - words[i+p][j].y) / m) + words[i+p][j].x);
							pic[y][x][0] = 255;
							pic[y][x][1] = 0;
							pic[y][x][2] = 0;
						}
					}
					n++;
					find = 0;
				}
			}
		}
	}

	return n;
}

void grid(int vertical_lines[12], int horizontal_lines[12], int *vc, int *hc, int height, int width)
{
	int n = 0, onFullWhite = 0, whiteCount = 0;
	for(int i = 0; i < height; i++) {
		whiteCount = 0;
		for(int j = 0; j < width; j++) {
			if((int)pic[i][j][0] == 255 ||  (int)pic[i][j][1] == 255 ||  (int)pic[i][j][2] == 255) whiteCount++;
			if(((int)pic[i][j][0] != 255 ||  (int)pic[i][j][1] != 255 ||  (int)pic[i][j][2] != 255) && onFullWhite) {
				onFullWhite = 0;
				horizontal_lines[n++] = i;
				*vc += 1;
			}
		}
		if(whiteCount == width) onFullWhite = 1;
	}
	n = 0;
	for(int j = 0; j < width; j++) {
		whiteCount = 0;
		for(int i = 0; i < height; i++) {
			if((int)pic[i][j][0] == 255 ||  (int)pic[i][j][1] == 255 ||  (int)pic[i][j][2] == 255) whiteCount++;
			if(((int)pic[i][j][0] != 255 ||  (int)pic[i][j][1] != 255 ||  (int)pic[i][j][2] != 255) && onFullWhite) {
				onFullWhite = 0;
				vertical_lines[n++] = j;
				*hc += 1;
			}
		}
		if(whiteCount == height) onFullWhite = 1;
	}
}

char detect_word(unsigned char given[2000][2000][3])
{
	
	long long diff = 0;
	long long min;
	int word;
	for(int  i = 0; i < WORDS_COUNT; i++) {
		for(int k = 0; k < 500; k++) {
			for(int x = 0; x < 500; x++) {
				diff += abs(given[k][x][0] - sample[i][k][x][0]);
				diff += abs(given[k][x][1] - sample[i][k][x][1]);
				diff += abs(given[k][x][2] - sample[i][k][x][2]);
			}
		}
		if(diff < min || i == 0) {
			min = diff;
			word = i;
		}
		diff = 0;
	}
	return list[word][0];
}

void make_average()
{
	int s =  8, q = 0, size = 100, width, height, n = 0, current_char = 0;
	FILE *f = fopen("data.txt", "r");
	char address[500], sadd[500];
	
	while(fscanf(f, "%s", &address) != EOF) {
		if(strlen(address) == 1) {
			
			for(int x = 0; x < q; x++) {
				printf("%c", 219);
			}
			
			gotoxy(s+((address[0] - 48)*2), 9);
			printf("%c ", list[address[0] - 48][0]);
			q = size - (floor((double)size / list[address[0] - 48][1])*list[address[0] - 48][1]);
			if(current_char >= 0 && n != 0) {
				for(int i =0; i < 500; i++) {
					for(int j = 0; j < 500; j++) {
						for(int k = 0; k < list[current_char][1]; k++) {
							for(int o = 0; o < 3; o++)
								average[i][j][o] = 0;
						}
					}
				}
				for(int i =0; i < 500; i++) {
					for(int j = 0; j < 500; j++) {
						for(int k = 0; k < list[current_char][1]; k++) {
							for(int o = 0; o < 3; o++)
								average[i][j][o] += samples[k][i][j][o];
						}
						
						for(int o = 0; o < 3; o++)
							average[i][j][o] /= list[current_char][1];
					}
				}
				for(int i =0; i < 500; i++) {
					for(int j = 0; j < 500; j++) {
						for(int o = 0; o < 3; o++) {
							sample[current_char][i][j][o] = average[i][j][o];
						}
					}
				}
				sprintf(sadd, "average/%c.bmp", list[current_char][0]);

				saveBMP(sample[current_char], 500, 500, sadd);
				
			}
			
			n = 0;
			current_char = address[0] - 48;
			continue;
		}
		
		readBMP(address, &width, &height, pic);
		crop(pic, cropped, &height, &width);
		free_arr(pic);
		
		resize(cropped, samples[n], height, width, 500, 500);
		free_arr(cropped);
		
		for(int x = 0; x < floor((double)size/list[current_char][1]); x++) {
			printf("%c", 219);
		}
		
		n++;
	}
	fclose(f);
}

void resize(unsigned char given[2000][2000][3], unsigned char scaled[2000][2000][3], int height, int width, double newh, double neww)
{
	int x = 0, k = 0, t;
	unsigned char heightColor[3] = {0}, widthColor[3] = {0}, finalColor[3] = {0};
	double heightFactor, widthFactor, heightHolder, widthHolder, tb;
	heightHolder = heightFactor = newh / height;
	widthHolder  = widthFactor  = neww / width;
	
	for(int i = 0; i < newh; i++)
	{
		k = 0;
		for(int j = 0; j < neww; j++)
		{
//			printf("%d-%d-%d-%d\n", x, k, i, j);
			for(int z = 0; z < 3; z++) {
				widthColor[z] = 0;
				heightColor[z] = 0;
			}
			if(widthHolder >= 1) {
				copy(given[x][k], widthColor);
			} else {
				get_mix(given[x][k], given[x][k+1], widthHolder, 1 - widthHolder, widthColor);
			}
			if(widthHolder <= 1) {
				widthHolder = widthFactor - (1 - widthHolder);
				k++;
			} else {
				widthHolder -= 1;
			}
			
			if(heightHolder >= 1) {
				copy(given[x][k], heightColor);
			} else {
				get_mix(given[x][k], given[x+1][k], heightHolder, 1 - heightHolder, heightColor);
			}
			
			get_mix(heightColor, widthColor, 0.5, 0.5, finalColor);
			copy(finalColor, scaled[i][j]);		
		}
		if(heightHolder <= 1) {
			heightHolder = heightFactor - (1 - heightHolder);
			x++;
		} else {
			heightHolder -= 1;
		}
		
	}
}

void crop(unsigned char given[2000][2000][3], unsigned char newp[2000][2000][3], int *height, int *width)
{
	int top, right, left, bottom, f = 0, fullWhite = 0;
	
	for(int i = 0; i < *height, f == 0; i++) {
		for(int j = 0; j < *width; j++) {
			if((int)given[i][j][0] != 255 ||  (int)given[i][j][1] != 255 ||  (int)given[i][j][2] != 255) {
				f = 1;
				top = bottom = i;
				right = left = j;
				break;
			}
		}
	}
	for(int i = top; i < *height; i++) {
		if(fullWhite == *width) break;
		fullWhite = 0;
		for(int j = 0; j < *width; j++) {
			if(given[i][j][0] == 255 && given[i][j][1] == 255 && given[i][j][2] == 255) fullWhite++;
			if((int)given[i][j][0] != 255 ||  (int)given[i][j][1] != 255 ||  (int)given[i][j][2] != 255) {
				if(j < left) left = j;
				if(j > right) right = j;
			}
		}
		if(fullWhite == *width) bottom = i-1;
	}
	if(bottom == top) {
		bottom = *height - 1;
	}
	pic_center[0] = top + ((bottom-top) / 2);
	pic_center[1] = left + ((right-left) / 2);
	for(int i = top; i <= bottom; i++) {
		for(int j = left; j <= right; j++) {
			copy(given[i][j], newp[i - top][j - left]);
		}
	}
	*height = bottom - top + 1;
	*width = right - left + 1;
}

void printEdges(int times)
{
	for(int i = 0; i < times; i++) printf("\n%c%*c%c", 219, width-2, ' ', 219);
}

void printFullLine()
{
	for (int i = 0; i < width; i++) printf("%c", 219);
}

void printEdge()
{
	printf("%c", 219);
}

void printInCenter(const char* text, int textColor, int bgColor)
{
	int size = strlen(text);
	int left = width - 2;
	if(bgColor != 0) setTextColor(bgColor, 0);
	else setTextColor(main_color, 0);
	printEdge();
	setTextColor(textColor, bgColor);
	printf("%*c", (int) ((left/2)-ceil((double)size/2)), ' ');
	printf("%s", text);
	printf("%*c", (int)((left/2)-floor((double)size/2)), ' ');
	if(bgColor != 0) setTextColor(bgColor, 0);
	else setTextColor(main_color, 0);
	printEdge();
}

void header()
{
	clear();
	setTextColor(main_color, 0);
	printFullLine();
	printEdges(1);
	
	printf("\n%c%*c", 219, ((width-2)/2)-4, ' ');
	for(int i = 0; i < 7; i++) {
		setTextColor((int)title[i][1], 0);
		printf("%c", title[i][0]);
	}
	setTextColor(main_color, 0);
	printf("%*c%c", ((width-2)/2)-3, ' ', 219);
	printEdges(3);
}

void footer()
{
	setTextColor(main_color,0);
	printEdges(3);
	printFullLine();
}
