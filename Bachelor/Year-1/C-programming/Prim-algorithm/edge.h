#pragma once

#include "utils.h"
#include <limits.h>

struct Edge{
    Vertex vertex1;
    Vertex vertex2;
    unsigned int length;
};

struct Edge CreateEdge(Vertex vertex1, Vertex vertex2, unsigned int length);
void PrintEdge(struct Edge edge);

static const struct Edge LONGEST_EDGE = {.vertex1 = 0, .vertex2 = 0, .length = INFINITE_LENGTH};
