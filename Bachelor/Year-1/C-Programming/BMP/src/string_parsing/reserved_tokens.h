#pragma once

#include "token.h"
#include "tokens_operations.h"



static const struct Token RESERVED_TOKENS[] = {
        {.type = NUMBER, .value = PI_NUMBER, .notations = "pi", .notationsCount = 1},
        {.type = NUMBER, .value = E_NUMBER, .notations = "e", .notationsCount = 1},
        {.type = VARIABLE, .notations = {"x"}, .notationsCount = 1},
        {.type = LEFT_BRACKET, .notations = {"("}, .notationsCount = 1},
        {.type = RIGHT_BRACKET, .notations = {")"}, .notationsCount = 1},
        {.type = LEFT_ASSOCIATIVE_OPERATOR, .priority = 1, .operation2 = &Sum, .notations = {"+"}, .notationsCount = 1},
        {.type = LEFT_ASSOCIATIVE_OPERATOR, .priority = 1, .operation2 = &Sub, .notations = {"-"}, .notationsCount = 1},
        {.type = LEFT_ASSOCIATIVE_OPERATOR, .priority = 2, .operation2 = &Mul, .notations = {"*"}, .notationsCount = 1},
        {.type = LEFT_ASSOCIATIVE_OPERATOR, .priority = 2, .operation2 = &Div, .notations = {"/"}, .notationsCount = 1},
        {.type = RIGHT_ASSOCIATIVE_OPERATOR, .priority = 3, .operation2 = &Pow, .notations = {"^"}, .notationsCount = 1},
        {.type = FUNCTION, .operation1 = &Sin, .notations = {"sin"}, .notationsCount = 1},
        {.type = FUNCTION, .operation1 = &Cos, .notations = {"cos"}, .notationsCount = 1},
        {.type = FUNCTION, .operation1 = &Tg, .notations = {"tan", "tg"}, .notationsCount = 2},
        {.type = FUNCTION, .operation1 = &Ctg, .notations = {"cot", "ctg"}, .notationsCount = 2},
        {.type = FUNCTION, .operation1 = &Arcsin, .notations = {"arcsin"}, .notationsCount = 1},
        {.type = FUNCTION, .operation1 = &Arccos, .notations = {"arccos"}, .notationsCount = 1},
        {.type = FUNCTION, .operation1 = &Arctg, .notations = {"arctg", "arctan"}, .notationsCount = 2},
        {.type = FUNCTION, .operation1 = &Arcctg, .notations = {"arcctg", "arccot"}, .notationsCount = 2},
        {.type = FUNCTION, .operation1 = &Sqrt, .notations = {"sqrt"}, .notationsCount = 1},
        {.type = FUNCTION, .operation1 = &Ln, .notations = {"ln"}, .notationsCount = 1},
        {.type = FUNCTION, .operation2 = &Log, .notations = {"log"}, .notationsCount = 1},
};
const unsigned char RESERVED_TOKENS_COUNT = sizeof(RESERVED_TOKENS) / sizeof(RESERVED_TOKENS[0]);
