#include "quantity.h"

double convert(const double value, const struct Quantity origin, const struct Quantity new){
    bool isOriginEqualsNew = origin.title == new.title;

    if (isOriginEqualsNew){
        return value;
    }

    const bool isOriginQuantityBasic = !(origin.toBasic);
    const bool isNewQuantityBasic = !(new.fromBasic);

    if (isOriginQuantityBasic){
        return new.fromBasic(value);
    }
    if (isNewQuantityBasic){
        return origin.toBasic(value);
    }

    const double valueInBasicQuantity = origin.toBasic(value);
    return new.fromBasic(valueInBasicQuantity);
}
