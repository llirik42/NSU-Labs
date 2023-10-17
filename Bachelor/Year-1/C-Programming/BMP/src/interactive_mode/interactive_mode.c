#include "interactive_mode.h"
#include <stdio.h>

#define INTEGRAL_PREFIX "Integral of "
#define INTEGRAL_PREFIX_LENGTH 12

bool checkAndHandleDerivative(const char* functionNotation, const unsigned int length){
    return functionNotation[length - 1] == '\'';
}
bool checkAndHandleAntiderivative(const char* functionNotation, const unsigned int length){
    if (length < INTEGRAL_PREFIX_LENGTH){
        return false;
    }

    return CompareStringsSoftly(INTEGRAL_PREFIX, functionNotation, INTEGRAL_PREFIX_LENGTH) &&
           functionNotation[length - 2] == 'd' && functionNotation[length - 1] == 'x';
}
bool readInput(char buffer[]){
    unsigned int index = 0;
    int curChar = getchar();

    while (curChar != '\n'){
        buffer[index++] = (char) (curChar);
        curChar = getchar();
    }

    buffer[index] = '\0';

    return index != 0;
}

bool InputIntervals(struct Interval* curXInterval, struct Interval* curYInterval){
    char buffer[MAX_INPUT_SIZE];

    printf("Write intervals (left x, right x, bottom y, top y)\n");

    if(!readInput(buffer)){
        return false;
    }

    double xLeft;
    double xRight;
    double yBottom;
    double yTop;

    bool successReading = sscanf(buffer, "%lf%lf%lf%lf", &xLeft, &xRight, &yBottom, &yTop) == 4 &&
                          (xLeft < xRight && yBottom < yTop);

    printf("\n");

    if (!successReading){
        return false;
    }

    *curXInterval = CreateInterval(xLeft, xRight);
    *curYInterval = CreateInterval(yBottom, yTop);

    return true;
}
bool InputFunction(char* functionNotation, bool* derivative, bool* antiderivative){
    char buffer[MAX_INPUT_SIZE];

    printf("Write function and what to do with it\n");

    readInput(buffer);

    const unsigned int inputLength = GetStringLength(buffer);

    const bool isDerivative = checkAndHandleDerivative(buffer, inputLength);
    const bool isAntiderivative = checkAndHandleAntiderivative(buffer, inputLength);

    unsigned int shift = 0;

    if (isDerivative){
        buffer[inputLength - 1] = '\0';
    }
    if (isAntiderivative){
        buffer[inputLength - 2] = '\0';
        shift = INTEGRAL_PREFIX_LENGTH;
    }

    for (unsigned int i = 0; i < MAX_INPUT_SIZE - shift; i++){
        functionNotation[i] = buffer[i + shift];
    }

    *derivative = isDerivative;
    *antiderivative = isAntiderivative;

    return (isDerivative && !isAntiderivative) || (!isDerivative && isAntiderivative) || (!isDerivative && !isAntiderivative);
}
