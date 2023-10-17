#include <stdbool.h>
#include <stdio.h>
#include "stack.h"


#define ELEMENT_DEFAULT_VALUE 0

typedef int StackElement;

bool checkNull(void* s){
    if (s == NULL){
        printf("Null error\n");
        return false;
    }

    return true;
}

void initStack(struct Stack* stack){
    stack->length = 0;
    stack->isEmpty = true;
}

void printStack(struct Stack* stack){
    if (!checkNull(stack)){
        return;
    }

    if (stack->isEmpty){
        printf("Empty error\n");
        return;
    }

    for (size_t index = 0; index < stack->length; index++){
        printf("%d ", stack->elements[index]);
    }
    printf("\n");
}

void push(struct Stack* stack, StackElement element){
    if (!checkNull(stack)){
        return;
    }

    if (stack->length == MAX_STACK_SIZE){
        printf("Overflow error\n");
        return;
    }

    stack->elements[stack->length] = element;
    stack->length++;
    stack->isEmpty = false;
}

StackElement peek(struct Stack* stack){
    if (!checkNull(stack)){
        return ELEMENT_DEFAULT_VALUE;
    }

    if (stack->isEmpty){
        printf("Empty stack error\n");
        return ELEMENT_DEFAULT_VALUE;
    }

    return stack->elements[stack->length - 1];
}

StackElement pop(struct Stack* stack){
    if (!checkNull(stack)){
        return ELEMENT_DEFAULT_VALUE;
    }

    if (stack->isEmpty){
        printf("Empty stack error\n");
        return ELEMENT_DEFAULT_VALUE;
    }

    StackElement result = peek(stack);

    stack->length--;

    return result;
}

size_t stackSize(struct Stack* stack) {
    if (!checkNull(stack)){
        return 0;
    }

    return stack->length;
}
