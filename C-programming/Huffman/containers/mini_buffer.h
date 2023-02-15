#pragma once

#include "../utils.h"
#include <stdbool.h>
#include <stdio.h>

#define MINI_BUFFER_SIZE 2
#define EMPTY_BIT (-1)

struct MiniBuffer{
    Byte bytes[MINI_BUFFER_SIZE];
    unsigned char bitsLeft;
    FILE* in;
};

void InitMiniBuffer(struct MiniBuffer* miniBuffer, FILE* in);
bool InputMiniBuffer(struct MiniBuffer* miniBuffer);
char ExtractBit(struct MiniBuffer* miniBuffer);
int ExtractSymbol(struct MiniBuffer* miniBuffer);
