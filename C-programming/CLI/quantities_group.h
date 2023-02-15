#pragma once

#include "quantity.h"

#define getGroupTitle(group) ((group)->title)
#define getGroupQuantitiesCounter(group) ((group)->quantitiesCounter)
#define getGroupQuantity(group, i) (i == 0 ? group->basicQuantity : group->nonBasicQuantities[i - 1])

struct QuantityGroup{
    const char* title;
    unsigned int quantitiesCounter;
    struct Quantity basicQuantity;
    struct Quantity nonBasicQuantities[];
};
