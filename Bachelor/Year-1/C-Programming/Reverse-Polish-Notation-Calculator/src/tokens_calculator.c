#include "tokens_calculator.h"

struct StatusCode doubleCalculation(const struct Token operator, const struct Token operand1,
                                    const struct Token operand2){
    return operator.calculateValue(operand1, operand2);
}
struct StatusCode integerCalculation(const struct Token operator, const struct Token operand1,
                                     const struct Token operand2){
    struct StatusCode codeOfApplying = operator.calculateValue(operand1, operand2);
    if (isCodeSuccess(codeOfApplying)){
        codeOfApplying.resultValue = (int) codeOfApplying.resultValue;
    }
    return codeOfApplying;
}

struct TokensCalculator createTokensCalculator(bool supportDouble){
    struct TokensCalculator result = {
            .calculateValue = supportDouble ? &doubleCalculation : &integerCalculation
    };

    return result;
}
