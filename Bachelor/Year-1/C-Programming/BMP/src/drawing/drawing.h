#pragma once

#include "../utils/utils.h"
#include "../bmp/bmp.h"

struct Plane;
struct Function;

struct Plane* CreatePlane(struct Interval xInterval, struct Interval yInterval, unsigned int width, unsigned int height);
void DestroyPlane(struct Plane* plane);
struct Image* GetPlaneImage(const struct Plane* plane);
void DrawFunction(struct Plane* plane, struct Function* function, struct Color color);

struct Function* CreateFunction(struct Interval interval, const char* string);
void DestroyFunction(struct Function* function);
struct Function* GetDerivative(struct Function* function);
struct Function* GetAntiderivative(struct Function* function);
