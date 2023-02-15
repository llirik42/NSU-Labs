#include "testing.h"
#include "diff.h"

#define TESTS_COUNT 15
#define OUT_PATH "../testing/out.txt"
#define CLEAR_OUT(out) out = fopen(OUT_PATH, "w+");

bool test(Number index, FILE* out){
    char pathIn1[MAX_PATH_LENGTH];
    char pathIn2[MAX_PATH_LENGTH];
    char pathOutTxt[MAX_PATH_LENGTH];
    char pathOutBinary[MAX_PATH_LENGTH];

    sprintf(pathIn1, "../testing/test_%u/file1.txt", index);
    sprintf(pathIn2, "../testing/test_%u/file2.txt", index);
    sprintf(pathOutTxt, "../testing/test_%u/out_1.txt", index);
    sprintf(pathOutBinary, "../testing/test_%u/out_2.txt", index);

    FILE* in1 = fopen(pathIn1, "rb");
    FILE* in2 = fopen(pathIn2, "rb");
    FILE* outTxt = fopen(pathOutBinary, "r");
    FILE* outBinary = fopen(pathOutTxt, "r");

    CLEAR_OUT(out)
    Diff(in1, in2, out, true);
    if (!CompareFiles(out, outBinary)){
        return false;
    }

    CLEAR_OUT(out)
    Diff(in1, in2, out, false);
    if (!CompareFiles(out, outTxt)){
        return false;
    }

    fclose(in1);
    fclose(in2);
    fclose(outBinary);
    fclose(outTxt);
    return true;
}

void StartTesting(){
    FILE* out = fopen(OUT_PATH, "w+");
    for (Number i = 1; i <= TESTS_COUNT; i++){
        if (!test(i, out)){
            printf("Error while testing %u test", i);
            return;
        }
    }
    printf("All tests are successfully passed");
    fclose(out);
}
