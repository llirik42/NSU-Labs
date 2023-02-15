#include <stdbool.h>
#include "simple_string.h"
#include "status_code.h"
#include "string_value_calculation.h"
#include "args.h"

struct StatusCode readInputAndPrintStringValue(const bool supportDouble, const bool interactiveMode,
                                               const struct StatusCode prevCode){
    struct String stringToCalculate = createString();

    if (interactiveMode){
        printf("in > ");
    }
    inputString(&stringToCalculate, stdin);

    struct StatusCode statusCode = calculateStringValue(stringToCalculate, supportDouble, prevCode);

    if (interactiveMode){
        printf("out < ");
    }
    printCode(statusCode);
    if (interactiveMode){
        printf("\n");
    }

    return statusCode;
}

int main(int argc, char* argv[]) {
    const bool interactiveMode = checkInteractiveMode(argc, argv);
    const bool supportDouble = checkSupportDouble(argc, argv);

    struct StatusCode prevCode = SYNTAX_ERROR;

    if (!interactiveMode){
        readInputAndPrintStringValue(supportDouble, interactiveMode, prevCode);
        return 0;
    }

    while (1){
        struct StatusCode curCode = readInputAndPrintStringValue(supportDouble, interactiveMode, prevCode);
        if (isCodeSuccess(curCode)){
            prevCode = curCode;
        }
    }
}
