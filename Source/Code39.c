#include "Wrapper.h"

#define CHARACTERS 43
#define INVALID 250
#define STRIPCODELENGTH 9
#define SUBCODELENGTH 10
#define WIDEMULTIPLIER 3

enum Bar {
	SW,
	LW,
	SB,
	LB
};

const char Allowed[CHARACTERS] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
	'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
	'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '-', '.',
	'$', '/', '+', '%', ' ' };

const uint8_t Codes[CHARACTERS][STRIPCODELENGTH] = {
	{ SB, SW, SB, LW, LB, SW, LB, SW, SB }, { LB, SW, SB, LW, SB, SW, SB, SW, LB },
	{ SB, SW, LB, LW, SB, SW, SB, SW, LB }, { LB, SW, LB, LW, SB, SW, SB, SW, SB },
	{ SB, SW, SB, LW, LB, SW, SB, SW, LB }, { LB, SW, SB, LW, LB, SW, SB, SW, SB },
	{ SB, SW, LB, LW, LB, SW, SB, SW, SB }, { SB, SW, SB, LW, SB, SW, LB, SW, LB },
	{ LB, SW, SB, LW, SB, SW, LB, SW, SB }, { SB, SW, LB, LW, SB, SW, LB, SW, SB },
	{ LB, SW, SB, SW, SB, LW, SB, SW, LB }, { SB, SW, LB, SW, SB, LW, SB, SW, LB },
	{ LB, SW, LB, SW, SB, LW, SB, SW, SB }, { SB, SW, SB, SW, LB, LW, SB, SW, LB },
	{ LB, SW, SB, SW, LB, LW, SB, SW, SB }, { SB, SW, LB, SW, LB, LW, SB, SW, SB },
	{ SB, SW, SB, SW, SB, LW, LB, SW, LB }, { LB, SW, SB, SW, SB, LW, LB, SW, SB },
	{ SB, SW, LB, SW, SB, LW, LB, SW, SB }, { SB, SW, SB, SW, LB, LW, LB, SW, SB },
	{ LB, SW, SB, SW, SB, SW, SB, LW, LB }, { SB, SW, LB, SW, SB, SW, SB, LW, LB },
	{ LB, SW, LB, SW, SB, SW, SB, LW, SB }, { SB, SW, SB, SW, LB, SW, SB, LW, LB },
	{ LB, SW, SB, SW, LB, SW, SB, LW, SB }, { SB, SW, LB, SW, LB, SW, SB, LW, SB },
	{ SB, SW, SB, SW, SB, SW, LB, LW, LB }, { LB, SW, SB, SW, SB, SW, LB, LW, SB },
	{ SB, SW, LB, SW, SB, SW, LB, LW, SB }, { SB, SW, SB, SW, LB, SW, LB, LW, SB },
	{ LB, LW, SB, SW, SB, SW, SB, SW, LB }, { SB, LW, LB, SW, SB, SW, SB, SW, LB },
	{ LB, LW, LB, SW, SB, SW, SB, SW, SB }, { SB, LW, SB, SW, LB, SW, SB, SW, LB },
	{ LB, LW, SB, SW, LB, SW, SB, SW, SB }, { SB, LW, LB, SW, LB, SW, SB, SW, SB },
	{ SB, LW, SB, SW, SB, SW, LB, SW, LB }, { LB, LW, SB, SW, SB, SW, LB, SW, SB },
	{ SB, LW, SB, LW, SB, LW, SB, SW, SB }, { SB, LW, SB, LW, SB, SW, SB, LW, SB },
	{ SB, LW, SB, SW, SB, LW, SB, LW, SB }, { SB, SW, SB, LW, SB, LW, SB, LW, SB },
	{ SB, LW, LB, SW, SB, SW, LB, SW, SB }
};

const uint8_t Cap[STRIPCODELENGTH] = { SB, LW, SB, SW, LB, SW, LB, SW, SB };

uint8_t Check_Validity(const char Input) {
	for (uint8_t Counter = 0; Counter < CHARACTERS; Counter++) {
		if (To_Lower(Input) == Allowed[Counter]) {
			return Counter;
		}
	}
	return INVALID;
}

uint8_t* Get_Subcode(char Input) {
	uint8_t Candidate = Check_Validity(Input);
	static uint8_t Yield[SUBCODELENGTH];
	for (uint8_t Counter = 0; Counter < STRIPCODELENGTH; Counter++) {
		Yield[Counter] = Codes[Candidate][Counter];
	}
	Yield[STRIPCODELENGTH] = SW;
	return Yield;
}

void Generate39(char* Input) {
	uint32_t Digits = 1;
	uint32_t Raw_Digits = 0;
	while (Input[Raw_Digits] != '\0') {
		if (Check_Validity(Input[Raw_Digits]) != INVALID) {
			Digits++;
		}
		Raw_Digits++;
	}
	uint8_t* Content = malloc(sizeof(uint8_t) * SUBCODELENGTH * (Digits + 1));
	for (uint8_t Counter = 0; Counter < STRIPCODELENGTH; Counter++) {
		Content[Counter] = Cap[Counter];
	}
	Content[STRIPCODELENGTH] = SW;
	uint32_t Index = 1;
	for (uint32_t Counter1 = 0; Counter1 <= Raw_Digits; Counter1++) {
		if (Check_Validity(Input[Counter1]) != INVALID) {
			for (uint8_t Counter2 = 0; Counter2 < SUBCODELENGTH; Counter2++) {
				Content[(Index * SUBCODELENGTH) + Counter2] =
					Get_Subcode(Input[Counter1])[Counter2];
			}
			Index++;
		}
	}
	for (uint8_t Counter = 0; Counter < STRIPCODELENGTH; Counter++) {
		Content[(Index * SUBCODELENGTH) + Counter] = Cap[Counter];
	}
	uint32_t Width = 0;
	for (uint32_t Counter = 0; Counter < (Digits * SUBCODELENGTH) - 1; Counter++) {
		if (Content[Counter] == LW || Content[Counter] == LB) {
			Width += WIDEMULTIPLIER;
		} else {
			Width++;
		}
	}
	unsigned char* Data_Row = calloc((((Width + 7) / 8) + 3) & ~3, sizeof(char));
	Index = 0;
	uint8_t Marker;
	uint8_t Backlog = 0;
	for (uint32_t Counter = 0; Counter < Width; Counter++) {
		if (Backlog == 0) {
			if (Content[Index] == SW || Content[Index] == LW) {
				Marker = 1;
			} else {
				Marker = 0;
			}
			if (Content[Index] == LW || Content[Index] == LB) {
				Backlog = WIDEMULTIPLIER - 1;
			}
			Index++;
		} else {
			Backlog--;
		}
		uint32_t Byte_Index = Counter / 8;
		uint8_t Bit_Index = 7 - (Counter % 8);
		if (Marker == 1) {
			Data_Row[Byte_Index] &= ~(1 << Bit_Index);
		} else {
			Data_Row[Byte_Index] |= (1 << Bit_Index);
		}
	}
	Write_BMP(Width, Data_Row);
	free(Data_Row);
	free(Content);
}