#pragma once

#include "../utils.h"
#include <stdio.h>
#include <stdbool.h>

#define PRINTING_BUFFER_SIZE 1024

struct PrintingBuffer{
    Byte elements[PRINTING_BUFFER_SIZE];
    unsigned short bitsCount;
    FILE* output;
};

void InitPrintingBuffer(struct PrintingBuffer* buffer, FILE* output);
Status PushCode(struct PrintingBuffer* printingBuffer, Code code);
Status PushByte(struct PrintingBuffer* printingBuffer, Byte byte);
Status PushBit(struct PrintingBuffer* printingBuffer, bool bit);
Status DumpPrintingBuffer(const struct PrintingBuffer* buffer);
