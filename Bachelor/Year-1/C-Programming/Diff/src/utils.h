#pragma once

#include <stdbool.h>
#include <stdio.h>

typedef unsigned char Byte;
typedef unsigned int Number;

#define MAX_BUFFER_SIZE 1024
#define BLOCK_SIZE 16
#define MAX_STRING_OUTPUT_LENGTH 80
#define MAX_WIDE_HEX_NOTATION_LENGTH_HALVED 4
#define MAX_DOUBLE_HEX_NOTATION_LENGTH 2
#define MAX_PATH_LENGTH 128

#define MIN(A, B) ((A) < (B) ? (A) : (B))
#define MAX(A, B) ((A) > (B) ? (A) : (B))
#define IS_NEW_LINE(C) ((C) == '\n' || (C) == '\r')
#define IS_EOF(C) ((C) == '\0')
#define IS_NEW_LINE_OR_EOF(C) (IS_NEW_LINE(C) || IS_EOF(C))

bool IsFileTxt(FILE* file);
const char* ToDoubleHex(Byte byte);
long GetFileSize(FILE* file);
bool CompareStrings(const char* string1, const char* string2);
bool CompareFiles(FILE* file1, FILE* file2);
