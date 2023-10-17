#pragma once

#include <stdbool.h>

#define MAX_STACK_SIZE 100

typedef int StackElement;

struct Stack{
    size_t length;

    bool isEmpty;

    StackElement elements[MAX_STACK_SIZE];
};

void initStack(struct Stack* stack);

void printStack(struct Stack* stack);

void push(struct Stack* stack, StackElement element);

StackElement peek(struct Stack* stack);

StackElement pop(struct Stack* stack);

size_t stackSize(struct Stack* stack);
