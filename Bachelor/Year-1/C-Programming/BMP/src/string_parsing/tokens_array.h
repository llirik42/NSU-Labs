#pragma once

#include "../utils/utils.h"
#include "token.h"

#define MAX_ARRAY_SIZE 32

struct TokensArray{
    unsigned int length;
    struct Token tokens[MAX_ARRAY_SIZE];
};

struct TokensArray CreateArray();
bool IsArrayEmpty(const struct TokensArray* tokensArray);
void Push(struct TokensArray* tokensArray, struct Token token);
struct Token Peek(const struct TokensArray* tokensArray);
struct Token Pop(struct TokensArray* tokensArray);
unsigned int GetArrayLength(const struct TokensArray* tokensArray);
struct Token GetArrayElement(const struct TokensArray* tokensArray, unsigned int index);
void DeleteArrayElement(struct TokensArray* tokensArray, unsigned int elementIndex);
void SetArrayElement(struct TokensArray* tokensArray, unsigned int index, struct Token token);
