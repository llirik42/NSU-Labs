#pragma once

typedef unsigned int Vertex;
typedef unsigned int Number;
typedef unsigned char Byte;
typedef int Code;

#define SET_BIT(B, N, T) B = (T) ? ((1 << (N)) | (B)) : (~(1 << (N)) & (B));
#define GET_BIT(B, N) (((B) >> (N)) % 2)
#define MIN(A, B) ((A) > (B) ? (B) : (A))
