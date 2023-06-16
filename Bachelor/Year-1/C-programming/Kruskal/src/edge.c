#include "edge.h"
#include <stdio.h>

void printEdge(const struct Edge edge){
    printf("%u %u\n", edge.vertex1 + 1, edge.vertex2 + 1);
}
bool isEdgeEmpty(const struct Edge edge){
    return edge.isEmpty;
}
bool compareEdges(const struct Edge edge1, const struct Edge edge2){
    return edge1.length < edge2.length;
}
struct Edge createEdge(const unsigned int vertex1, const unsigned int vertex2, const unsigned int length){
    const struct Edge newEdge = {
            .vertex1 = vertex1, .vertex2 = vertex2, .length = length, .isEmpty = false
    };
    return newEdge;
}
