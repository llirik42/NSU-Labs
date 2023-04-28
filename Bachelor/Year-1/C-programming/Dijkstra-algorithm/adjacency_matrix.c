#include "adjacency_matrix.h"
#include <malloc.h>

unsigned int calculateEdgeIndex(const Number verticesCount, const Number v1, const Number v2){
    unsigned int y = v1 < v2 ? v1 : v2;
    unsigned int x = v1 + v2 - y;

    unsigned int n = verticesCount;

    return (2 * n - y - 1) * y / 2 + (x - y - 1);
}
void initAdjacencyMatrix(struct AdjacencyMatrix* adjacencyMatrix){
    for (unsigned int i = 0; i < adjacencyMatrix->edgesCount; i++){
        adjacencyMatrix->edges[i] = INFINITE_DISTANCE;
    }
}

struct AdjacencyMatrix* createAdjacencyMatrix(const Number verticesCount){
    struct AdjacencyMatrix* newMatrix = malloc(sizeof(struct AdjacencyMatrix));
    if (!newMatrix){
        return NULL;
    }

    const Number edgesCount = verticesCount * (verticesCount - 1) / 2;
    newMatrix->edgesCount = edgesCount;
    newMatrix->verticesCount = verticesCount;

    newMatrix->edges = malloc(sizeof(struct Distance) * edgesCount);
    if (!newMatrix->edges){
        destroyAdjacencyMatrix(newMatrix);
        return NULL;
    }

    initAdjacencyMatrix(newMatrix);

    return newMatrix;
}
void destroyAdjacencyMatrix(struct AdjacencyMatrix* adjacencyMatrix){
    if (!adjacencyMatrix){
        return;
    }
    free(adjacencyMatrix->edges);
    free(adjacencyMatrix);
}
void addToAdjacencyMatrix(struct AdjacencyMatrix* adjacencyMatrix, const Number v1, const Number v2, const Number length){
    unsigned int edgesIndex = calculateEdgeIndex(adjacencyMatrix->verticesCount, v1, v2);

    if (edgesIndex >= adjacencyMatrix->edgesCount){
        return;
    }

    adjacencyMatrix->edges[edgesIndex] = createDistance(length);
}
struct Distance getEdge(const struct AdjacencyMatrix* adjacencyMatrix, const Number v1, const Number v2){
    if (v1 == v2){
        return INFINITE_DISTANCE;
    }

    unsigned int edgeIndex = calculateEdgeIndex(adjacencyMatrix->verticesCount, v1, v2);

    if (edgeIndex >= adjacencyMatrix->edgesCount){
        return INFINITE_DISTANCE;
    }

    struct Distance result = adjacencyMatrix->edges[edgeIndex];
    return result;
}
