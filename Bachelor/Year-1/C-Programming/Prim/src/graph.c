#include "graph.h"
#include <malloc.h>
#include <stdio.h>
#include <stdbool.h>

struct Graph{
    unsigned int* edges;
    unsigned int verticesCount;
};

#define MAX_VERTICES_COUNT 5000
#define MAX_EDGE_LENGTH INT_MAX

#define HANDLE(OPERATION) {const Status status = OPERATION; if(status != SUCCESS){ \
                           DestroyGraph(*pGraph); return status;}}

bool inRange(const unsigned int value, const unsigned int maxValue){
    return value <= maxValue;
}

Status inputData(unsigned int* number, unsigned int maxValue, const Status status){
    const int code = scanf("%u", number);

    if (code == EOF){
        return BAD_NUMBER_OF_LINES;
    }

    if (!code || !inRange(*number, maxValue)){
        return status;
    }

    return SUCCESS;
}

unsigned int calculateEdgeIndex(const unsigned int verticesCount, const unsigned int v1, const unsigned int v2){
    unsigned int y = v1 < v2 ? v1 : v2;
    unsigned int x = v1 + v2 - y;

    unsigned int n = verticesCount;

    return (2 * n - y - 1) * y / 2 + (x - y - 1);
}

void initGraph(struct Graph* graph){
    const unsigned int stopIndex = graph->verticesCount * (graph->verticesCount - 1) / 2;

    for (unsigned int i = 0; i < stopIndex; i++){
        graph->edges[i] = INFINITE_LENGTH;
    }
}
struct Graph* createEmptyGraph(unsigned int verticesCount){
    struct Graph* graph = malloc(sizeof(struct Graph));
    if (!graph){
        return NULL;
    }

    graph->verticesCount = verticesCount;

    graph->edges = malloc(sizeof(int) * (verticesCount * (verticesCount - 1) / 2));
    if (!graph->edges){
        DestroyGraph(graph);
        return NULL;
    }

    initGraph(graph);

    return graph;
}
void addEdge(struct Graph* graph, const Vertex v1, const Vertex v2, const unsigned int length){
    if (v1 == v2){
        return;
    }

    unsigned int edgesIndex = calculateEdgeIndex(graph->verticesCount, v1, v2);
    graph->edges[edgesIndex] = length;
}

Status InputGraph(struct Graph** pGraph){
    *pGraph = NULL;
    unsigned int verticesCount;
    unsigned int edgesCount;

    HANDLE(inputData(&verticesCount, MAX_VERTICES_COUNT, BAD_NUMBER_OF_VERTICES))

    const unsigned int maxEdgesCount = verticesCount * (verticesCount - 1) / 2;
    HANDLE(inputData(&edgesCount, maxEdgesCount, BAD_NUMBER_OF_EDGES))

    *pGraph = createEmptyGraph(verticesCount);

    if (!(*pGraph)){
        return NO_MEMORY;
    }

    for (unsigned int i = 0; i < edgesCount; i++){
        Vertex vertex1;
        HANDLE(inputData(&vertex1, verticesCount, BAD_VERTEX))

        Vertex vertex2;
        HANDLE(inputData(&vertex2, verticesCount, BAD_VERTEX))

        unsigned int length;
        HANDLE(inputData(&length, MAX_EDGE_LENGTH, BAD_LENGTH))

        addEdge(*pGraph, vertex1 - 1 , vertex2 - 1, length);
    }

    return SUCCESS;
}
void DestroyGraph(struct Graph* graph){
    if (graph){
        free(graph->edges);
        free(graph);
    }
}
unsigned int GetEdgeLength(const struct Graph* graph, Vertex v1, Vertex v2){
    if (v1 == v2){
        return INFINITE_LENGTH;
    }

    unsigned int edgeIndex = calculateEdgeIndex(graph->verticesCount, v1, v2);

    return graph->edges[edgeIndex];
}
unsigned int GetGraphVerticesCount(const struct Graph* graph){
    return graph->verticesCount;
}
