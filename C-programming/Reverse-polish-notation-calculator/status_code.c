#include "status_code.h"
#include <stdio.h>

struct StatusCode createCode(const double resultValue){
    const struct StatusCode result = {.type = SUCCESS, .resultValue = resultValue};
    return result;
}
void printResponseValue(const struct StatusCode code){
    const double doubleValue = code.resultValue;
    const int integerValue = (int) doubleValue;

    if (doubleValue == integerValue){
        printf("%d\n", integerValue);
        return;
    }
    printf("%f\n", doubleValue);
}
void printErrorMessage(const struct StatusCode code){
    printf("%s\n", code.errorMessage);
}
void printCode(const struct StatusCode code){
    if (isCodeSuccess(code)){
        printResponseValue(code);
        return;
    }
    printErrorMessage(code);
}
