#pragma once

#include "status_code.h"
#include "token.h"
#include <stdbool.h>

bool isUndefined(struct Token token);
bool isOperand(struct Token token);
bool isLeftBracket(struct Token token);
bool isRightBracket(struct Token token);
bool isBracket(struct Token token);
bool isOperator(struct Token token);
bool comparePriority(struct Token operator1, struct Token operator2);

struct Token getTokenByChar(char c);
struct Token getOperandToken(double operand);
struct Token statusCodeToToken(struct StatusCode statusCode);

struct StatusCode tokenToStatusCode(struct Token token);
