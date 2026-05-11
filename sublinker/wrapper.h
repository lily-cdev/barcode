#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define bool _Bool
#define true 1
#define false 0

#define output_name "yield.bmp"
#define data_offset 62
#define default_size 1
#define flag_ct 5
#define quietzone_default 17

#define max(a, b) (((a) > (b)) ? (a) : (b))
#define min(a, b) (((a) < (b)) ? (a) : (b))

extern const unsigned char Black[4];
extern const unsigned char White[4];
extern uint8_t Adding_Quietzone;
extern uint8_t Quietzone_Width;

char To_Lower(const char Input);
bool Is_Digit(const char Input);
uint8_t To_UInt8(const char Input);

void Write_BMP(const uint32_t Width, bool* Row);
void Write_2D(uint32_t Width, uint32_t Height, bool** Content);
void Generate39(char* Input);
void Generate128(char* Input);
uint8_t GenerateITF(char* Input);
uint8_t GenerateDM(char* Input);