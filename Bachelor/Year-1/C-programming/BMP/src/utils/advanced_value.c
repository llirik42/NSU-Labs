#include <stdbool.h>
#include "utils.h"

bool IsUndefinedValue(const struct AdvancedValue value){
    return value.isUndefined;
}
struct AdvancedValue CreateAdvancedValue(double value){
    struct AdvancedValue graphValue = {.value = value, .isUndefined = false};
    return graphValue;
}
