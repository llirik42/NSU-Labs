#include <stdbool.h>
#include <ctype.h>
#include "token.h"
#include "string_value_calculation.h"
#include "tokens_stack.h"
#include "tokens_utils.h"
#include "tokens_calculator.h"

#define charToDigit(c) ((unsigned char) ((c) - '0'))
#define PREV_VALUE_SYMBOL '$'

struct CalculationData{
    bool hasPrevValue;
    bool canReadString;

    char curChar;

    unsigned short curIndex;
    unsigned short bracketsBalanceCounter;

    int curNumber;

    double prevValue;

    const struct String string;

    struct Token curToken;
    struct Token prevToken;

    struct Stack operandsStack;
    struct Stack nonOperandsStack;

    struct TokensCalculator tokensCalculator;
};
struct CalculationData createCalculationData(const struct String string, const bool supportDouble,
                                             struct StatusCode prevCode){
    struct CalculationData result = {
            .hasPrevValue = isCodeSuccess(prevCode),
            .canReadString = true,
            .curChar = getStringChar(&string, 0),
            .curIndex = 0,
            .bracketsBalanceCounter = 0,
            .curNumber = 0,
            .prevValue = prevCode.resultValue,
            .string = string,
            .curToken = UNDEFINED_TOKEN,
            .prevToken = UNDEFINED_TOKEN,
            .operandsStack = createStack(),
            .nonOperandsStack = createStack(),
            .tokensCalculator = createTokensCalculator(supportDouble),
    };
    return result;
}

bool isCurCharPrevValueSymbol(struct CalculationData* data){
    return data->curChar == PREV_VALUE_SYMBOL;
}
bool isCurCharDigit(struct CalculationData* data){
    return isdigit(data->curChar);
}
bool isCurIndexLast(struct CalculationData* data){
    return data->curIndex == getStringLength(&(data->string)) - 1;
}
bool checkBracketsBalance(struct CalculationData* data){
    if (isLeftBracket(data->curToken)){
        data->bracketsBalanceCounter++;
        return true;
    }
    if (data->bracketsBalanceCounter > 0){
        data->bracketsBalanceCounter--;
        return true;
    }
    return false;
}
bool checkCurToken(const struct CalculationData* data){
    const struct Token prevToken = data->prevToken;
    const struct Token curToken = data->curToken;

    return isUndefined(prevToken) ||
           ((isOperand(curToken) || isLeftBracket(curToken)) && (isOperator(prevToken) || isLeftBracket(prevToken))) ||
           ((isOperator(curToken) || isRightBracket(curToken)) && (isOperand(prevToken) || isRightBracket(prevToken)));
}

void updateCurToken(struct CalculationData* data){
    data->curToken = isCurCharPrevValueSymbol(data) ? getOperandToken(data->prevValue) : getTokenByChar(data->curChar);
}
void updateCurNumber(struct CalculationData* data){
    data->curNumber = data->curNumber * 10 + charToDigit(data->curChar);
}
void move(struct CalculationData* data){
    data->curIndex++;
    data->curChar = getStringChar(&(data->string), data->curIndex);
    data->canReadString = data->curIndex < getStringLength(&(data->string));
}
void endIteration(struct CalculationData* data){
    data->prevToken = data->curToken;
    move(data);
}

struct StatusCode applyTopOperator(struct CalculationData* data){
    if (!isStackEmpty(&(data->nonOperandsStack)) && canApplyOperator(&(data->operandsStack))){
        struct Token operator = stackPop(&(data->nonOperandsStack));

        struct Token t1 = stackPop(&(data->operandsStack));
        struct Token t2 = stackPop(&(data->operandsStack));

        struct StatusCode codeOfApplying = data->tokensCalculator.calculateValue(operator, t2, t1);

        if (isCodeSuccess(codeOfApplying)){
            struct Token resultToken = statusCodeToToken(codeOfApplying);
            stackPush(&(data->operandsStack), resultToken);
        }

        return codeOfApplying;
    }
    return SYNTAX_ERROR;
}

struct StatusCode pushLowerOperators(struct CalculationData* data){
    while (!isStackEmpty(&(data->nonOperandsStack))){
        struct Token topToken = stackPeek(&(data->nonOperandsStack));

        if (isOperator(topToken)){
            if (comparePriority(data->curToken, topToken)){
                try(applyTopOperator(data))
                continue;
            }
        }
        break;
    }
    return SUCCESS_CODE;
}

struct StatusCode pushBeforeLeftBracket(struct CalculationData* data){
    while (1){
        struct Token curStackToken = stackPeek(&(data->nonOperandsStack));
        if (isLeftBracket(curStackToken)){
            stackPop(&(data->nonOperandsStack));
            break;
        }
        try(applyTopOperator(data))
    }
    return SUCCESS_CODE;
}

struct StatusCode handleOperatorToken(struct CalculationData* data){
    try(pushLowerOperators(data))
    stackPush(&(data->nonOperandsStack), data->curToken);
    return SUCCESS_CODE;
}

struct StatusCode handleBracketToken(struct CalculationData* data){
    if (!checkBracketsBalance(data)){
        return SYNTAX_ERROR;
    }
    if (isRightBracket(data->curToken)){
        try(pushBeforeLeftBracket(data))
    } else{
        stackPush(&(data->nonOperandsStack), data->curToken);
    }

    return SUCCESS_CODE;
}

struct StatusCode applyAllOperators(struct CalculationData* data){
    while (!isStackEmpty(&(data->nonOperandsStack))){
        try(applyTopOperator(data))
    }
    struct Token topOperand = stackPop(&(data->operandsStack));
    return tokenToStatusCode(topOperand);
}

struct StatusCode handleLastIndex(struct CalculationData* data){
    data->curToken = getOperandToken(data->curNumber);
    stackPush(&(data->operandsStack), data->curToken);
    if (!checkCurToken(data)){
        return SYNTAX_ERROR;
    }
    return SUCCESS_CODE;
}

struct StatusCode handleOperandToken(struct CalculationData* data){
    data->prevToken = getOperandToken(data->curNumber);
    stackPush(&(data->operandsStack), data->prevToken);
    data->curNumber = 0;
    return SUCCESS_CODE;
}

struct StatusCode handlePrevValueSymbol(struct CalculationData* data){
    if (!data->hasPrevValue){
        return NO_PREV_VALUE_ERROR;
    }
    data->curToken = getOperandToken(data->prevValue);
    stackPush(&(data->operandsStack), data->curToken);
    return SUCCESS_CODE;
}

struct StatusCode handleToken(struct CalculationData* data){
    const struct Token curToken = data->curToken;

    if (isOperator(curToken)){
        try(handleOperatorToken(data))
    }
    if (isBracket(curToken)){
        try(handleBracketToken(data))
    }
    if (isOperand(curToken)){
        try(handlePrevValueSymbol(data))
    }
    return SUCCESS_CODE;
}

struct StatusCode readString(struct CalculationData* data){
    while (data->canReadString){
        while (isCurCharDigit(data)){
            updateCurNumber(data);

            if (isCurIndexLast(data)){
                try(handleLastIndex(data))
                return SUCCESS_CODE;
            }

            move(data);

            if (!isCurCharDigit(data)){
                try(handleOperandToken(data))
                break;
            }
        }

        updateCurToken(data);

        if (!checkCurToken(data)){
            return SYNTAX_ERROR;
        }

        try(handleToken(data))

        endIteration(data);
    }

    return SUCCESS_CODE;
}

struct StatusCode calculateStringValue(const struct String string, const bool supportDouble,
                                       const struct StatusCode prevCode){
    if (isStringEmpty(&string)){
        return SYNTAX_ERROR;
    }

    struct CalculationData data = createCalculationData(string, supportDouble, prevCode);

    try(readString(&data))

    const struct StatusCode applyingCode = applyAllOperators(&data);

    return applyingCode;
}