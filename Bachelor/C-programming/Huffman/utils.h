#pragma once

#include <stdbool.h>
#include <stdio.h>

#define SUCCESS 0
#define INCORRECT_INPUT 1
#define NO_MEMORY 2
#define INCORRECT_COMPRESSION 3
#define INCORRECT_PATHS 4
#define EXECUTION_ERROR 5
#define INCORRECT_MODE 6

#define SIZE_OF_BYTE 8
#define SIZE_OF_UINT256_BITS 256
#define SIZE_OF_UINT256_BYTES 32
#define ALPHABET_SIZE 256

#define SET_BIT_8(N, I, V) N = (V) ? ((1 << (SIZE_OF_BYTE - (I) - 1)) | (N)) : (~(1 << (SIZE_OF_BYTE - (I) - 1)) & (N));
#define GET_BIT_8(N, I) ((1 << (SIZE_OF_BYTE - (I) - 1) & (N)) >> (SIZE_OF_BYTE - (I) - 1))

#define SET_BIT_256(N, I, V) SET_BIT_8((N).bytes[(I) / SIZE_OF_BYTE], (I) % SIZE_OF_BYTE, V)
#define GET_BIT_256(N, I) GET_BIT_8((N).bytes[(I) / SIZE_OF_BYTE], (I) % SIZE_OF_BYTE)

#define GET_BIT_CODE(C, I) GET_BIT_256((C).code, (I))
#define SET_BIT_CODE(C, I, V) SET_BIT_256((C).code, I, V)

#define CEIL(A, B) (((A) / (B)) + ((A) % (B) ? 1 : 0))

#define CHAR_TO_DIGIT(C) ((C) - '0')

#define HANDLE_OPERATION(OP) {const Status status = (OP); if (status != SUCCESS) {return status;}}

typedef unsigned char Byte;
typedef unsigned char Status;

typedef struct{
    Byte bytes[SIZE_OF_UINT256_BYTES];
} UINT256;
typedef struct{
    UINT256 code;
    unsigned short bitsCount;
} Code;

void PrintStatus(Status status);
long GetFileSize(FILE* file);
bool IsFileEmpty(FILE* file, bool gitLabTesting);
unsigned int GetStringLength(const char* string);
bool CompareStrings(const char* string1, const char* string2);
char* SeparateNameFromFullPath(char* path);
