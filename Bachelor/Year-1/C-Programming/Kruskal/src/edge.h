#pragma once

#include <stdbool.h>

struct Edge{
    unsigned int vertex1;
    unsigned int vertex2;
    unsigned int length;
    bool isEmpty;
};

void printEdge(struct Edge edge);
bool isEdgeEmpty(struct Edge);
bool compareEdges(struct Edge edge1, struct Edge edge2);
struct Edge createEdge(unsigned int vertex1, unsigned int vertex2, unsigned int length);

static const struct Edge EMPTY_EDGE = {.vertex1 = 0, .vertex2 = 0, .length = 0, .isEmpty = true};
