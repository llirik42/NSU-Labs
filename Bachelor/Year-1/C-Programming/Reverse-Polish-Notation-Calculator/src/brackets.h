#pragma once

#include "token.h"

static const struct Token BRACKETS[] = {
        {.type = LEFT_BRACKET, .notation = '('},
        {.type = RIGHT_BRACKET, .notation = ')'}
};
const unsigned char BRACKETS_COUNT = sizeof(BRACKETS) / sizeof(BRACKETS[0]);
