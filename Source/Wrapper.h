#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define bool _Bool
#define true 1
#define false 0

#define OUTPUTNAME "yield.bmp"
#define DATAOFFSET 62
#define DEFAULTSIZE 1
#define POSSIBLEFLAGS 4
#define QUIETZONEDEFAULT 17

extern const unsigned char Black[4];
extern const unsigned char White[4];
extern uint8_t Adding_Quietzone;
extern uint8_t Quietzone_Width;

inline char To_Lower(const char Input) {
	if (Input >= 'A' && Input <= 'Z') {
		return Input - ('A' - 'a');
	}
	return Input;
}

inline bool Is_Digit(const char Input) {
	return Input >= '0' && Input <= '9';
}

inline uint8_t To_UInt8(const char Input) {
	return (uint8_t)(Input) - (uint8_t)'0';
}

void Write_BMP(const uint32_t Width, unsigned char* Row);
void Generate39(char* Input);
void Generate128(char* Input);
uint8_t GenerateITF(char* Input);