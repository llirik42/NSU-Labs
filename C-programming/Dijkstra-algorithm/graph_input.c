#include <stdio.h>
#include <limits.h>
#include "graph_input.h"

#define try(expression) {bool tmp = (expression); if (!tmp) {return tmp;}}

bool inputAndCheckData(Number * data, const Number minValue, const Number maxValue, char* error){
    int codeOfInput = scanf("%u", data);

    if (codeOfInput == EOF){
        printf("?\n");
        return false;
    }
    if (codeOfInput == 0 || *data > maxValue || *data < minValue){
        printf("%s\n", error);
        return false;
    }

    return true;
}


bool inputEdges(struct AdjacencyMatrix* adjacencyMatrix, const Number verticesCount, const Number edgesCount){
    for (unsigned int i = 0; i < edgesCount; i++){
        Number startOfEdge, endOfEdge, edgeLength;

        int codeOfInput = scanf("%u %u %u", &startOfEdge, &endOfEdge, &edgeLength);

        if (codeOfInput != 3){
            printf("bad number of lines");
            return false;
        }

        if (startOfEdge < 1 || startOfEdge > verticesCount || edgeLength < 1 || endOfEdge > verticesCount){
            printf("bad vertex");
            return false;
        }

        if (edgeLength > INT_MAX){
            printf("bad length");
            return false;
        }

        addToAdjacencyMatrix(adjacencyMatrix, startOfEdge - 1, endOfEdge - 1, edgeLength);
    }
    return true;
}
bool inputGraphData(struct AdjacencyMatrix** adjacencyMatrix, Number* verticesCount, Number* sourceVertex,
                    Number* destinationVertex){
    Number edgesCount = 0;

    try(inputAndCheckData(verticesCount, 0, 5000, "bad number of vertices"))

    try(inputAndCheckData(sourceVertex, 1, *verticesCount, "bad vertex"))

    try(inputAndCheckData(destinationVertex, 1, *verticesCount, "bad vertex"))

    const unsigned int maxEdgesCount = *verticesCount * (*verticesCount + 1) / 2;
    try(inputAndCheckData(&edgesCount, 0, maxEdgesCount, "bad number of edges"))

    (*adjacencyMatrix) = createAdjacencyMatrix(*verticesCount);

    if (!inputEdges(*adjacencyMatrix, *verticesCount, edgesCount)){
        destroyAdjacencyMatrix(*adjacencyMatrix);
        return false;
    }
    (*sourceVertex)--;
    (*destinationVertex)--;

    return true;
}
