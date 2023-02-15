#pragma once

#include <stdbool.h>

struct Color{
    unsigned char blue;
    unsigned char green;
    unsigned char red;
};
struct AdvancedValue{
    double value;
    bool isUndefined;
};
struct Interval{
    double left;
    double right;
    double length;
};
struct PlanePoint{
    double x;
    struct AdvancedValue y;
};

bool IsUndefinedValue(struct AdvancedValue value);
struct AdvancedValue CreateAdvancedValue(double value);

struct Interval CreateInterval(double left, double right);
bool Contains(struct Interval interval, double number);

struct PlanePoint CreatePlanePoint(double x, struct AdvancedValue y);

unsigned int CharToDigit(char c);
bool IsCharDigit(char c);
unsigned int GetStringLength(const char* string);
bool CompareStringsSoftly(const char* string1, const char* string2, unsigned int length);

static const struct Color WHITE_COLOR = {.red = 255, .green = 255, .blue = 255};
static const struct Color BLACK_COLOR = {.red = 0, .green = 0, .blue = 0};

static const struct AdvancedValue UNDEFINED_VALUE = {.isUndefined = true};
