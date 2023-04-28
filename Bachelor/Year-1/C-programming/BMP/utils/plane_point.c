#include "utils.h"

struct PlanePoint CreatePlanePoint(double x, struct AdvancedValue y){
    struct PlanePoint result = {.x = x, .y = y};
    return result;
}
