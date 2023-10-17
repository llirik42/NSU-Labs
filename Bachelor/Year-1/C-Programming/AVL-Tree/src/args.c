#include "args.h"
#include <string.h>

bool checkArg(const int argc, char* argv[], char* arg){
    for (int i = 0; i < argc; i++){
        const char* curArg = argv[i];

        if (!strcmp(arg, curArg)){
            return true;
        }
    }
    return false;
}

bool CheckTKey(int arc, char* argv[]){
    return checkArg(arc, argv, "-t");
}
