#include "utils/utils.h"
#include "bmp/bmp.h"
#include "drawing/drawing.h"
#include "interactive_mode/interactive_mode.h"

#define HEIGHT 1024
#define WIDTH 1024

#define SUCCESS_GENERATING 0
#define NO_MEMORY (-1)
#define WRONG_INPUT (1)

void printWrongInput(){
    printf("Wrong input\n\n");
}
void printNoMemory(){
    printf("No memory\n\n");
}
void printSuccessDrawing(){
    printf("Function was successfully drawn\n\n");
}
char generateBMPWithFunction(const char* functionNotation, const struct Interval xInterval, const struct Interval yInterval,
                             const bool derivative, const bool antiderivative, const char* path){
    struct Plane *plane = CreatePlane(xInterval, yInterval, WIDTH, HEIGHT);
    if (!plane){
        return NO_MEMORY;
    }

    struct Function *mainFunction = CreateFunction(xInterval, functionNotation);
    if (!mainFunction){
        DestroyPlane(plane);
        return WRONG_INPUT;
    }

    if (derivative){
        struct Function* derivativeFunction = GetDerivative(mainFunction);
        if (!derivativeFunction){
            DestroyPlane(plane);
            return NO_MEMORY;
        }
        DrawFunction(plane, derivativeFunction, BLACK_COLOR);
        DestroyFunction(derivativeFunction);
    }
    else if (antiderivative){
        struct Function* antiderivativeFunction = GetAntiderivative(mainFunction);
        if (!antiderivativeFunction){
            DestroyPlane(plane);
            return -1;
        }
        DrawFunction(plane, antiderivativeFunction, BLACK_COLOR);
        DestroyFunction(antiderivativeFunction);
    }
    else{
        DrawFunction(plane, mainFunction, BLACK_COLOR);
    }

    DestroyFunction(mainFunction);
    GenerateBMP(GetPlaneImage(plane), path);
    DestroyPlane(plane);

    return SUCCESS_GENERATING;
}

int main() {
    const char* PATH = "../bmp.bmp";

    while (1) {
        struct Interval curXInterval;
        struct Interval curYInterval;

        if (!InputIntervals(&curXInterval, &curYInterval)){
            printWrongInput();
            continue;
        }

        char functionNotation[MAX_INPUT_SIZE];
        bool derivative;
        bool antiderivative;

        if (!InputFunction(functionNotation, &derivative, &antiderivative)){
            printWrongInput();
            continue;
        }

        char code = generateBMPWithFunction(functionNotation, curXInterval, curYInterval, derivative, antiderivative, PATH);

        if (code == NO_MEMORY){
            printNoMemory();
            break;
        }
        else if (code == WRONG_INPUT){
            printWrongInput();
        }
        else{
            printSuccessDrawing();
        }
    }
}
