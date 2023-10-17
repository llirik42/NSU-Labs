#include <stdio.h>
#include <limits.h>
#include "graph_input.h"

#define try(expression) {bool tmp = (expression); if (!tmp) {return tmp;}}

bool inRange(const unsigned int value, const unsigned int minValue, const unsigned int maxValue){
    return value >= minValue && value <= maxValue;
}

bool inputAndCheckData(unsigned int* data, const unsigned int minValue, const unsigned int maxValue,
                       char* error){
    int codeOfInput = scanf("%u", data);

    if (codeOfInput == EOF){
        printf("Weird\n");
        return false;
    }
    if (!codeOfInput || !inRange(*data, minValue, maxValue)){
        printf("%s\n", error);
        return false;
    }

    return true;
}

bool inputEdges(struct EdgesArray* edgesArray, const unsigned int verticesCount, const unsigned int edgesCount){
    for (unsigned int i = 0; i < edgesCount; i++){
        unsigned int vertex1, vertex2, edgeLength;

        int codeOfInput = scanf("%u %u %u", &vertex1, &vertex2, &edgeLength);

        if (codeOfInput != 3){
            printf("bad number of lines");
            return false;
        }

        if (!inRange(vertex1, 1, verticesCount) || !inRange(vertex2, 1, verticesCount)){
            printf("bad vertex");
            return false;
        }
        if (edgeLength > INT_MAX){
            printf("bad length");
            return false;
        }

        const struct Edge newEdge = createEdge(vertex1 - 1, vertex2 - 1, edgeLength);
        setArrayElementValue(edgesArray, i, newEdge);
    }
    return true;
}
bool inputGraphData(struct EdgesArray** pEdgesArray, unsigned int* verticesCount, unsigned int* edgesCount){
    try(inputAndCheckData(verticesCount, 0, 5000, "bad number of vertices"))

    const unsigned int maxEdgesCount = *verticesCount * (*verticesCount + 1) / 2;
    try(inputAndCheckData(edgesCount, 0, maxEdgesCount, "bad number of edges"))

    (*pEdgesArray) = createEdgesArray(*edgesCount);

    if (!inputEdges(*pEdgesArray, *verticesCount, *edgesCount)){
        destroyEdgesArray(*pEdgesArray);
        return false;
    }

    return true;
}
