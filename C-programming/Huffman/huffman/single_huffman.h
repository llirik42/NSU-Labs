#pragma once

#include "../utils.h"
#include <stdio.h>

Status SingleCompress(FILE* in, FILE* out);
Status SingleDecompress(FILE* in, FILE* out, unsigned int bytesLimit);
