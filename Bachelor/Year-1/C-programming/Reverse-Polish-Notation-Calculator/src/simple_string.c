#include "simple_string.h"
#include <stdbool.h>

#define DEFAULT_VALUE 'x'

bool checkEndOfFile(const int curChar){
    return curChar != EOF;
}
bool checkNewLine(const int curChar){
    return curChar != '\n';
}

struct String createString(){
    struct String result = {.length = 0};
    return result;
}
void inputString(struct String* string, FILE* file){
    if (!string || !file){
        return;
    }

    int curChar = fgetc(file);

    bool (*checkCurChar)(int) = file == stdin ? &checkNewLine : &checkEndOfFile;

    while (checkCurChar(curChar)){
        if (getStringLength(string) == MAX_STRING_LENGTH){
            break;
        }

        string->chars[string->length] = (char) curChar;
        string->length++;
        curChar = fgetc(file);
    }
}

bool isStringEmpty(const struct String* string){
    return !string || string->length == 0;
}

unsigned int getStringLength(const struct String* string){
    if (!string){
        return 0;
    }
    return string->length;
}

char getStringChar(const struct String* string, const unsigned int index){
    if (!string || index > getStringLength(string)){
        return DEFAULT_VALUE;
    }
    return string->chars[index];
}
