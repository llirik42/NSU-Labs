#pragma once

#include "../utils.h"
#include "huffman_tree.h"
#include <stdio.h>
#include <stdbool.h>

#define MATCHING_BUFFER_SIZE 1024

#define CAN_MATCH(MATCHING_BUFFER) ((MATCHING_BUFFER).maxOffset - (MATCHING_BUFFER).offset)
#define BYTES_LEFT(MATCHING_BUFFER) (((MATCHING_BUFFER).maxOffset - (MATCHING_BUFFER).offset) / SIZE_OF_BYTE)

struct MatchingBuffer{
    FILE* in;
    bool canRead;
    UINT256 elements[MATCHING_BUFFER_SIZE];
    int nextChar;
    int nextNextChar;
    unsigned int offset;
    unsigned int maxOffset;
    const struct Node* huffmanTreeRoot;
};

void InitMatchingBuffer(struct MatchingBuffer* matchingBuffer, const struct Node* huffmanTreeRoot, FILE* in);
Byte Match(struct MatchingBuffer* matchingBuffer);
