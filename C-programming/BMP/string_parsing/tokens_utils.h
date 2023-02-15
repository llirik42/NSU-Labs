#pragma once

#include "token.h"
#include <stdbool.h>
#include "string_parsing.h"

bool IsTokenUndefined(struct Token token);
bool IsTokenFunction(struct Token token);
bool IsTokenVariable(struct Token token);
bool IsTokenOperator(struct Token token);
bool IsTokenNumber(struct Token token);
bool IsTokenOperand(struct Token token);
bool IsTokenLeftBracket(struct Token token);
bool IsTokenRightBracket(struct Token token);
bool ComparePriority(struct Token operator1, struct Token operator2);
struct Token GetTokenFromString(const char* string, unsigned int* shift);
struct Token AdvancedValueToToken(struct AdvancedValue advancedValue);
