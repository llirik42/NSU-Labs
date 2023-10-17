#pragma once

#define MAX_HEAP_SIZE 256

typedef unsigned char Byte;

struct Heap{
    Byte elements[MAX_HEAP_SIZE];
    unsigned char elementsCount;
};

void InitHeap(struct Heap* heap);
void Insert(struct Heap* heap, Byte byte);
int ExtractMinimum(struct Heap* heap);
