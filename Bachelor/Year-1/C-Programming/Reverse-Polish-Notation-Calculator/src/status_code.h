#pragma once

#include <stdbool.h>

#define isCodeSuccess(code) ((code).type == SUCCESS)
#define try(code) { struct StatusCode tmpCode = code; if(!isCodeSuccess(tmpCode)) {return tmpCode;} }

enum CodeType{
    SUCCESS,
    ERROR,
};
struct StatusCode{
    enum CodeType type;
    char* errorMessage;
    double resultValue;
};

struct StatusCode createCode(double resultValue);
void printCode(struct StatusCode code);

static const struct StatusCode SUCCESS_CODE = {
        .type = SUCCESS
};
static const struct StatusCode SYNTAX_ERROR = {
        .type = ERROR,
        .errorMessage = "syntax error"
};
static const struct StatusCode NO_PREV_VALUE_ERROR = {
        .type = ERROR,
        .errorMessage = "no previous value to use"
};
static const struct StatusCode DIVISION_BY_ZERO_ERROR = {
        .type = ERROR, .errorMessage = "division by zero"};
static const struct StatusCode ZERO_TO_ZERO_OR_LESS_POWER_ERROR = {
        .type = ERROR,
        .errorMessage = "0^a where a <= 0"
};
static const struct StatusCode NEGATIVE_TO_NON_INT_POWER_ERROR = {
        .type = ERROR,
        .errorMessage = "a^b where a < 0 and b isn't an integer"
};
