#include "../sublinker/wrapper.h"

FILE* Write_Header(uint32_t Width, uint32_t Height) {
	FILE* Yield = fopen(output_name, "wb");
	fwrite("BM", 2, 1, Yield);
	#define write(Type, Data) do { fwrite(&(Type){ Data }, sizeof(Type), 1, Yield); } while (0)
	write(uint32_t, (((((Width * 8) + 31) / 32) * 4) * Height) + data_offset);
	write(uint16_t, 0);
	write(uint16_t, 0);
	write(uint32_t, data_offset);
	write(uint32_t, 40);
	write(int32_t, Width);
	write(int32_t, Height);
	write(uint16_t, 1);
	write(uint16_t, 8);
	write(uint32_t, 0);
	write(uint32_t, 0);
	write(int32_t, 12000);
	write(int32_t, 12000);
	write(uint32_t, 2);
	write(uint32_t, 0);
	#undef write
	fwrite(Black, 4, sizeof(uint8_t), Yield);
	fwrite(White, 4, sizeof(uint8_t), Yield);
	return Yield;
}

uint32_t Pad_Image(uint32_t* Height, uint32_t Width, uint8_t* Subzone_Width, uint8_t* Pad_Len) {
	(*Subzone_Width) = Adding_Quietzone * Quietzone_Width;
    (*Height) += (*Subzone_Width) * 2;
	uint32_t Yield = ((*Subzone_Width) * 2) + Width;
	(*Pad_Len) = (4 - (Yield & 3)) & 3;
	return Yield;
}

void Write_BMP(uint32_t Width, bool* Subrow) {
	uint32_t Height = Width / 4;
	if (Width < 75) {
        Height = max(75, Height);
	}
    uint8_t Subzone_Width, Pad_Len;
	uint32_t Row_Size = Pad_Image(&Height, Width, &Subzone_Width, &Pad_Len);
	uint32_t Full = Row_Size + Pad_Len;
	uint8_t* White_Row = malloc(Full);
	memset(White_Row, 1, Full);
    uint8_t* Row = malloc(Full);
	memset(Row, 1, Full);
    for (uint32_t C1 = 0; C1 < Width; C1++) {
		Row[C1 + Subzone_Width] = (uint8_t)Subrow[C1];
	}
	FILE* Yield = Write_Header(Row_Size, Height);
	uint8_t* Padding = calloc(Pad_Len, sizeof(uint8_t));
	for (uint8_t C1 = 0; C1 < Subzone_Width; C1++) {
		fwrite(White_Row, Full, sizeof(uint8_t), Yield);
	}
	for (uint32_t C1 = 0; C1 < Height - (Subzone_Width * 2); C1++) {
		fwrite(Row, Full, sizeof(uint8_t), Yield);
	}
	for (uint8_t C1 = 0; C1 < Subzone_Width; C1++) {
		fwrite(White_Row, Full, sizeof(uint8_t), Yield);
	}
	free(White_Row);
    free(Row);
	fclose(Yield);
}

void Write_2D(uint32_t Width, uint32_t Height, bool** Content) {
	uint8_t Subzone_Width, Pad_Len;
	uint32_t Row_Size = Pad_Image(&Height, Width, &Subzone_Width, &Pad_Len);
	FILE* Yield = Write_Header(Row_Size, Height);
	uint8_t* White_Row = malloc(Row_Size + Pad_Len);
	memset(White_Row, 1, Row_Size + Pad_Len);
	uint8_t* Start = malloc(Subzone_Width);
	memset(Start, 1, Subzone_Width);
	uint8_t* End = malloc(Subzone_Width + Pad_Len);
	memset(End, 1, Subzone_Width + Pad_Len);
	for (uint8_t C1 = 0; C1 < Subzone_Width; C1++) {
        fwrite(White_Row, sizeof(uint8_t), Row_Size, Yield);
    }
	for (uint32_t C1 = 0; C1 < Height - (Subzone_Width * 2); C1++) {
		fwrite(Start, sizeof(uint8_t), Subzone_Width, Yield);
		fwrite(Content[C1], sizeof(uint8_t), Width, Yield);
		fwrite(End, sizeof(uint8_t), Subzone_Width + Pad_Len, Yield);
	}
	for (uint8_t C1 = 0; C1 < Subzone_Width; C1++) {
		fwrite(White_Row, sizeof(uint8_t), Row_Size, Yield);
	}
	free(White_Row);
	fclose(Yield);
}