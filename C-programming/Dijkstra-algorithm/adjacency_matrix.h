#pragma once

#include "distance.h"

struct AdjacencyMatrix{
    struct Distance* edges;
    Number verticesCount;
    Number edgesCount;
};

struct AdjacencyMatrix* createAdjacencyMatrix(Number verticesCount);
void destroyAdjacencyMatrix(struct AdjacencyMatrix* adjacencyMatrix);
void addToAdjacencyMatrix(struct AdjacencyMatrix* adjacencyMatrix, Number v1, Number v2, Number length);
struct Distance getEdge(const struct AdjacencyMatrix* adjacencyMatrix, Number v1, Number v2);
