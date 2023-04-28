#pragma once

#include <stdio.h>
#include "utils.h"

struct Buffer{
    Number bytesCount;
    Byte bytes[MAX_BUFFER_SIZE + 1];
};

void InitBuffer(struct Buffer* buffer);
Number WriteInputToBuffer(struct Buffer* buffer, FILE* file);
Number AppendInputToBuffer(struct Buffer* buffer, FILE* file);
Number SmartShift(struct Buffer* buffer, Number discrepancyByteIndex);
Number CompareBuffers(const struct Buffer* buffer1, const struct Buffer* buffer2);

Byte GetByteInTxt(const struct Buffer* buffer, Number index);
const char* GetByteInHex(const struct Buffer* buffer, Number index);
Number GetNewLineSymbolsCount(const struct Buffer* buffer, Number index);
