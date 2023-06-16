#pragma once

#include "../utils.h"
#include <stdbool.h>
#include <stdio.h>

#define IS_NODE_LEAF(NODE) (!(NODE->leftChild))
#define GET_ROOT(TREE) ((TREE)->root)
#define GET_CHILD(NODE, RIGHT) ((RIGHT) ? (NODE)->rightChild : (NODE)->leftChild)
#define GET_LEAF_BYTE(LEAF) ((LEAF)->byte)

struct Node{
    Byte byte;
    bool isLeftChild;
    unsigned long long frequency;
    struct Node* leftChild;
    struct Node* rightChild;
    struct Node* parent;
};
struct Tree{
    struct Node* nodes;
    struct Node* root;
    unsigned short nodesCount;
    unsigned short maxLeavesCount;
};

struct Tree* CreateEmptyTree(unsigned short uniqueCharsCounter);
Status InputTree(FILE* in, struct Tree** tree);
Status FillTreeByFrequencies(struct Tree* tree, const unsigned long long* frequencies);
void FillCodes(const struct Tree* tree, Code codes[]);
Status PrintTree(const struct Tree* tree, FILE* out);
void DestroyTree(struct Tree* tree);
