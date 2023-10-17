#pragma once

#include "token.h"

#define MAX_STACK_SIZE 1024

struct Stack{
    struct Token tokens[MAX_STACK_SIZE];
    unsigned int length;
};

struct Stack createStack();

bool isStackEmpty(const struct Stack* stack);
bool canApplyOperator(const struct Stack* stack);

void stackPush(struct Stack* stack, struct Token value);

struct Token stackPeek(const struct Stack* stack);
struct Token stackPop(struct Stack* stack);
