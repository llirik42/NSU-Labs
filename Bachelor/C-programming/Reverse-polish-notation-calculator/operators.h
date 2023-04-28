#pragma once

#include "token.h"
#include "tokens_operations.h"

static const struct Token OPERATORS[] = {
        {.type = LEFT_ASSOCIATIVE_OPERATOR, .priority = 1, .calculateValue = &tokenSum, .notation = '+'},
        {.type = LEFT_ASSOCIATIVE_OPERATOR, .priority = 1, .calculateValue = &tokenSub, .notation = '-'},
        {.type = LEFT_ASSOCIATIVE_OPERATOR, .priority = 2, .calculateValue = &tokenMul, .notation = '*'},
        {.type = LEFT_ASSOCIATIVE_OPERATOR, .priority = 2, .calculateValue = &tokenDiv, .notation = '/'},
        {.type = RIGHT_ASSOCIATIVE_OPERATOR, .priority = 3, .calculateValue = &tokenPow, .notation = '^'}
};
unsigned char OPERATORS_COUNT = sizeof(OPERATORS) / sizeof(OPERATORS[0]);
