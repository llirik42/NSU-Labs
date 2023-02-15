#pragma once

#include "huffman_tree.h"

struct Queue;

struct Queue* CreateQueue(unsigned short maxSize);
void DestroyQueue(struct Queue* queue);
void Insert(struct Queue* queue, struct Node* node);
struct Node* ExtractMinimum(struct Queue* queue);
