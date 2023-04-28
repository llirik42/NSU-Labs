#include "drawing.h"
#include <malloc.h>
#include "functions.h"

struct Plane{
    struct Interval xInterval;
    struct Interval yInterval;
    struct Image* image;
};

unsigned int scale(const double value, const struct Interval interval, const unsigned int toHigh){
    if (interval.length == 0){
        return 0;
    }

    return (unsigned int) (toHigh * (value - interval.left) / (interval.length));
}
void drawPoint(struct Plane* plane, const struct PlanePoint point, const struct Color color){
    if (IsUndefinedValue(point.y)){
        return;
    }

    const unsigned int imageX = scale(point.x, plane->xInterval, GetImageWidth(plane->image) - 1);
    const unsigned int imageY = scale(point.y.value, plane->yInterval, GetImageHeight(plane->image) - 1);

    struct Image* image = plane->image;

    SetPixelColor(image, imageY, imageX, color);

    SetPixelColor(image, imageY, imageX - 1, color);
    SetPixelColor(image, imageY, imageX + 1, color);

    SetPixelColor(image, imageY - 1, imageX, color);
    SetPixelColor(image, imageY + 1, imageX, color);

    SetPixelColor(image, imageY - 1, imageX - 1, color);
    SetPixelColor(image, imageY - 1, imageX + 1, color);
    SetPixelColor(image, imageY + 1, imageX - 1, color);
    SetPixelColor(image, imageY + 1, imageX + 1, color);
}
void drawAxes(struct Plane* plane){
    struct Image* image = plane->image;

    const unsigned int height = GetImageHeight(image);
    const unsigned int width = GetImageWidth(image);

    if (Contains(plane->xInterval, 0)){
        const unsigned int positionOfOY = scale(0, plane->xInterval, width);

        for (unsigned int i = 0; i < width; i++){
            SetPixelColor(image, i, positionOfOY, BLACK_COLOR);
        }
    }

    if (Contains(plane->yInterval, 0)){
        const unsigned int positionOfOX = scale(0, plane->yInterval, height);

        for (unsigned int i = 0; i < height; i++){
            SetPixelColor(image, positionOfOX, i, BLACK_COLOR);
        }
    }
}

struct Plane* CreatePlane(const struct Interval xInterval, const struct Interval yInterval, unsigned int width,
                          unsigned int height){
    struct Plane* plane = malloc(sizeof(struct Plane));
    if (!plane){
        return NULL;
    }

    plane->image = CreateImage(height, width, WHITE_COLOR);
    if (!plane->image){
        DestroyPlane(plane);
        return NULL;
    }

    plane->xInterval = xInterval;
    plane->yInterval = yInterval;

    drawAxes(plane);

    return plane;
}
void DestroyPlane(struct Plane* plane){
    if (!plane){
        return;
    }

    DestroyImage(plane->image);
    free(plane);
}
struct Image* GetPlaneImage(const struct Plane* plane){
    return plane ? plane->image : NULL;
}
void DrawFunction(struct Plane* plane, struct Function* function, struct Color color){
    if (!plane || !function){
        return;
    }

    const unsigned int functionPointsCounter = GetFunctionPointsCounter(function);

    for (unsigned int i = 0; i < functionPointsCounter; i++){
        drawPoint(plane, GetFunctionPoint(function, i), color);
    }
}
