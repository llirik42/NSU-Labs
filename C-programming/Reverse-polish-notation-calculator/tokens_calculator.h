#pragma once

#include "token.h"
#include "status_code.h"
#include <stdbool.h>

struct TokensCalculator{
    struct StatusCode(*calculateValue)(struct Token, struct Token, struct Token);
};

struct TokensCalculator createTokensCalculator(bool supportDouble);
