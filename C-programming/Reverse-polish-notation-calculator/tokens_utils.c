#include "tokens_utils.h"
#include "operators.h"
#include "brackets.h"


bool isUndefined(const struct Token token){
    return token.type == UNDEFINED;
}
bool isOperand(const struct Token token){
    return token.type == OPERAND;
}
bool isLeftBracket(const struct Token token){
    return token.type == LEFT_BRACKET;
}
bool isRightBracket(const struct Token token){
    return token.type == RIGHT_BRACKET;
}
bool isBracket(const struct Token token){
    return isLeftBracket(token) || isRightBracket(token);
}
bool isOperator(const struct Token token){
    enum TokenType type = token.type;
    return type == LEFT_ASSOCIATIVE_OPERATOR || type == RIGHT_ASSOCIATIVE_OPERATOR;
}
bool comparePriority(const struct Token operator1, const struct Token operator2){
    if (operator2.type == LEFT_ASSOCIATIVE_OPERATOR){
        return operator1.priority <= operator2.priority;
    }
    return operator1.priority < operator2.priority;
}

struct Token findTokenByNotation(const char notation, const struct Token* array,
                                 const unsigned char length){
    for (unsigned char index = 0; index < length; index++){
        struct Token curOperator = array[index];
        if (curOperator.notation == notation){
            return curOperator;
        }
    }
    return UNDEFINED_TOKEN;
}
struct Token getTokenByChar(const char c){
    struct Token result = findTokenByNotation(c, OPERATORS, OPERATORS_COUNT);
    if (isUndefined(result)){
        result = findTokenByNotation(c, BRACKETS, BRACKETS_COUNT);
    }
    return result;
}
struct Token getOperandToken(const double operand){
    struct Token result = {.type = OPERAND, .value = operand};
    return result;
}
struct Token statusCodeToToken(const struct StatusCode statusCode){
    if (!isCodeSuccess(statusCode)){
        return UNDEFINED_TOKEN;
    }
    return getOperandToken(statusCode.resultValue);
}

struct StatusCode tokenToStatusCode(const struct Token token){
    if (isOperand(token)){
        return createCode(token.value);
    }
    return SYNTAX_ERROR;
}
