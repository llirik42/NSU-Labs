#include <stdbool.h>
#include "utils.h"

struct Interval CreateInterval(const double left, const double right){
    const double min = right > left ? left : right;
    const double max = right > left ? right : left;

    struct Interval newInterval = {.left = min, .right = max, .length = max - min};
    return newInterval;
}
bool Contains(const struct Interval interval, const double number){
    return interval.left <= number && number <= interval.right;
}
