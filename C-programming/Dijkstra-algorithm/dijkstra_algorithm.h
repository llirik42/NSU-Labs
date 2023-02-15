#pragma once

#include <stdbool.h>
#include "distance.h"
#include "adjacency_matrix.h"

struct DijkstraData{
    Number verticesCount;
    Number sourceVertex;
    Number destinationVertex;
    bool* visited;
    Number * mainPath;
    struct Distance* distances;
    struct AdjacencyMatrix* adjacencyMatrix;
};

struct DijkstraData* createDijkstraData(struct AdjacencyMatrix* adjacencyMatrix, Number verticesCount,
                                        Number sourceVertex, Number destinationVertex);
void destroyDijkstraData(struct DijkstraData* dijkstraData);

void printResult(struct DijkstraData* dijkstraData);
void dijkstraAlgorithm(struct DijkstraData* dijkstraData);
