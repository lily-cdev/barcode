#include "Wrapper.h"

#define CHUNKS 106
#define CHUNKSIZE 6
#define STARTSETB 104
#define OFFSET 102

const uint8_t Set_Exchangers[3] = { 101, 100, 99 };

enum Charsets {
	None,
	Set_A,
	Set_B,
	Set_C
};

struct Chunk {
	uint8_t ID;
	char Code_A;
	char Code_B;
	uint8_t Code_C;
	uint8_t Barcode[6];
};

struct Chunk Lookup_Table[CHUNKS] = {
	{ 0, ' ', ' ', 0, { 2, 1, 2, 2, 2, 2 } }, { 1, '!', '!', 1, { 2, 2, 2, 1, 2, 2 } },
	{ 2, '\"', '\"', 2, { 2, 2, 2, 2, 2, 1 } }, { 3, '#', '#', 3, { 1, 2, 1, 2, 2, 3 } },
	{ 4, '$', '$', 4, { 1, 2, 1, 3, 2, 2 } }, { 5, '%', '%', 5, { 1, 3, 1, 2, 2, 2 } },
	{ 6, '&', '&', 6, { 1, 2, 2, 2, 1, 3 } }, { 7, '\'', '\'', 7, { 1, 2, 2, 3, 1, 2 } },
	{ 8, '(', '(', 8, { 1, 3, 2, 2, 1, 2 } }, { 9, ')', ')', 9, { 2, 2, 1, 2, 1, 3 } },
	{ 10, '*', '*', 10, { 2, 2, 1, 3, 1, 2 } }, { 11, '+', '+', 11, { 2, 3, 1, 2, 1, 2 } },
	{ 12, ',', ',', 12, { 1, 1, 2, 2, 3, 2 } }, { 13, '-', '-', 13, { 1, 2, 2, 1, 3, 2 } },
	{ 14, '.', '.', 14, { 1, 2, 2, 2, 3, 1 } }, { 15, '/', '/', 15, { 1, 1, 3, 2, 2, 2 } },
	{ 16, '0', '0', 16, { 1, 2, 3, 1, 2, 2 } }, { 17, '1', '1', 17, { 1, 2, 3, 2, 2, 1 } },
	{ 18, '2', '2', 18, { 2, 2, 3, 2, 1, 1 } }, { 19, '3', '3', 19, { 2, 2, 1, 1, 3, 2 } },
	{ 20, '4', '4', 20, { 2, 2, 1, 2, 3, 1 } }, { 21, '5', '5', 21, { 2, 1, 3, 2, 1, 2 } },
	{ 22, '6', '6', 22, { 2, 2, 3, 1, 1, 2 } }, { 23, '7', '7', 23, { 3, 1, 2, 1, 3, 1 } },
	{ 24, '8', '8', 24, { 3, 1, 1, 2, 2, 2 } }, { 25, '9', '9', 25, { 3, 2, 1, 1, 2, 2 } },
	{ 26, ':', ':', 26, { 3, 2, 1, 2, 2, 1 } }, { 27, ';', ';', 27, { 3, 1, 2, 2, 1, 2 } },
	{ 28, '<', '<', 28, { 3, 2, 2, 1, 1, 2 } }, { 29, '=', '=', 29, { 3, 2, 2, 2, 1, 1 } },
	{ 30, '>', '>', 30, { 2, 1, 2, 1, 2, 3 } }, { 31, '?', '?', 31, { 2, 1, 2, 3, 2, 1 } },
	{ 32, '@', '@', 32, { 2, 3, 2, 1, 2, 1 } }, { 33, 'A', 'A', 33, { 1, 1, 1, 3, 2, 3 } },
	{ 34, 'B', 'B', 34, { 1, 3, 1, 1, 2, 3 } }, { 35, 'C', 'C', 35, { 1, 3, 1, 3, 2, 1 } },
	{ 36, 'D', 'D', 36, { 1, 1, 2, 3, 1, 3 } }, { 37, 'E', 'E', 37, { 1, 3, 2, 1, 1, 3 } },
	{ 38, 'F', 'F', 38, { 1, 3, 2, 3, 1, 1 } }, { 39, 'G', 'G', 39, { 2, 1, 1, 3, 1, 3 } },
	{ 40, 'H', 'H', 40, { 2, 3, 1, 1, 1, 3 } }, { 41, 'I', 'I', 41, { 2, 3, 1, 3, 1, 1 } },
	{ 42, 'J', 'J', 42, { 1, 1, 2, 1, 3, 3 } }, { 43, 'K', 'K', 43, { 1, 1, 2, 3, 3, 1 } },
	{ 44, 'L', 'L', 44, { 1, 3, 2, 1, 3, 1 } }, { 45, 'M', 'M', 45, { 1, 1, 3, 1, 2, 3 } },
	{ 46, 'N', 'N', 46, { 1, 1, 3, 3, 2, 1 } }, { 47, 'O', 'O', 47, { 1, 3, 3, 1, 2, 1 } },
	{ 48, 'P', 'P', 48, { 3, 1, 3, 1, 2, 1 } }, { 49, 'Q', 'Q', 49, { 2, 1, 1, 3, 3, 1 } },
	{ 50, 'R', 'R', 50, { 2, 3, 1, 1, 3, 1 } }, { 51, 'S', 'S', 51, { 2, 1, 3, 1, 1, 3 } },
	{ 52, 'T', 'T', 52, { 2, 1, 3, 3, 1, 1 } }, { 53, 'U', 'U', 53, { 2, 1, 3, 1, 3, 1 } },
	{ 54, 'V', 'V', 54, { 3, 1, 1, 1, 2, 3 } }, { 55, 'W', 'W', 55, { 3, 1, 1, 3, 2, 1 } },
	{ 56, 'X', 'X', 56, { 3, 3, 1, 1, 2, 1 } }, { 57, 'Y', 'Y', 57, { 3, 1, 2, 1, 1, 3 } },
	{ 58, 'Z', 'Z', 58, { 3, 1, 2, 3, 1, 1 } }, { 59, '[', '[', 59, { 3, 3, 2, 1, 1, 1 } },
	{ 60, '\\', '\\', 60, { 3, 1, 4, 1, 1, 1 } }, { 61, ']', ']', 61, { 2, 2, 1, 4, 1, 1 } },
	{ 62, '^', '^', 62, { 4, 3, 1, 1, 1, 1 } }, { 63, '_', '_', 63, { 1, 1, 1, 2, 2, 4 } },
	{ 64, '\0', '`', 64, { 1, 1, 1, 4, 2, 2 } }, { 65, '\x01', 'a', 65, { 1, 2, 1, 1, 2, 4 } },
	{ 66, '\x02', 'b', 66, { 1, 2, 1, 4, 2, 1 } }, { 67, '\x03', 'c', 67, { 1, 4, 1, 1, 2, 2 } },
	{ 68, '\x04', 'd', 68, { 1, 4, 1, 2, 2, 1 } }, { 69, '\x05', 'e', 69, { 1, 1, 2, 2, 1, 4 } },
	{ 70, '\x06', 'f', 70, { 1, 1, 2, 4, 1, 2 } }, { 71, '\x07', 'g', 71, { 1, 2, 2, 1, 1, 4 } },
	{ 72, '\x08', 'h', 72, { 1, 2, 2, 4, 1, 1 } }, { 73, '\x09', 'i', 73, { 1, 4, 2, 1, 1, 2 } },
	{ 74, '\x0A', 'j', 74, { 1, 4, 2, 2, 1, 1 } }, { 75, '\x0B', 'k', 75, { 2, 4, 1, 2, 1, 1 } },
	{ 76, '\x0C', 'l', 76, { 2, 2, 1, 1, 1, 4 } }, { 77, '\x0D', 'm', 77, { 4, 1, 3, 1, 1, 1 } },
	{ 78, '\x0E', 'n', 78, { 2, 4, 1, 1, 1, 2 } }, { 79, '\x0F', 'o', 79, { 1, 3, 4, 1, 1, 1 } },
	{ 80, '\x10', 'p', 80, { 1, 1, 1, 2, 4, 2 } }, { 81, '\x11', 'q', 81, { 1, 2, 1, 1, 4, 2 } },
	{ 82, '\x12', 'r', 82, { 1, 2, 1, 2, 4, 1 } }, { 83, '\x13', 's', 83, { 1, 1, 4, 2, 1, 2 } },
	{ 84, '\x14', 't', 84, { 1, 2, 4, 1, 1, 2 } }, { 85, '\x15', 'u', 85, { 1, 2, 4, 2, 1, 1 } },
	{ 86, '\x16', 'v', 86, { 4, 1, 1, 2, 1, 2 } }, { 87, '\x17', 'w', 87, { 4, 2, 1, 1, 1, 2 } },
	{ 88, '\x18', 'x', 88, { 4, 2, 1, 2, 1, 1 } }, { 89, '\x19', 'y', 89, { 2, 1, 2, 1, 4, 1 } },
	{ 90, '\x1A', 'z', 90, { 2, 1, 4, 1, 2, 1 } }, { 91, '\x1B', '{', 91, { 4, 1, 2, 1, 2, 1 } },
	{ 92, '\x1C', '|', 92, { 1, 1, 1, 1, 4, 3 } }, { 93, '\x1D', '}', 93, { 1, 1, 1, 3, 4, 1 } },
	{ 94, '\x1E', '\0', 94, { 1, 3, 1, 1, 4, 1 } }, { 95, '\x1F', '\x7F', 95, { 1, 1, 4, 1, 1, 3 } },
	{ 96, '\x1E', '\x1E', 96, { 1, 1, 4, 3, 1, 1 } }, { 97, '\x1F', '\x1F', 97, { 4, 1, 1, 1, 1, 3 } },
	{ 98, '\x7F', '\x7F', 98, { 4, 1, 1, 3, 1, 1 } }, { 99, '\0', '\0', 99, { 1, 1, 3, 1, 4, 1 } },
	{ 100, '\0', '\0', 250, { 1, 1, 4, 1, 3, 1 } }, { 101, '\0', '\0', 250, { 3, 1, 1, 1, 4, 1 } },
	{ 102, '\0', '\0', 250, { 4, 1, 1, 1, 3, 1 } }, { 103, '\0', '\0', 250, { 2, 1, 1, 4, 1, 2 } },
	{ 104, '\0', '\0', 250, { 2, 1, 1, 2, 1, 4 } }, { 105, '\0', '\0', 250, { 2, 1, 1, 2, 3, 2 } },
};

bool Scan_Charset(const uint8_t Set, uint8_t* Position, char Candidate1, char Candidate2) {
	for (uint8_t Counter = 0; Counter < CHUNKS; Counter++) {
		if ((Set == Set_A && Lookup_Table[Counter].Code_A == Candidate1) ||
			(Set == Set_B && Lookup_Table[Counter].Code_B == Candidate1)) {
			*Position = Counter;
			return true;
		} else if (Set == Set_C) {
			if (Is_Digit(Candidate1) && Is_Digit(Candidate2)) {
				uint8_t Value = (((uint8_t)Candidate1 - (uint8_t)'0') * 10) +
					((uint8_t)Candidate2 - (uint8_t)'0');
				if (Lookup_Table[Counter].Code_C == Value) {
					*Position = Counter;
					return true;
				}
			}
		}
	}
	return false;
}

const uint8_t Full_Stop[7] = { 2, 3, 3, 1, 1, 1, 2 };

void Apply_Chunk(const bool Writing, uint32_t* Position, uint32_t* Index,
	uint8_t* Content, uint8_t Chunk[CHUNKSIZE], const uint8_t Increment) {
	if (Writing) {
		for (uint8_t Counter = 0; Counter < CHUNKSIZE; Counter++) {
			Content[*Position + Counter] = Chunk[Counter];
		}
	}
	*Position += CHUNKSIZE;
	*Index += Increment;
}

void Increment_Checksum(uint64_t* Checksum, uint32_t* Checksum_Index,
	const uint8_t Position) {
	*Checksum += *Checksum_Index * Position;
	(*Checksum_Index)++;
}

uint32_t Encode(uint8_t* Data, const bool Writing, uint8_t* Barcode, const int Cap) {
	uint32_t Bars = 0;
	uint32_t Index = 0;
	uint64_t Checksum = STARTSETB;
	uint32_t Checksum_Index = 1;
	Apply_Chunk(Writing, &Bars, &Index, Barcode, Lookup_Table
		[STARTSETB].Barcode, 0);
	uint8_t Selected_Charset = Set_B;
	while (Index < Cap) {
		uint8_t Queried_Charset = None;
		if (Selected_Charset == Set_C) {
			uint8_t Position = 0;
			if (Index < Cap - 1 && Is_Digit(Data[Index]) && Is_Digit(
				Data[Index + 1]) && Scan_Charset(Selected_Charset,
					&Position, Data[Index], Data[Index + 1])) {
				Apply_Chunk(Writing, &Bars, &Index, Barcode,
					Lookup_Table[Position].Barcode, 2);
				Increment_Checksum(&Checksum, &Checksum_Index, Position);
			} else {
				Queried_Charset = Set_B;
			}
		} else {
			uint8_t Position = 0;
			if (Scan_Charset(Selected_Charset, &Position, Data[Index], 0)) {
				Apply_Chunk(Writing, &Bars, &Index, Barcode,
					Lookup_Table[Position].Barcode, 1);
				Increment_Checksum(&Checksum, &Checksum_Index, Position);
			} else {
				if (Selected_Charset == Set_A) {
					Queried_Charset = Set_B;
				} else {
					Queried_Charset = Set_A;
				}
			}
			if (Index < Cap - 1 && Is_Digit(Data[Index]) && Is_Digit(Data[Index + 1])) {
				uint8_t Digits = 2;
				while (Index + Digits < Cap && Is_Digit(Data[Index + Digits])) {
					Digits++;
				}
				if (Digits >= 4 && Digits % 2 == 0) {
					Queried_Charset = Set_C;
				}
			}
		}
		if (Queried_Charset != None) {
			Apply_Chunk(Writing, &Bars, &Index, Barcode,
				Lookup_Table[Set_Exchangers[Queried_Charset - 1]].Barcode, 0);	
			Increment_Checksum(&Checksum, &Checksum_Index, Set_Exchangers[Queried_Charset - 1]);
			Selected_Charset = Queried_Charset;
		}
	}
	Checksum %= 103;
	Apply_Chunk(Writing, &Bars, &Index, Barcode,
		Lookup_Table[Checksum].Barcode, 0);
	if (Writing) {
		for (uint8_t Counter = 0; Counter < CHUNKSIZE + 1; Counter++) {
			Barcode[Bars + Counter] = Full_Stop[Counter];
		}
	}
	Bars += CHUNKSIZE + 1;
	return Bars;
}

void Generate128(char* Input) {
	uint32_t Characters = 0;
	uint32_t Raw_Characters = 0;
	while (Input[Raw_Characters] != '\0') {
		if ((int32_t)Input[Raw_Characters] > 0 && (int32_t)Input[Raw_Characters] < 128) {
			Characters++;
		}
		Raw_Characters++;
	}
	uint8_t* Content = malloc(sizeof(uint8_t) * Characters);
	uint32_t Ticker = 0;
	for (uint32_t Counter = 0; Counter <= Raw_Characters; Counter++) {
		if ((int32_t)Input[Counter] > 0 && (int32_t)Input[Counter] < 128) {
			Content[Ticker] = (uint8_t)Input[Counter];
			Ticker++;
		}
	}
	uint32_t Bars = Encode(Content, false, NULL, Characters);
	uint8_t* Barcode = malloc(sizeof(uint8_t) * Bars);
	Encode(Content, true, Barcode, Characters);
	uint32_t Width = 0;
	for (uint32_t Counter1 = 0; Counter1 < Bars; Counter1++) {
		for (uint8_t Counter2 = 0; Counter2 < Barcode[Counter1]; Counter2++) {
			Width++;
		}
	}
	unsigned char* Data_Row = calloc((((Width + 7) / 8) + 3) & ~3, sizeof(char));
	uint32_t Index = 0;
	bool White = false;
	for (uint32_t Counter1 = 0; Counter1 < Bars; Counter1++) {
		for (uint8_t Counter2 = 0; Counter2 < Barcode[Counter1]; Counter2++) {
			uint32_t Byte_Index = Index / 8;
			uint8_t Bit_Index = 7 - (Index % 8);
			if (White) {
				Data_Row[Byte_Index] &= ~(1 << Bit_Index);					
			} else {
				Data_Row[Byte_Index] |= (1 << Bit_Index);
			}
			Index++;
		}
		White = !White;
	}
	Write_BMP(Width, Data_Row);
	free(Data_Row);
	free(Barcode);
	free(Content);
}