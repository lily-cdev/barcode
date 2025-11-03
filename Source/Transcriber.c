#include "Wrapper.h"

void Write_BMP(uint32_t Width, unsigned char* Subrow) {
    uint8_t QUIETZONEWIDTH = Quietzone_Width;
    if (Adding_Quietzone == 0) {
        QUIETZONEWIDTH = 0;
    }
	uint32_t Height = Width / 4;
	if (Width < 75) {
        Height = max(75, Height);
	}
    Height += QUIETZONEWIDTH * 2;
    uint32_t Padded_Width = Width + (QUIETZONEWIDTH * 2);
    uint32_t Row_Size = ((Padded_Width + 31) / 32) * 4;
    unsigned char* White_Row = calloc(Row_Size, sizeof(char));
    unsigned char* Row = calloc(Row_Size, sizeof(char));
    uint32_t Source_Width = ((Width + 31) / 32) * 4;
    for (uint32_t Counter = 0; Counter < Source_Width; Counter++) {
		uint32_t Byte_Index = QUIETZONEWIDTH / 8;
		uint8_t Bit_Index = QUIETZONEWIDTH & 7;
        Row[Counter + Byte_Index] |= Subrow[Counter] >> Bit_Index;
		if (Bit_Index && (Counter + Byte_Index + 1) < Row_Size) {
			Row[Counter + Byte_Index + 1] |= Subrow[Counter] << (8 - Bit_Index);
		}
    }
	uint32_t Image_Size = Row_Size * Height;
	uint32_t File_Size = Image_Size + DATAOFFSET;
	uint32_t Data_Offset = DATAOFFSET;
	FILE* Yield = NULL;
	int Check = fopen_s(&Yield, OUTPUTNAME, "wb");
	if (Check != 0) {
		return;
	}
	fwrite("BM", 1, 2, Yield);
	fwrite(&File_Size, 4, 1, Yield);
	const uint16_t Zero2 = 0;
	fwrite(&Zero2, 2, 1, Yield);
	fwrite(&Zero2, 2, 1, Yield);
	fwrite(&Data_Offset, 4, 1, Yield);
	const uint32_t DIB_Size = 40;
	fwrite(&DIB_Size, 4, 1, Yield);
	fwrite(&Padded_Width, 4, 1, Yield);
	fwrite(&Height, 4, 1, Yield);
	const uint16_t One2 = 1;
	fwrite(&One2, 2, 1, Yield);
	fwrite(&One2, 2, 1, Yield);
	const uint32_t Zero4 = 0;
	fwrite(&Zero4, 4, 1, Yield);
	fwrite(&Image_Size, 4, 1, Yield);
	const int32_t PPM = 11811;
	fwrite(&PPM, 4, 1, Yield);
	fwrite(&PPM, 4, 1, Yield);
	fwrite(&Zero4, 4, 1, Yield);
	fwrite(&Zero4, 4, 1, Yield);
	fwrite(White, 1, 4, Yield);
	fwrite(Black, 1, 4, Yield);
	for (int Counter = 0; Counter < QUIETZONEWIDTH; Counter++) {
        fwrite(White_Row, 1, Row_Size, Yield);
    }
	for (int Counter = QUIETZONEWIDTH; Counter < Height - QUIETZONEWIDTH; Counter++) {
		fwrite(Row, 1, Row_Size, Yield);
	}
    for (int Counter = Height - QUIETZONEWIDTH; Counter < Height; Counter++) {
        fwrite(White_Row, 1, Row_Size, Yield);
    }
    free(White_Row);
    free(Row);
	fclose(Yield);
}