#include "../utils/utils.h"
#include "string_parsing.h"
#include "tokens_utils.h"
#include "reserved_tokens.h"
#include <math.h>

struct Token createOperandToken(const double operand){
    struct Token result = {.type = NUMBER, .value = operand};
    return result;
}
void handleOperand(struct Token* token, const double value){
    if (token->type == VARIABLE){
        token->value = value;
    }
}
struct Token findTokenByNotation(const char* string, unsigned int* shift){
    for (unsigned char i = 0; i < RESERVED_TOKENS_COUNT; i++){
        struct Token curOperator = RESERVED_TOKENS[i];

        for (unsigned int j = 0; j < curOperator.notationsCount; j++){
            const char* curNotation = curOperator.notations[j];
            const unsigned int curNotationLength = GetStringLength(curNotation);

            if (CompareStringsSoftly(curNotation, string, curNotationLength)){
                (*shift) += curNotationLength;
                return curOperator;
            }
        }
    }
    return UNDEFINED_TOKEN;
}
struct AdvancedValue getNumberFromString(const char* string, unsigned int* shift){
    bool negative = string[0] == '-';
    const unsigned int startIndex = negative ? 1 : 0;

    (*shift) += startIndex;

    if (!IsCharDigit(string[startIndex])){
        return UNDEFINED_VALUE;
    }

    double result = 0;
    int curPow = -1;
    unsigned int curIndex = startIndex;
    char curChar = ' ';
    bool metPoint = false;
    bool isNonInteger = false;

    while(1){
        curChar = string[curIndex++];

        if (curChar != '.' && !IsCharDigit(curChar)){
            break;
        }

        if (curChar == '.'){
            if (metPoint){
                return UNDEFINED_VALUE;
            }
            metPoint = true;
            continue;
        }

        if (IsCharDigit(curChar)){
            (*shift)++;

            const unsigned int curDigit = CharToDigit(curChar);

            if (!metPoint){
                result = result * 10 + curDigit;
                continue;
            }
            isNonInteger = true;
            result += pow(10, curPow--) * curDigit;
            continue;
        }
    }

    if (isNonInteger){
        (*shift)++;
    }

    if (negative){
        result *= -1;
    }
    return CreateAdvancedValue(result);
}

bool IsTokenUndefined(const struct Token token){
    return token.type == UNDEFINED;
}
bool IsTokenFunction(const struct Token token){
    return token.type == FUNCTION;
}
bool IsTokenVariable(const struct Token token){
    return token.type == VARIABLE;
}
bool IsTokenOperator(const struct Token token){
    return token.type == LEFT_ASSOCIATIVE_OPERATOR || token.type == RIGHT_ASSOCIATIVE_OPERATOR;
}
bool IsTokenNumber(struct Token token){
    return token.type == NUMBER;
}
bool IsTokenOperand(const struct Token token){
    return token.type == VARIABLE || token.type == NUMBER;
}
bool IsTokenLeftBracket(struct Token token){
    return token.type == LEFT_BRACKET;
}
bool IsTokenRightBracket(struct Token token){
    return token.type == RIGHT_BRACKET;
}
bool IsParsedDataEmpty(const struct ParsedData parsedData){
    return parsedData.isEmpty;
}
bool ComparePriority(const struct Token operator1, const struct Token operator2){
    if (operator2.type == LEFT_ASSOCIATIVE_OPERATOR){
        return operator1.priority <= operator2.priority;
    }
    return operator1.priority < operator2.priority;
}
struct Token GetTokenFromString(const char* string, unsigned int* shift){
    const char* shiftedString = string + *shift;

    const struct Token nonNumberToken = findTokenByNotation(shiftedString, shift);

    if (!IsTokenUndefined(nonNumberToken)){
        return nonNumberToken;
    }

    return AdvancedValueToToken(getNumberFromString(shiftedString, shift));
}
struct Token AdvancedValueToToken(const struct AdvancedValue advancedValue){
    return IsUndefinedValue(advancedValue) ? UNDEFINED_TOKEN : createOperandToken(advancedValue.value);
}
struct AdvancedValue CalculateParsedDataValue(struct ParsedData parsedData, double variableValue){
    struct TokensArray tokens = parsedData.tokensArray;

    for (unsigned int i = 1; i < GetArrayLength(&tokens); i++){
        const struct Token curToken = GetArrayElement(&tokens, i);

        if (IsTokenFunction(curToken)){
            DeleteArrayElement(&tokens, i);
            struct Token operand = GetArrayElement(&tokens, i - 1);

            handleOperand(&operand, variableValue);

            const struct AdvancedValue operationsResult = curToken.operation1(operand);

            if (IsUndefinedValue(operationsResult)){
                return UNDEFINED_VALUE;
            }

            SetArrayElement(&tokens, i - 1, AdvancedValueToToken(operationsResult));
            i--;
        }

        if (IsTokenOperator(curToken)){
            DeleteArrayElement(&tokens, i);
            struct Token operand1 = GetArrayElement(&tokens, i - 1);
            struct Token operand2 = GetArrayElement(&tokens, i - 2);

            handleOperand(&operand1, variableValue);
            handleOperand(&operand2, variableValue);

            const struct AdvancedValue operationsResult = curToken.operation2(operand2, operand1);

            if (IsUndefinedValue(operationsResult)){
                return UNDEFINED_VALUE;
            }

            DeleteArrayElement(&tokens, i - 1);
            SetArrayElement(&tokens, i - 2, AdvancedValueToToken(operationsResult));
            i -= 2;
        }
    }

    const struct Token result = Pop(&tokens);

    if (IsTokenUndefined(result)){
        return UNDEFINED_VALUE;
    }

    return IsTokenVariable(result) ? CreateAdvancedValue(variableValue) : CreateAdvancedValue(result.value);
}
