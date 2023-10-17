#pragma once

#include "status_code.h"

enum TokenType{
    UNDEFINED,
    OPERAND,
    LEFT_ASSOCIATIVE_OPERATOR,
    RIGHT_ASSOCIATIVE_OPERATOR,
    LEFT_BRACKET,
    RIGHT_BRACKET
};
struct Token{
    enum TokenType type;
    char notation;
    double value;
    struct StatusCode(*calculateValue)(const struct Token, const struct Token);
    unsigned char priority;
};

static const struct Token UNDEFINED_TOKEN = {.type = UNDEFINED};
