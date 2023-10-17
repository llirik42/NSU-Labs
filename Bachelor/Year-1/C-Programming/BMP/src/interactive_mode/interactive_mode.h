#pragma once

#include <stdbool.h>
#include "../utils/utils.h"
#include "../drawing/functions.h"

#define MAX_INPUT_SIZE 64

bool InputIntervals(struct Interval* curXInterval, struct Interval* curYInterval);
bool InputFunction(char* functionNotation, bool* derivative, bool* antiderivative);
