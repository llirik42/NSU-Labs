#include "graph_input.h"
#include <stdio.h>

#define TRY(function, msg) {const Code tmp = function; if(tmp == -1){printBadNumberOfLines();return false;} \
                        if(tmp == 0){printf(msg); return false;}}

void printBadNumberOfLines(){
    printf("bad number of lines");
}

bool inRange(const Number value, const Number minValue, const Number maxValue){
    return value >= minValue && value <= maxValue;
}
Code inputAndCheckData(Number* number, const Number minValue, const Number maxValue){
    const Code code = scanf("%u", number);

    if (code != 1){
        return -1;
    }

    return inRange(*number, minValue, maxValue);
}

bool InputGraph(struct AdjacencyMatrix** adjacencyMatrix, struct VerticesQueue** verticesQueue, Number* verticesCount){
    TRY(inputAndCheckData(verticesCount, 0, 2000), "bad number of vertices")

    Number edgesCount;
    const Number maxEdgesCount = *verticesCount * (*verticesCount - 1) / 2;

    TRY(inputAndCheckData(&edgesCount, 0, maxEdgesCount), "bad number of edges")

    *adjacencyMatrix = CreateAdjacencyMatrix(*verticesCount);
    *verticesQueue = CreateVerticesQueue(*verticesCount);
    if (!(*adjacencyMatrix) || !(*verticesQueue)){
        DestroyAdjacencyMatrix(*adjacencyMatrix);
        DestroyVerticesQueue(*verticesQueue);
        printf("No memory");
        return false;
    }

    for (Number index = 0; index < edgesCount; index++){
        Vertex vertex1;
        Vertex vertex2;

        TRY(inputAndCheckData(&vertex1, 1, *verticesCount), "bad vertex")
        TRY(inputAndCheckData(&vertex2, 1, *verticesCount), "bad vertex")

        AddEdge(*adjacencyMatrix, vertex1 - 1, vertex2 - 1);
    }

    return true;
}
