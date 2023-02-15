#include <stdio.h>
#include "utils.h"
#include "containers.h"
#include "graph_input.h"

bool TopologicalSort(struct AdjacencyMatrix* adjacencyMatrix, struct VerticesQueue* verticesQueue, const Number verticesCount){
    bool impossibleToSort = false;
    for (Number index = 0; index < verticesCount; index++){
        Vertex curVertex = GetVertexWithNoAncestors(adjacencyMatrix);

        // не нашли вершину, в которую не входит ни одна дуга
        if (curVertex == 0){
            impossibleToSort = true;
            break;
        }

        Enqueue(verticesQueue, curVertex - 1);
        DeleteEdgesFromVertex(adjacencyMatrix, curVertex - 1);
    }
    return !impossibleToSort;
}
void PrintVerticesQueue(struct VerticesQueue* verticesQueue, const Number verticesCount){
    for (Number index = 0; index < verticesCount; index++){
        printf("%u ", (Number) (Dequeue(verticesQueue) + 1));
    }
}

int main() {
    Number verticesCount;
    struct AdjacencyMatrix* adjacencyMatrix = NULL;
    struct VerticesQueue* sortedVertices = NULL;

    if (!InputGraph(&adjacencyMatrix, &sortedVertices, &verticesCount)){
        DestroyAdjacencyMatrix(adjacencyMatrix);
        DestroyVerticesQueue(sortedVertices);
        return 0;
    }

    if (!TopologicalSort(adjacencyMatrix, sortedVertices, verticesCount)){
        printf("impossible to sort");
    }
    else{
        PrintVerticesQueue(sortedVertices, verticesCount);
    }

    DestroyAdjacencyMatrix(adjacencyMatrix);
    DestroyVerticesQueue(sortedVertices);
}
