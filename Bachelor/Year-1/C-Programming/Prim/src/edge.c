#include "edge.h"
#include <stdio.h>

struct Edge CreateEdge(Vertex vertex1, Vertex vertex2, unsigned int length){
    struct Edge result = {.vertex1 = vertex1, .vertex2 = vertex2, .length = length};
    return result;
}
void PrintEdge(struct Edge edge){
    printf("%u %u\n", edge.vertex1 + 1, edge.vertex2 + 1);
}
