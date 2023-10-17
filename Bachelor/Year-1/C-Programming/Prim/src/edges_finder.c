#include "edges_finder.h"
#include <malloc.h>

struct EdgesFinder{
    struct Edge* shortestEdges;
    struct Graph* graph;
    struct SpanningTree* spanningTree;
    unsigned int verticesCount;
};

void initEdgesFinder(struct EdgesFinder* edgesFinder){
    for (Vertex curVertex = 0; curVertex < edgesFinder->verticesCount; curVertex++){
        edgesFinder->shortestEdges[curVertex] = LONGEST_EDGE;
    }
}

struct EdgesFinder* CreateEdgesFinder(struct Graph* graph, struct SpanningTree* spanningTree){
    struct EdgesFinder* edgesFinder = malloc(sizeof(struct EdgesFinder));
    if (!edgesFinder){
        return NULL;
    }

    edgesFinder->verticesCount = GetGraphVerticesCount(graph);

    edgesFinder->shortestEdges = malloc(sizeof(struct Edge) * edgesFinder->verticesCount);
    if (!edgesFinder->shortestEdges){
        DestroyEdgesFinder(edgesFinder);
        return NULL;
    }

    edgesFinder->graph = graph;
    edgesFinder->spanningTree = spanningTree;

    initEdgesFinder(edgesFinder);

    return edgesFinder;
}
void DestroyEdgesFinder(struct EdgesFinder* edgesFinder){
    if (edgesFinder){
        free(edgesFinder->shortestEdges);
        free(edgesFinder);
    }
}
bool FindShortestEdge(const struct EdgesFinder* edgesFinder, struct Edge* shortestEdge, Vertex* vertex){
    *shortestEdge = edgesFinder->shortestEdges[0];
    *vertex = 0;

    for (Vertex curVertex = 1; curVertex < edgesFinder->verticesCount; curVertex++){
        if (InSpanningTree(edgesFinder->spanningTree, curVertex)){
            continue;
        }

        const struct Edge curEdge = edgesFinder->shortestEdges[curVertex];
        if (curEdge.length < shortestEdge->length){
            *shortestEdge = curEdge;
            *vertex = curVertex;
        }
    }

    return shortestEdge->length != INFINITE_LENGTH;
}
void AdjustEdges(struct EdgesFinder* edgesFinder, const Vertex newVertex){
    for (Vertex curVertex = 0; curVertex < edgesFinder->verticesCount; curVertex++){
        if (InSpanningTree(edgesFinder->spanningTree, curVertex)){
            continue;
        }

        const unsigned int newEdgeLength = GetEdgeLength(edgesFinder->graph, newVertex, curVertex);

        struct Edge* oldEdge = &edgesFinder->shortestEdges[curVertex];

        if (newEdgeLength < oldEdge->length){
            *oldEdge = CreateEdge(newVertex, curVertex, newEdgeLength);
        }
    }
}
