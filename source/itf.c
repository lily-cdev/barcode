#include "../sublinker/wrapper.h"

typedef struct {
	uint8_t Long1_Pos;
	uint8_t Long2_Pos;
} Symbol;

const Symbol ITF_Table[10] = {
	{ 2, 3 },
	{ 0, 4 },
	{ 1, 4 },
	{ 0, 1 },
	{ 2, 4 },
	{ 0, 2 },
	{ 1, 2 },
	{ 3, 4 },
	{ 0, 3 },
	{ 1, 3 },
};

uint8_t GenerateITF(char* Input) {
	uint32_t Digits = 0;
	uint32_t Raw_Digits = 0;
	while (Input[Raw_Digits] != '\0') {
		if (Is_Digit(Input[Raw_Digits])) {
			Digits++;
		}
		Raw_Digits++;
	}
	if (Digits <= 0) {
		return 2;
	}
	if (Digits % 2 != 0) {
		return 1;
	}
	uint8_t* Data = malloc(sizeof(uint8_t) * Digits);
	uint32_t Index = 0;
	for (uint32_t C1 = 0; C1 < Raw_Digits; C1++) {
		if (Is_Digit(Input[C1])) {
			Data[Index] = (int)(Input[C1]) - (int)'0';
			Index++;
		}
	}
	uint32_t Modules = (Digits * 7) + 8;
	bool* Data_Row = calloc(Modules, sizeof(bool));
	for (int C1 = 0; C1 < 4; C1++) {
		Data_Row[C1] = (C1 & 1);
	}
	uint8_t Black_Number;
	uint8_t White_Number;
	bool White = false;
	uint8_t Position = 4;
	for (uint32_t C1 = 4; C1 < Modules - 4; C1 += 14) {
		Black_Number = Data[(C1 - 4) / 7];
		White_Number = Data[((C1 - 4) / 7) + 1];
		for (uint8_t C2 = 0; C2 < 5; C2++) {
			if (ITF_Table[Black_Number].Long1_Pos == C2 || ITF_Table[Black_Number].Long2_Pos == C2) {
				Data_Row[Position] = false;
				Position++;
			}
			Data_Row[Position] = false;
			Position++;
			if (ITF_Table[White_Number].Long1_Pos == C2 || ITF_Table[White_Number].Long2_Pos == C2) {
				Data_Row[Position] = true;
				Position++;
			}
			Data_Row[Position] = true;
			Position++;
		}
		Data_Row[Position] = false;
		for (int C1 = Modules - 3; C1 < Modules; C1++) {
			Data_Row[Position] = !(C1 & 1);
		}
	}
	Write_BMP(Modules, Data_Row);
	free(Data_Row);
	free(Data);
	return 0;
}