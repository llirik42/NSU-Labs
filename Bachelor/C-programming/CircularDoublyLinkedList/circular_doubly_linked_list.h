#pragma once

#include <stdlib.h>

struct Node {
    size_t value;

    struct Node* prev;
    struct Node* next;
};

struct CDList {
    struct Node* first;

    size_t length;
};

struct CDList* createNewList(size_t length);

void printList(struct CDList* list);

void freeList(struct CDList* list);

size_t count(size_t N, int K);
