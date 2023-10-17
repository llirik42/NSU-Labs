#include "tokens_utils.h"

struct TokensArray CreateArray(){
    struct TokensArray result = {.length = 0};
    return result;
}
bool IsArrayEmpty(const struct TokensArray* tokensArray){
    return !tokensArray || tokensArray->length == 0;
}
void Push(struct TokensArray* tokensArray, const struct Token value){
    if (!tokensArray){
        return;
    }

    if (tokensArray->length < MAX_ARRAY_SIZE){
        tokensArray->tokens[tokensArray->length] = value;
        tokensArray->length++;
    }
}
struct Token Peek(const struct TokensArray* tokensArray){
    return IsArrayEmpty(tokensArray) ? UNDEFINED_TOKEN : tokensArray->tokens[tokensArray->length - 1];
}
struct Token Pop(struct TokensArray* tokensArray){
    if (IsArrayEmpty(tokensArray)){
        return UNDEFINED_TOKEN;
    }

    const struct Token result = Peek(tokensArray);
    tokensArray->length--;
    return result;
}
unsigned int GetArrayLength(const struct TokensArray* tokensArray){
    return IsArrayEmpty(tokensArray) ? 0 : tokensArray->length;
}
struct Token GetArrayElement(const struct TokensArray* tokensArray, const unsigned int index){
    return IsArrayEmpty(tokensArray) ? UNDEFINED_TOKEN : tokensArray->tokens[index];
}
void DeleteArrayElement(struct TokensArray* tokensArray, const unsigned int elementIndex){
    if (!tokensArray || elementIndex >= tokensArray->length){
        return;
    }

    if (elementIndex < tokensArray->length - 1){
        for (unsigned int i = elementIndex; i < tokensArray->length - 1; i++){
            tokensArray->tokens[i] = tokensArray->tokens[i + 1];
        }
    }

    tokensArray->tokens[tokensArray->length - 1] = UNDEFINED_TOKEN;

    tokensArray->length--;
}
void SetArrayElement(struct TokensArray* tokensArray, const unsigned int index, const struct Token token){
    if (!tokensArray || index >= tokensArray->length){
        return;
    }

    tokensArray->tokens[index] = token;
}
