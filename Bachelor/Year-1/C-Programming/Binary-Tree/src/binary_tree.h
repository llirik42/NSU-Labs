#pragma once

#include <stdbool.h>

struct Tree {
    int data;

    struct Tree* left;
    struct Tree* right;
};

size_t getHeight(const struct Tree* tree);

bool add(struct Tree** tree, int data);

struct Tree* find(struct Tree* root, int data);

size_t countNodes(const struct Tree* root);

void destroyTree(struct Tree* root);
