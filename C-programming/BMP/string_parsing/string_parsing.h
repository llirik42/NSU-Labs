#pragma once

#include "../utils/utils.h"
#include "tokens_array.h"
#include <stdbool.h>

struct ParsedData{
    struct TokensArray tokensArray;
    bool isEmpty;
};

struct ParsedData ParseString(const char* string);
bool IsParsedDataEmpty(struct ParsedData parsedData);
struct AdvancedValue CalculateParsedDataValue(struct ParsedData parsedData, double variableValue);
