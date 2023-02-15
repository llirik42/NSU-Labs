#include "tokens_operations.h"
#include <math.h>
#include <stdbool.h>
#include "tokens_utils.h"


#define EPSILON 0.001

bool isAngleCorrectForTan(const double a){
    const double piTimes = a / PI_NUMBER;
    const double piTimeRoundedDown = floor(piTimes);

    if (fabs(piTimes - piTimeRoundedDown) < EPSILON){
        return true;
    }

    const double piHalfTimes = a / (PI_NUMBER / 2);
    const double piHalfTimesRoundedDown = floor(piHalfTimes);

    return fabs(piHalfTimes - piHalfTimesRoundedDown) > EPSILON;
}
bool isAngleCorrectForCot(const double a){
    const double piHalfTimes = a / PI_NUMBER;
    const double piHalfTimesRoundedDown = floor(piHalfTimes);

    return fabs(piHalfTimes - piHalfTimesRoundedDown) > EPSILON;
}

double acot(const double x){
    return PI_NUMBER / 2 - atan(x);
}
double cot(const double x){
    return 1/ tan(x);
}

OperationResult Sum(BINARY_ARGS){
    return CreateAdvancedValue(operand1.value + operand2.value);
}
OperationResult Sub(BINARY_ARGS){
    return CreateAdvancedValue(operand1.value - operand2.value);
}
OperationResult Mul(BINARY_ARGS){
    return CreateAdvancedValue(operand1.value * operand2.value);
}
OperationResult Div(BINARY_ARGS){
    double divisorValue = operand2.value;
    return divisorValue == 0 ? UNDEFINED_VALUE : CreateAdvancedValue(operand1.value / divisorValue);
}
OperationResult Pow(BINARY_ARGS){
    double base = operand1.value;
    double power = operand2.value;

    bool isPowerInteger = power == (int) power;

    if ((base == 0 && power <= 0) || (base < 0 && !isPowerInteger)){
        return UNDEFINED_VALUE;
    }
    return CreateAdvancedValue(pow(base, power));
}
OperationResult Sin(UNARY_ARGS){
    return CreateAdvancedValue(sin(operand.value));
}
OperationResult Cos(UNARY_ARGS){
    return CreateAdvancedValue(cos(operand.value));
}
OperationResult Tg(UNARY_ARGS){
    return isAngleCorrectForTan(operand.value) ? CreateAdvancedValue(tan(operand.value)) : UNDEFINED_VALUE;
}
OperationResult Ctg(UNARY_ARGS){
    return isAngleCorrectForCot(operand.value) ? CreateAdvancedValue(cot(operand.value)) : UNDEFINED_VALUE;
}
OperationResult Arcsin(UNARY_ARGS){
    const double operandValue = operand.value;
    return fabs(operandValue) <= 1 ? CreateAdvancedValue(asin(operandValue)) : UNDEFINED_VALUE;
}
OperationResult Arccos(UNARY_ARGS){
    const double operandValue = operand.value;
    return fabs(operandValue) <= 1 ? CreateAdvancedValue(acos(operandValue)) : UNDEFINED_VALUE;
}
OperationResult Arctg(UNARY_ARGS){
    return CreateAdvancedValue(atan(operand.value));
}
OperationResult Arcctg(UNARY_ARGS){
    return CreateAdvancedValue(acot(operand.value));
}
OperationResult Sqrt(UNARY_ARGS){
    const double operandValue = operand.value;
    return operandValue >= 0 ? CreateAdvancedValue(sqrt(operandValue)) : UNDEFINED_VALUE;
}
OperationResult Ln(UNARY_ARGS){
    const double operandValue = operand.value;
    return operandValue > 0 ? CreateAdvancedValue(log(operandValue)) : UNDEFINED_VALUE;
}
OperationResult Log(BINARY_ARGS){
    const struct AdvancedValue tmp1 = Ln(operand1);
    const struct AdvancedValue tmp2 = Ln(operand2);

    if (IsUndefinedValue(tmp1) || IsUndefinedValue(tmp2)){
        return UNDEFINED_VALUE;
    }

    return Div(AdvancedValueToToken(tmp2), AdvancedValueToToken(tmp1));
}
