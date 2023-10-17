#include <stdio.h>
#include "diff.h"
#include <locale.h>

void printWrongInput(){
    printf("wrong input");
}

int main(int arc, char* argv[]){
    setlocale(LC_ALL, "Rus");

    if (arc > 4 || arc < 3){
        printWrongInput();
        return 1;
    }

    FILE* file1 = fopen(argv[1], "rb");
    FILE* file2 = fopen(argv[2], "rb");

    if (!file1 || !file2){
        if (file1){
            fclose(file1);
        }
        if (file2){
            fclose(file2);
        }

        printWrongInput();
        return 1;
    }
    const bool binaryModeArg = arc == 4 ? CompareStrings(argv[3], "-b") : false;
    const bool txtMode = !binaryModeArg && IsFileTxt(file1) && IsFileTxt(file2);
    
    return Diff(file1, file2, stdout, txtMode);
}
