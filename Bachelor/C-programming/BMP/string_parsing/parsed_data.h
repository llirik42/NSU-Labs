#pragma once

#include "token.h"

struct ParsedData CreateParsedData(struct TokensArray tokensArray);

static const struct ParsedData EMPTY_PARSED_DATA = {.isEmpty = true};
