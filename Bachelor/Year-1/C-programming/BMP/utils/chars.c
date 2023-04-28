#include <stdbool.h>
#include "utils.h"

char toLowerCase(const char c){
    if ('A' <= c && c <= 'Z'){
        return (char) (c - (char) (('A' - 'a')));
    }
    return c;
}

unsigned int CharToDigit(char c){
    return ((unsigned char) ((c) - '0'));
}
bool IsCharDigit(char c){
    return '0' <= c && c <= '9';
}
unsigned int GetStringLength(const char* string){
    unsigned int result = 0;
    while (string[result] != '\0') {
        result++;
    }
    return result;
}
bool CompareStringsSoftly(const char* string1, const char* string2, const unsigned int length){
    for (unsigned int i = 0; i < length; i++){
        const char curChar1 = string1[i];
        const char curChar2 = string2[i];

        if (toLowerCase(curChar1) != toLowerCase(curChar2)){
            return false;
        }
    }
    return true;
}
