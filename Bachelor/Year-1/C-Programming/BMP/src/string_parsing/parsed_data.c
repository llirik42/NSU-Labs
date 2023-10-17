#include "tokens_array.h"
#include "string_parsing.h"
#include "parsed_data.h"

struct ParsedData CreateParsedData(struct TokensArray tokensArray){
    struct ParsedData result = {.isEmpty = false, .tokensArray = tokensArray};
    return result;
}
