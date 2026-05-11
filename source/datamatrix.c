#include "../sublinker/wrapper.h"
#define dm_sizes 24

uint16_t Capacities[dm_sizes] = { 1, 3, 6, 10, 16, 20, 28, 34, 42, 60, 84, 112, 142, 172, 202, 277, 365, 453,
	573, 693, 813, 1047, 1301, 1555 };
uint8_t Widths[dm_sizes] = { 10, 12, 14, 16, 18, 20, 22, 24, 26, 32, 36, 40, 44, 48, 52, 64, 72, 80, 88, 96,
	104, 120, 132, 144 };

uint8_t GenerateDM(char* Input) {
	uint16_t Length = 0;
	while (Input[Length] != '\0') {
		Length++;
	}
	uint8_t Width = 0;
	for (uint8_t C1 = 0; C1 < dm_sizes; C1++) {
		if (Length <= Widths[C1]) {
			Width = Widths[C1];
			break;
		}
	}
	if (Width == 0) {
		return 1;
	}
	bool** Matrix = calloc(Width, sizeof(bool*));
	bool** Occupied = calloc(Width, sizeof(bool*));
	for (int C1 = 0; C1 < Width; C1++) {
		Matrix[C1] = calloc(Width, sizeof(bool));
		Occupied[C1] = calloc(Width, sizeof(bool));
	}
	memset(Matrix[0], true, sizeof(bool) * Width);
	memset(Occupied[0], true, sizeof(bool) * Width);
	Write_2D(Width, Width, Matrix);
	for (int C1 = 0; C1 < Width; C1++) {
		free(Matrix[C1]);
		free(Occupied[C1]);
	}
	free(Matrix);
	free(Occupied);
    return 0;
}