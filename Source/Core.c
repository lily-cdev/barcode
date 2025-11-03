#include "Wrapper.h"

enum Types {
    None,
    Code39,
    Code128,
    ITF
};

enum Flag_Types {
    Set_Quietzone,
    Set_Code39,
    Set_Code128,
    Set_ITF
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

Flag Flags[POSSIBLEFLAGS] = {
    { Set_Quietzone, &Adding_Quietzone, 1, "-q", 2 },
    { Set_Code39, &Barcode_Type, Code39, "-c39", 4 },
    { Set_Code128, &Barcode_Type, Code128, "-c128", 5 },
    { Set_ITF, &Barcode_Type, ITF, "-itf", 4 }
};

int main(int Argc, char* Argv[]) {
	if (Argc == 1) {
		puts("error 01 -> an argument is required");
		return 1;
    } else if (Argc > UINT8_MAX) {
        puts("error 02 -> uint8_t overflow");
        return 2;
    }
    for (uint8_t Counter1 = 2; Counter1 < Argc; Counter1++) {
        for (uint8_t Counter2 = 0; Counter2 < POSSIBLEFLAGS; Counter2++) {
            bool Matched = true;
            for (uint8_t Counter3 = 0; Counter3 < Flags[Counter2].Length; Counter3++) {
                if (Argv[Counter1][Counter3] == '\0' || Argv[Counter1][Counter3] !=
                    Flags[Counter2].Text[Counter3]) {
                    Matched = false;
                    break;
                }
            }
            if (Matched) {
                *Flags[Counter2].Linked = Flags[Counter2].Value;
                if (Flags[Counter2].ID == Set_Quietzone) {
                    uint16_t Candidate = 0;
                    uint8_t Index = Flags[Counter2].Length;
                    while(Argv[Counter1][Index] != '\0') {
                        if (!Is_Digit(Argv[Counter1][Index])) {
                            puts("error 05 -> letters contaminating integer parameter");
                            return 5;
                        }
                        Index++;
                    }
                    for (uint8_t Counter3 = 0; Counter3 < Index - Flags[Counter2].Length; Counter3++) {
                        Candidate = To_UInt8(Argv[Counter1][Counter3 + Flags[Counter2].Length]) + (Candidate * 10);
                    }
                    if (Candidate > UINT8_MAX) {
                        puts("error 02 -> uint8_t overflow");
                        return 2;
                    }
                    if (Candidate == 0) {
                        Quietzone_Width = QUIETZONEDEFAULT;
                    } else {
                        Quietzone_Width = Candidate;
                    }
                }
            }
        }
    }
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
        uint8_t Yield = GenerateITF(Argv[1]);
        if (Yield == 1) {
            puts("error 04 -> uneven amount of digits");
            return 4;
        } else if (Yield == 2) {
            puts("error 06 -> no encodable characters given");
            return 6;
        }
        break;
    }
    return 0;
}