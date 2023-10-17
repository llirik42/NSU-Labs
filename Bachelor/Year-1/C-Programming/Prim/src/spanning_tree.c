#include "spanning_tree.h"
#include <malloc.h>
#include <stdio.h>

struct SpanningTree{
    bool* vertices;
    struct Edge* edges;
    unsigned int edgesCount;
    unsigned int maxEdgesCount;
};

void initSpanningTree(struct SpanningTree* spanningTree, const unsigned int verticesCount){
    for (unsigned int i = 0; i < verticesCount; i++){
        spanningTree->vertices[i] = false;
    }
    spanningTree->edgesCount = 0;
    spanningTree->maxEdgesCount = verticesCount - 1;
}

struct SpanningTree* CreateSpanningTree(unsigned int verticesCount){
    struct SpanningTree* spanningTree = malloc(sizeof(struct SpanningTree));

    if (!spanningTree){
        return NULL;
    }

    spanningTree->vertices = malloc(sizeof(bool) * verticesCount);
    if (!spanningTree->vertices){
        DestroySpanningTree(spanningTree);
        return NULL;
    }

    spanningTree->edges = malloc(sizeof(struct Edge) * (verticesCount - 1));
    if (!spanningTree->edges){
        DestroySpanningTree(spanningTree);
        return NULL;
    }

    initSpanningTree(spanningTree, verticesCount);

    return spanningTree;
}
void DestroySpanningTree(struct SpanningTree* spanningTree){
    if (spanningTree){
        free(spanningTree->vertices);
        free(spanningTree->edges);
        free(spanningTree);
    }
}
void AddVertexToSpanningTree(struct SpanningTree* spanningTree, Vertex vertex){
    spanningTree->vertices[vertex] = true;
}
bool InSpanningTree(const struct SpanningTree* spanningTree, Vertex vertex){
    return spanningTree->vertices[vertex];
}
void AddEdgeToSpanningTree(struct SpanningTree* spanningTree, const struct Edge edge){
    spanningTree->edges[spanningTree->edgesCount++] = edge;
    spanningTree->vertices[edge.vertex1] = true;
    spanningTree->vertices[edge.vertex2] = true;
}
void PrintSpanningTree(const struct SpanningTree* spanningTree){
    if (spanningTree->edgesCount != spanningTree->maxEdgesCount){
        printf("no spanning tree");
        return;
    }

    for (unsigned int i = 0; i < spanningTree->edgesCount; i++){
        PrintEdge(spanningTree->edges[i]);
    }
}
