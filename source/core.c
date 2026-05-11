#include "../sublinker/wrapper.h"

enum Types {
	None,
	Code39,
	Code128,
	ITF,
	DM
};

enum Flag_Types {
	Set_Quietzone,
	Set_Code39,
	Set_Code128,
	Set_ITF,
	Set_DM
};

const unsigned char Black[4] = { 0, 0, 0, 0 };
const unsigned char White[4] = { 255, 255, 255, 0 };
uint8_t Adding_Quietzone = 0;
uint8_t Quietzone_Width = 0;
uint8_t Barcode_Type = None;

typedef struct {
	uint8_t ID;
	uint8_t* Linked;
	uint8_t Value;
	char* Text;
	uint8_t Length;
} Flag;

Flag Flags[flag_ct] = {
	{ Set_Quietzone, &Adding_Quietzone, 1, "-q", 2 },
	{ Set_Code39, &Barcode_Type, Code39, "-c39", 4 },
	{ Set_Code128, &Barcode_Type, Code128, "-c128", 5 },
	{ Set_ITF, &Barcode_Type, ITF, "-itf", 4 },
	{ Set_DM, &Barcode_Type, DM, "-dm", 3 }
};

bool Is_Digit(const char Input) {
	return Input >= '0' && Input <= '9';
}

uint8_t To_UInt8(const char Input) {
	return (uint8_t)(Input) - (uint8_t)'0';
}

char To_Lower(const char Input) {
	if (Input >= 'A' && Input <= 'Z') {
		return Input - ('A' - 'a');
	}
	return Input;
}

int main(int Argc, char* Argv[]) {
	if (Argc == 1) {
		puts("error 01 -> an argument is required");
		return 1;
	} else if (Argc > UINT8_MAX) {
		puts("error 02 -> uint8_t overflow");
		return 2;
	}
	for (uint8_t C1 = 2; C1 < Argc; C1++) {
		for (uint8_t C2 = 0; C2 < flag_ct; C2++) {
			bool Matched = true;
			for (uint8_t C3 = 0; C3 < Flags[C2].Length; C3++) {
				if (Argv[C1][C3] == '\0' || Argv[C1][C3] != Flags[C2].Text[C3]) {
					Matched = false;
					break;
				}
			}
			if (Matched) {
				*Flags[C2].Linked = Flags[C2].Value;
				if (Flags[C2].ID == Set_Quietzone) {
					uint16_t Candidate = 0;
					uint8_t Index = Flags[C2].Length;
					while(Argv[C1][Index] != '\0') {
						if (!Is_Digit(Argv[C1][Index])) {
							puts("error 05 -> no letters allowed");
							return 5;
						}
						Index++;
					}
					for (uint8_t C3 = 0; C3 < Index - Flags[C2].Length; C3++) {
						Candidate = To_UInt8(Argv[C1][C3 + Flags[C2].Length]) + (Candidate * 10);
					}
					if (Candidate > UINT8_MAX) {
						puts("error 02 -> uint8_t overflow");
						return 2;
					}
					Quietzone_Width = (Candidate == 0) ? quietzone_default : Candidate;
				}
			}
		}
	}
	uint8_t Yield;
	switch (Barcode_Type) {
	case None:
		puts("error 03 -> an encoding specification is required");
		return 3;
	case Code39:
		Generate39(Argv[1]);
		return 0;
	case Code128:
		Generate128(Argv[1]);
		break;
	case ITF:
		Yield = GenerateITF(Argv[1]);
		if (Yield == 1) {
			puts("error 04 -> uneven amount of digits");
			return 4;
		} else if (Yield == 2) {
			puts("error 06 -> no encodable characters given");
			return 6;
		}
		break;
	case DM:
		Yield = GenerateDM(Argv[1]);
		if (Yield == 1) {
			puts("error 07 -> input too large");
			return 7;
		}
	}
	return 0;
}