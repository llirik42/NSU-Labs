#include "../utils/utils.h"
#include "string_parsing.h"
#include "parsed_data.h"
#include "tokens_utils.h"

bool checkCurToken(const struct Token prevToken, const struct Token curToken){
    if (IsTokenUndefined(prevToken)){
        return true;
    }

    if (IsTokenNumber(curToken)){
        return !IsTokenFunction(prevToken) && !IsTokenRightBracket(prevToken) && !IsTokenVariable(prevToken);
    }

    if (IsTokenVariable(curToken) || IsTokenFunction(curToken)){
        return !IsTokenFunction(prevToken) && !IsTokenOperand(prevToken) && !IsTokenRightBracket(prevToken);
    }

    if (IsTokenOperator(curToken) || IsTokenRightBracket(curToken)){
        return !IsTokenFunction(prevToken) && !IsTokenOperator(prevToken) && !IsTokenLeftBracket(prevToken);
    }

    if (IsTokenLeftBracket(curToken)){
        return !IsTokenOperand(prevToken) && !IsTokenRightBracket(prevToken);
    }

    return true;
}

struct ParsedData ParseString(const char* string){
    if (!string){
        return EMPTY_PARSED_DATA;
    }

    struct TokensArray outputTokens = CreateArray();

    struct TokensArray tmpArray = CreateArray();

    const unsigned int length = GetStringLength(string);
    unsigned int shift = 0;

    struct Token prevToken = UNDEFINED_TOKEN;
    struct Token curToken;

    unsigned int bracketsBalanceCounter = 0;

    while (shift < length){
        curToken = GetTokenFromString(string, &shift);

        if (IsTokenUndefined(curToken)){
            break;
        }

        if (!checkCurToken(prevToken, curToken)){
            return EMPTY_PARSED_DATA;
        }

        if (IsTokenOperand(curToken)){
            Push(&outputTokens, curToken);
        }

        if (IsTokenFunction(curToken)){
            Push(&tmpArray, curToken);
        }

        if (IsTokenOperator(curToken)){
            while (!IsArrayEmpty(&tmpArray)){
                struct Token topToken = Peek(&tmpArray);

                if (IsTokenOperator(topToken)){
                    if (ComparePriority(curToken, topToken)){
                        Push(&outputTokens, Pop(&tmpArray));
                        continue;
                    }
                }
                break;
            }

            Push(&tmpArray, curToken);
        }

        if (IsTokenLeftBracket(curToken)){
            bracketsBalanceCounter++;

            Push(&tmpArray, curToken);
        }

        if (IsTokenRightBracket(curToken)){
            if (bracketsBalanceCounter == 0){
                return EMPTY_PARSED_DATA;
            }
            bracketsBalanceCounter--;

            struct Token topToken = Pop(&tmpArray);

            while (!IsTokenLeftBracket(topToken)){
                Push(&outputTokens, topToken);
                topToken = Pop(&tmpArray);
            }

            if (IsTokenFunction(Peek(&tmpArray))){
                Push(&outputTokens, Pop(&tmpArray));
            }
        }

        prevToken = curToken;
    }

    if (bracketsBalanceCounter != 0){
        return EMPTY_PARSED_DATA;
    }

    while (!IsArrayEmpty(&tmpArray)){
        Push(&outputTokens, Pop(&tmpArray));
    }

    return IsArrayEmpty(&outputTokens) ? EMPTY_PARSED_DATA : CreateParsedData(outputTokens);
}
