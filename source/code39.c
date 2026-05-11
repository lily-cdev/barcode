#include "../sublinker/wrapper.h"

#define characters 43
#define invalid 250
#define stripcode_len 9
#define subcode_len 10
#define wide_multiplier 3

enum Bar {
	SW,
	LW,
	SB,
	LB
};

const char Allowed[characters] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
	'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
	'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '-', '.',
	'$', '/', '+', '%', ' ' };

const uint8_t Codes[characters][stripcode_len] = {
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

const uint8_t Cap[stripcode_len] = { SB, LW, SB, SW, LB, SW, LB, SW, SB };

uint8_t Check_Validity(const char Input) {
	for (uint8_t C1 = 0; C1 < characters; C1++) {
		if (To_Lower(Input) == Allowed[C1]) {
			return C1;
		}
	}
	return invalid;
}

uint8_t* Get_Subcode(char Input) {
	uint8_t Candidate = Check_Validity(Input);
	static uint8_t Yield[subcode_len];
	for (uint8_t C1 = 0; C1 < stripcode_len; C1++) {
		Yield[C1] = Codes[Candidate][C1];
	}
	Yield[stripcode_len] = SW;
	return Yield;
}

void Generate39(char* Input) {
	uint32_t Digits = 1;
	uint32_t Raw_Digits = 0;
	while (Input[Raw_Digits] != '\0') {
		if (Check_Validity(Input[Raw_Digits]) != invalid) {
			Digits++;
		}
		Raw_Digits++;
	}
	uint8_t* Content = malloc(sizeof(uint8_t) * subcode_len * (Digits + 1));
	for (uint8_t C1 = 0; C1 < stripcode_len; C1++) {
		Content[C1] = Cap[C1];
	}
	Content[stripcode_len] = SW;
	uint32_t Index = 1;
	for (uint32_t C1 = 0; C1 <= Raw_Digits; C1++) {
		if (Check_Validity(Input[C1]) != invalid) {
			for (uint8_t C2 = 0; C2 < subcode_len; C2++) {
				Content[(Index * subcode_len) + C2] = Get_Subcode(Input[C1])[C2];
			}
			Index++;
		}
	}
	for (uint8_t C1 = 0; C1 < stripcode_len; C1++) {
		Content[(Index * subcode_len) + C1] = Cap[C1];
	}
	uint32_t Width = 0;
	for (uint32_t C1 = 0; C1 < (Digits * subcode_len) - 1; C1++) {
		Width += (Content[C1] == LW || Content[C1] == LB) ? wide_multiplier : 1;
	}
	bool* Data_Row = malloc(sizeof(bool) * Width);
	Index = 0;
	bool Marker = false;
	uint8_t Backlog = 0;
	for (uint32_t C1 = 0; C1 < (Digits * subcode_len) - 1; C1++) {
		Marker = (Content[C1] == SW || Content[C1] == LW);
		bool Extended = (Content[C1] == LW || Content[C1] == LB);
		for (uint8_t C2 = 0; C2 < ((Extended) ? wide_multiplier : 1); C2++) {
			Data_Row[Index] = Marker;
			Index++;
		}
	}
	Write_BMP(Width, Data_Row);
	free(Data_Row);
	free(Content);
}