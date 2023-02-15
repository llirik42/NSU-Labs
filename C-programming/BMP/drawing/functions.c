#include "../string_parsing/string_parsing.h"
#include "drawing.h"
#include "functions.h"
#include <malloc.h>

#define DELTA 0.0001

struct Function{
    struct PlanePoint* points;
    unsigned int pointsCounter;
    struct Interval interval;
};

struct AdvancedValue calculateDerivative(const struct PlanePoint point1, const struct PlanePoint point2){
    if (IsUndefinedValue(point1.y) || IsUndefinedValue(point2.y)){
        return UNDEFINED_VALUE;
    }

    const double x1 = point1.x;
    const double y1 = point1.y.value;

    const double x2 = point2.x;
    const double y2 = point2.y.value;

    return CreateAdvancedValue((y2 - y1) / (x2 - x1));
}
struct AdvancedValue calculateAverageDerivative(const struct PlanePoint point1, const struct PlanePoint point2,
                                                const struct PlanePoint point3){
    struct AdvancedValue leftDerivative = calculateDerivative(point1, point2);
    struct AdvancedValue rightDerivative = calculateDerivative(point2, point3);

    if (IsUndefinedValue(leftDerivative)){
        return rightDerivative;
    }

    if (IsUndefinedValue(rightDerivative)){
        return leftDerivative;
    }

    return CreateAdvancedValue((leftDerivative.value + rightDerivative.value) / 2);
}
void initFunction(struct Function** function, const struct Interval interval){
    *function = malloc(sizeof(struct Function));
    if (!(*function)){
        return;
    }

    const unsigned int pointsCounter = (unsigned int) (interval.length / DELTA + 1);

    (*function)->points = malloc(sizeof(struct PlanePoint) * pointsCounter);
    if (!(*function)->points){
        free(function);
        return;
    }

    (*function)->interval = interval;
    (*function)->pointsCounter = pointsCounter;
}
void fillFunction(struct Function* function, struct ParsedData parsedData, const struct Interval interval){
    for (unsigned int i = 0; i < function->pointsCounter; i++){
        const double curX = interval.left + DELTA * i;

        const struct AdvancedValue curY = CalculateParsedDataValue(parsedData, curX);

        function->points[i] = CreatePlanePoint(curX, curY);
    }
}
struct Function* CreateFunction(struct Interval interval, const char* string){
    struct ParsedData parsedData = ParseString(string);

    if (IsParsedDataEmpty(parsedData)){
        return NULL;
    }

    struct Function* function = NULL;
    initFunction(&function, interval);
    if (!function){
        return NULL;
    }

    fillFunction(function, parsedData, interval);

    return function;
}
struct PlanePoint GetFunctionPoint(const struct Function* function, unsigned int index){
    const struct PlanePoint undefinedPoint = CreatePlanePoint(0, UNDEFINED_VALUE);
    return function && index < function->pointsCounter ? function->points[index] : undefinedPoint;
}
unsigned int GetFunctionPointsCounter(const struct Function* function){
    return function ? function->pointsCounter : 0;
}
void DestroyFunction(struct Function* function){
    if (!function){
        return;
    }

    free(function->points);
    free(function);
}
struct Function* GetDerivative(struct Function* function){
    if (!function || function->pointsCounter < 2){
        return NULL;
    }

    struct Function* derivative = NULL;
    initFunction(&derivative, function->interval);

    if (!derivative){
        return NULL;
    }

    const struct PlanePoint firstFunctionPoint = function->points[0];
    const struct PlanePoint secondFunctionPoint = function->points[1];
    const double firstDerivativePointX = firstFunctionPoint.x;
    const struct AdvancedValue firstDerivativePointY = calculateDerivative(firstFunctionPoint, secondFunctionPoint);
    derivative->points[0] = CreatePlanePoint(firstDerivativePointX, firstDerivativePointY);

    const unsigned int lastPointIndex = function->pointsCounter - 1;
    const struct PlanePoint lastFunctionPoint = function->points[lastPointIndex];
    const struct PlanePoint lastLastFunctionPoint = function->points[lastPointIndex - 1];
    const double lastDerivativePointX = lastFunctionPoint.x;
    const struct AdvancedValue lastDerivativePointY = calculateDerivative(lastFunctionPoint, lastLastFunctionPoint);
    derivative->points[lastPointIndex] = CreatePlanePoint(lastDerivativePointX, lastDerivativePointY);

    for (unsigned int i = 0; i < derivative->pointsCounter - 1; i++){
        const struct PlanePoint prevPoint = function->points[i - 1];
        const struct PlanePoint curPoint = function->points[i];
        const struct PlanePoint nextPoint = function->points[i + 1];

        const double curX = curPoint.x;
        const struct AdvancedValue curY = calculateAverageDerivative(prevPoint, curPoint, nextPoint);
        derivative->points[i] = CreatePlanePoint(curX, curY);
    }

    return derivative;
}
struct Function* GetAntiderivative(struct Function* function){
    if (!function){
        return NULL;
    }

    struct Function* antiderivative = NULL;
    initFunction(&antiderivative, function->interval);

    if (!antiderivative){
        return NULL;
    }

    antiderivative->points[0] = CreatePlanePoint(function->points[0].x, CreateAdvancedValue(0));

    for (unsigned int i = 1; i < function->pointsCounter; i++){
        const double curX = function->points[i].x;
        const struct AdvancedValue prevAntiderivativeY = antiderivative->points[i - 1].y;
        const struct AdvancedValue curFunctionY = function->points[i].y;

        struct AdvancedValue curY;

        if (IsUndefinedValue(curFunctionY)){
            curY = UNDEFINED_VALUE;
        }
        else if (IsUndefinedValue(prevAntiderivativeY)){
            curY = CreateAdvancedValue(0);
        }
        else{
            curY = CreateAdvancedValue(prevAntiderivativeY.value + curFunctionY.value * DELTA);
        }

        antiderivative->points[i] = CreatePlanePoint(curX, curY);
    }

    return antiderivative;
}
