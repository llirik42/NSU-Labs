#pragma once

#include <stdbool.h>

#define getQuantityTitle(quantity) ((quantity).title)
#define getQuantityMinValue(quantity, group) quantity.title == (group).basicQuantity.title ? \
                                             quantity.minValue :                          \
                                             quantity.fromBasic((group).basicQuantity.minValue)

struct Quantity{
    char* title;

    double minValue;

    double (*toBasic)(const double);
    double (*fromBasic)(const double);
};

double convert(double value, struct Quantity origin, struct Quantity new);
