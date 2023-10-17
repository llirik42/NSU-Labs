#pragma once

#include "../utils/utils.h"

#define MAX_NOTATIONS_COUNT 2

enum TokenType{
    UNDEFINED,
    NUMBER,
    VARIABLE,
    FUNCTION,
    LEFT_ASSOCIATIVE_OPERATOR,
    RIGHT_ASSOCIATIVE_OPERATOR,
    LEFT_BRACKET,
    RIGHT_BRACKET
};
struct Token{
    enum TokenType type;
    double value;
    char* notations[MAX_NOTATIONS_COUNT];
    unsigned int notationsCount;
    struct AdvancedValue(*operation1)(const struct Token);
    struct AdvancedValue(*operation2)(const struct Token, const struct Token);
    unsigned char priority;
};

static const struct Token UNDEFINED_TOKEN = {.type = UNDEFINED};
