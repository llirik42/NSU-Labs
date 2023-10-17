#pragma once

#include <stdbool.h>
#include <stdio.h>
#include "buffer.h"

bool Diff(FILE* file1, FILE* file2, FILE* output, bool txtMode);
