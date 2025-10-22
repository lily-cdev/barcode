#include "Wrapper.h"
#define POSSIBLEFLAGS 4

enum Types {
    None,
    Code39,
    Code128,
    ITF
};

const unsigned char Black[4] = { 0, 0, 0, 0 };
const unsigned char White[4] = { 255, 255, 255, 0 };
uint8_t Adding_Quietzone = 0;
uint8_t Barcode_Type = None;

typedef struct {
    uint8_t* Linked;
    uint8_t Value;
    char* Text;
    int Length;
} Flag;

Flag Flags[POSSIBLEFLAGS] = {
    { &Adding_Quietzone, 1, "-q", 2 },
    { &Barcode_Type, Code39, "-c39", 4 },
    { &Barcode_Type, Code128, "-c128", 5 },
    { &Barcode_Type, ITF, "-itf", 4 }
};

int main(int Argc, char* Argv[]) {
	if (Argc == 1) {
		puts("error -> an argument is required");
		return 1;
    } else if (Argc > 255) {
        puts("error -> too many arguments!");
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
            }
        }
    }
    switch (Barcode_Type) {
    case None:
        puts("error -> an encoding specification is required");//
        return 3;
    case Code39:
        Generate39(Argv[1]);
        return 0;
    case Code128:
        Generate128(Argv[1]);
        break;
    case ITF:
        if (GenerateITF(Argv[1]) == 1) {
            puts("error -> uneven amount of digits");
            return 1;
        }
        break;
    }
    return 0;
}