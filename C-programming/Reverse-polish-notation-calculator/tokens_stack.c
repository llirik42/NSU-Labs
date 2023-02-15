#include "tokens_stack.h"

#define DEFAULT_VALUE UNDEFINED_TOKEN

struct Stack createStack(){
    struct Stack result = {.length = 0};
    return result;
}

bool isStackEmpty(const struct Stack* stack){
    return !stack || stack->length == 0;
}
bool canApplyOperator(const struct Stack* stack){
    return !stack || stack->length >= 2;
}

void stackPush(struct Stack* stack, const struct Token value){
    if (!stack){
        return;
    }

    if (stack->length < MAX_STACK_SIZE){
        stack->tokens[stack->length] = value;
        stack->length++;
    }
}

struct Token stackPeek(const struct Stack* stack){
    if (isStackEmpty(stack)){
        return DEFAULT_VALUE;
    }
    return stack->tokens[stack->length - 1];
}
struct Token stackPop(struct Stack* stack){
    if (isStackEmpty(stack)){
        return DEFAULT_VALUE;
    }

    const struct Token result = stackPeek(stack);
    stack->length--;
    return result;
}
