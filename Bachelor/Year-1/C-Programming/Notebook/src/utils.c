#include "utils.h"
#include <stdio.h>

#define MAX_INPUT_LENGTH "128"

bool isCharNullTerminator(const char c){
    return c == '\0';
}
bool CompareStrings(const char string1[], const char string2[], const unsigned int maxLength){
    for (unsigned int i = 0; i < maxLength; i++){
        const char curChar1 = string1[i];
        const char curChar2 = string2[i];

        bool stringsHaveDifferentLength = isCharNullTerminator(curChar1) ^ isCharNullTerminator(curChar2);
        bool reachedOneStringEnd = isCharNullTerminator(curChar1) || isCharNullTerminator(curChar2);

        if (stringsHaveDifferentLength){
            return false;
        }

        if (reachedOneStringEnd){
            return true;
        }

        if (curChar1 != curChar2){
            return false;
        }
    }

    return true;
}
void ClearStdin(){
    int curChar = getchar();
    while (curChar != EOF && curChar != '\n'){
        curChar = getchar();
    }
}
void PrintWithPrefix(const char string[]){
    printf("%s%s\n", OUT_PREFIX, string);
}
bool InputString(char string[], const char title[]){
    printf("%s%s:\n%s", OUT_PREFIX, title, IN_PREFIX);
    return scanf("%"MAX_INPUT_LENGTH"s", string) == 1;
}
bool InputInt(unsigned int* number, const char title[]){
    printf("%s%s:\n%s", OUT_PREFIX, title, IN_PREFIX);
    return scanf("%"MAX_INPUT_LENGTH"u", number) == 1;
}
bool InputDouble(double * number, const char title[]){
    printf("%s%s:\n%s", OUT_PREFIX, title, IN_PREFIX);
    return scanf("%"MAX_INPUT_LENGTH"lf", number) == 1;
}
bool InputCommand(char command[]){
    printf("%s", IN_PREFIX);
    return scanf("%s"MAX_INPUT_LENGTH"s", command) == 1;
}
