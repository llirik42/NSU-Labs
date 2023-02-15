#pragma once

#include "config.h"

#define SIZE_OF_BYTE 8
#define POSSIBLE_BYTES_COUNT 256

#define FIRST_DRIVER_INDEX (-1)

#define GET_BIT_8(N, I) ((1 << (SIZE_OF_BYTE - (I) - 1) & (N)) >> (SIZE_OF_BYTE - (I) - 1))
#define SET_BIT_8(N, I, V) N = (V) ? ((1 << (SIZE_OF_BYTE - (I) - 1)) | (N)) : (~(1 << (SIZE_OF_BYTE - (I) - 1)) & (N));

#define ACTUAL_FIELD_WIDTH_BYTES (CEIL(FIELD_WIDTH, SIZE_OF_BYTE))

#define CEIL(A, B) (((A) / (B)) + ((A) % (B) ? 1 : 0))

#define COLOR unsigned char r, unsigned char g, unsigned char b, unsigned char a

typedef unsigned char Byte;

unsigned char GetBitsCount(Byte byte);
