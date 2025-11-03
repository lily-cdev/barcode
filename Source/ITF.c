#include "Wrapper.h"

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
    for (uint32_t Counter = 0; Counter < Raw_Digits; Counter++) {
        if (Is_Digit(Input[Counter])) {
            Data[Index] = (int)(Input[Counter]) - (int)'0';
            Index++;
        }
    }
    uint32_t Modules = (Digits * 7) + 8;
    unsigned char* Data_Row = calloc(Modules, sizeof(char));
    uint32_t Byte_Index;
    uint8_t Bit_Index;
    for (int Counter = 0; Counter < 4; Counter++) {
		Byte_Index = Counter / 8;
		Bit_Index = 7 - (Counter % 8);
        if (Counter % 2 == 0) {
            Data_Row[Byte_Index] |= (1 << Bit_Index);
        } else {
            Data_Row[Byte_Index] &= ~(1 << Bit_Index);
        }
    }
    uint8_t Black_Number;
    uint8_t White_Number;
	bool White = false;
    uint8_t Position = 4;
	for (uint32_t Counter1 = 4; Counter1 < Modules - 4; Counter1 += 14) {
        Black_Number = Data[(Counter1 - 4) / 7];
        White_Number = Data[((Counter1 - 4) / 7) + 1];
        for (uint8_t Counter2 = 0; Counter2 < 5; Counter2++) {
            if (ITF_Table[Black_Number].Long1_Pos == Counter2 ||
                ITF_Table[Black_Number].Long2_Pos == Counter2) {
                Byte_Index = Position / 8;
                Bit_Index = 7 - (Position % 8);
                Data_Row[Byte_Index] |= (1 << Bit_Index);
                Position++;
            }
            Byte_Index = Position / 8;
            Bit_Index = 7 - (Position % 8);
            Data_Row[Byte_Index] |= (1 << Bit_Index);
            Position++;
            if (ITF_Table[White_Number].Long1_Pos == Counter2 ||
                ITF_Table[White_Number].Long2_Pos == Counter2) {
                Byte_Index = Position / 8;
                Bit_Index = 7 - (Position % 8);
                Data_Row[Byte_Index] &= ~(1 << Bit_Index);
                Position++;
            }
            Byte_Index = Position / 8;
            Bit_Index = 7 - (Position % 8);
            Data_Row[Byte_Index] &= ~(1 << Bit_Index);
            Position++;
        }
		Byte_Index = (Modules - 4) / 8;
		Bit_Index = 7 - ((Modules - 4) % 8);
		Data_Row[Byte_Index] |= (1 << Bit_Index);
        for (int Counter = Modules - 3; Counter < Modules; Counter++) {
            Byte_Index = Counter / 8;
            Bit_Index = 7 - (Counter % 8);
            if (Counter % 2 == 0) {
                Data_Row[Byte_Index] &= ~(1 << Bit_Index);
            } else {
                Data_Row[Byte_Index] |= (1 << Bit_Index);
            }
        }
	}
    Write_BMP(Modules, Data_Row);
    free(Data_Row);
    free(Data);
    free(Input);
    return 0;
}