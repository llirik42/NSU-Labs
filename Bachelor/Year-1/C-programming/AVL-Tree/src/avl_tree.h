#pragma once

#include <stdbool.h>

struct Tree;

struct Tree* CreateTree(unsigned int nodesCount);
void DestroyTree(struct Tree* tree);
bool InputTree(struct Tree* tree);
void PrintTree(const struct Tree* tree);
unsigned char GetHeight(const struct Tree* tree);
