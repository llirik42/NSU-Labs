#include "utils.h"

static const char* STATUS_TO_MESSAGE[7] = {"Success", "Input error", "No memory", "Incorrect compression",
                                           "Incorrect paths", "Execution error", "Incorrect mode"};

void PrintStatus(const Status status){
    if (status != SUCCESS){
        printf("%s", STATUS_TO_MESSAGE[status]);
    }
}
long GetFileSize(FILE* file){
    const long position = ftell(file);
    fseek(file, 0, SEEK_END);
    const long result = ftell(file);
    fseek(file, position, SEEK_SET);
    return result;
}
bool IsFileEmpty(FILE* file, const bool gitLabTesting){
    if (!file){
        return true;
    }

    return GetFileSize(file) <= gitLabTesting;
}
unsigned int GetStringLength(const char* string){
    unsigned int result = 0;
    while (string[result] != '\0'){
        result++;
    }
    return result;
}
bool CompareStrings(const char* string1, const char* string2){
    const unsigned int length1 = GetStringLength(string1);
    for (unsigned char i = 0; i < length1; i++){
        if (string1[i] != string2[i]){
            return false;
        }
    }
    return true;
}
char* SeparateNameFromFullPath(char* path){
    const unsigned int length = GetStringLength(path);

    unsigned int index = 0;
    for (unsigned int i = length; i > 0; i--){
        if (path[i - 1] == '/'){
            index = i;
            break;
        }
    }
    return path + index;
}
