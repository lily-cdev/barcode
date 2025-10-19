#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define bool _Bool
#define true 1
#define false 0

#define OUTPUTNAME "yield.bmp"
#define DATAOFFSET 62

extern const unsigned char Black[4];
extern const unsigned char White[4];
extern uint8_t Adding_Quietzone;

inline char To_Lower(const char Input) {
	if (Input >= 'A' && Input <= 'Z') {
		return Input - ('A' - 'a');
	}
	return Input;
}

inline bool Is_Digit(const char Input) {
	return Input >= '0' && Input <= '9';
}

inline void Write_BMP(const uint32_t Width, unsigned char* Row) {
	uint32_t Height = Width / 4;
	if (Width < 75) {
		Height = max(75, Height);
	}
	uint32_t Row_Size = ((Width + 31) / 32) * 4;
	uint32_t Image_Size = Row_Size * Height;
	uint32_t File_Size = Image_Size + DATAOFFSET;
	uint32_t Data_Offset = DATAOFFSET;
	FILE* Yield = NULL;
	errno_t Check = fopen_s(&Yield, OUTPUTNAME, "wb");
	if (Check != 0) {
		return;
	}
	fwrite("BM", 1, 2, Yield);
	fwrite(&File_Size, 4, 1, Yield);
	const uint16_t Zero2 = 0;
	fwrite(&Zero2, 2, 1, Yield);
	fwrite(&Zero2, 2, 1, Yield);
	fwrite(&Data_Offset, 4, 1, Yield);
	const uint32_t DIB_Size = 40;
	fwrite(&DIB_Size, 4, 1, Yield);
	fwrite(&Width, 4, 1, Yield);
	fwrite(&Height, 4, 1, Yield);
	const uint16_t One2 = 1;
	fwrite(&One2, 2, 1, Yield);
	fwrite(&One2, 2, 1, Yield);
	const uint32_t Zero4 = 0;
	fwrite(&Zero4, 4, 1, Yield);
	fwrite(&Image_Size, 4, 1, Yield);
	const int32_t PPM = 11811;
	fwrite(&PPM, 4, 1, Yield);
	fwrite(&PPM, 4, 1, Yield);
	fwrite(&Zero4, 4, 1, Yield);
	fwrite(&Zero4, 4, 1, Yield);
	fwrite(White, 1, 4, Yield);
	fwrite(Black, 1, 4, Yield);
	for (int Counter = 0; Counter < Height; Counter++) {
		fwrite(Row, 1, Row_Size, Yield);
	}
	fclose(Yield);
}

void Generate39(char* Input);
void Generate128(char* Input);