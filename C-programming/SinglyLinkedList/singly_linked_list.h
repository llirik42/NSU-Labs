#pragma once

#include <stdbool.h>

struct SinglyLinkedList {
    struct Node* first;

    struct Node* last;

    size_t length;

    bool isEmpty;
};

struct SinglyLinkedList* createNewList();

void append(struct SinglyLinkedList* list, size_t value);

void printList(struct SinglyLinkedList* list);

void freeList(struct SinglyLinkedList* list);