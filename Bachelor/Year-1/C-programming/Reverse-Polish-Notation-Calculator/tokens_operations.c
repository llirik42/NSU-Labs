#include "tokens_operations.h"
#include <math.h>
#include <stdbool.h>

#define EPSILON 0.000001

bool isZero(const double number){
    return fabs(number) < EPSILON;
}
bool isLessThanZero(const double number){
    return number < -EPSILON;
}
bool isLessOrEqualZero(const double number){
    return number < EPSILON;
}

struct StatusCode tokenSum(const struct Token operand1, const struct Token operand2){
    return createCode(operand1.value + operand2.value);
}

struct StatusCode tokenSub(const struct Token operand1, const struct Token operand2){
    return createCode(operand1.value - operand2.value);
}

struct StatusCode tokenMul(const struct Token operand1, const struct Token operand2){
    return createCode(operand1.value * operand2.value);
}

struct StatusCode tokenDiv(const struct Token operand1, const struct Token operand2){
    double divisorValue = operand2.value;

    if (isZero(divisorValue)){
        return DIVISION_BY_ZERO_ERROR;
    }
    return createCode(operand1.value / divisorValue);
}

struct StatusCode tokenPow(const struct Token operand1, const struct Token operand2){
    double base = operand1.value;
    double power = operand2.value;

    bool isPowerInteger = power == (int) power;

    if (isZero(base) && isLessOrEqualZero(power)){
        return ZERO_TO_ZERO_OR_LESS_POWER_ERROR;
    }
    if (isLessThanZero(base) && !isPowerInteger){
        return NEGATIVE_TO_NON_INT_POWER_ERROR;
    }
    return createCode((int) pow(base, power));
}
