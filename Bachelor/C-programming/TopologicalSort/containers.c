#include "containers.h"

#include <malloc.h>
#include <math.h>

struct AdjacencyMatrix{
    Byte** adjacencyMatrix;
    Number height;
    Number width;
    Number verticesCount;
    Number* ancestors;
};
struct VerticesQueue{
    Vertex* vertices;
    Number index1;
    Number index2;
};

struct AdjacencyMatrix* mallocAdjacencyMatrix(const Number verticesCount){
    struct AdjacencyMatrix* adjacencyMatrix = malloc(sizeof(struct AdjacencyMatrix));
    if (!adjacencyMatrix){
        return NULL;
    }

    adjacencyMatrix->height = verticesCount;
    adjacencyMatrix->width = (Number) ceil( (double) verticesCount / 8);
    adjacencyMatrix->verticesCount = verticesCount;

    adjacencyMatrix->adjacencyMatrix = malloc(sizeof(Byte*) * adjacencyMatrix->height);
    if (!adjacencyMatrix->adjacencyMatrix){
        DestroyAdjacencyMatrix(adjacencyMatrix);
        return NULL;
    }

    for (Number i = 0; i < adjacencyMatrix->height; i++){
        adjacencyMatrix->adjacencyMatrix[i] = malloc(sizeof(Byte) * adjacencyMatrix->width);

        if (!adjacencyMatrix->adjacencyMatrix[i]){
            DestroyAdjacencyMatrix(adjacencyMatrix);
            return NULL;
        }
    }

    adjacencyMatrix->ancestors = malloc(sizeof(Number) * adjacencyMatrix->verticesCount);
    if (!adjacencyMatrix->ancestors){
        DestroyAdjacencyMatrix(adjacencyMatrix);
        return NULL;
    }

    return adjacencyMatrix;
}
void initAdjacencyMatrix(struct AdjacencyMatrix* adjacencyMatrix){
    for (Number i = 0; i < adjacencyMatrix->height; i++){
        for (Number j = 0; j < adjacencyMatrix->width; j++){
            adjacencyMatrix->adjacencyMatrix[i][j] = 0;
        }
    }

    for (Number i = 0; i < adjacencyMatrix->verticesCount; i++){
        adjacencyMatrix->ancestors[i] = 0;
    }
}

struct AdjacencyMatrix* CreateAdjacencyMatrix(const Number verticesCount){
    struct AdjacencyMatrix* adjacencyMatrix = mallocAdjacencyMatrix(verticesCount);

    if (!adjacencyMatrix){
        return NULL;
    }

    initAdjacencyMatrix(adjacencyMatrix);

    return adjacencyMatrix;
}
void DestroyAdjacencyMatrix(struct AdjacencyMatrix* adjacencyMatrix){
    if (!adjacencyMatrix){
        return;
    }

    if (adjacencyMatrix->adjacencyMatrix){
        for (Number index = 0; index < adjacencyMatrix->height; index++){
            free(adjacencyMatrix->adjacencyMatrix[index]);
        }
    }
    free(adjacencyMatrix->adjacencyMatrix);
    free(adjacencyMatrix->ancestors);
    free(adjacencyMatrix);
}
Vertex GetVertexWithNoAncestors(const struct AdjacencyMatrix* adjacencyMatrix){
    for (Vertex i = 0; i < adjacencyMatrix->verticesCount; i++){
        if (!(adjacencyMatrix->ancestors[i])){
            return i + 1;
        }
    }
    return 0;
}
void AddEdge(struct AdjacencyMatrix* adjacencyMatrix, const Vertex ancestor, const Vertex descendant){
    SET_BIT(adjacencyMatrix->adjacencyMatrix[ancestor][descendant / 8], 7 - descendant % 8, 1)
    adjacencyMatrix->ancestors[descendant]++;
}
void DeleteEdgesFromVertex(struct AdjacencyMatrix* adjacencyMatrix, const Vertex vertex){
    for (Number i = 0; i < adjacencyMatrix->width; i++){
        const Number stopIndex = MIN(8, adjacencyMatrix->verticesCount - 8 * i);

        for (Number j = 0; j < stopIndex; j++){
            if (GET_BIT(adjacencyMatrix->adjacencyMatrix[vertex][i], 7 - j)){
                SET_BIT(adjacencyMatrix->adjacencyMatrix[vertex][i], 7 - j, 0)
                adjacencyMatrix->ancestors[i * 8 + j]--;
            }
        }
    }
    adjacencyMatrix->ancestors[vertex] = 1;
}

struct VerticesQueue* CreateVerticesQueue(const Number verticesCount){
    struct VerticesQueue* verticesQueue = malloc(sizeof(struct VerticesQueue));
    if (!verticesQueue){
        return NULL;
    }

    verticesQueue->vertices = malloc(sizeof(Number) * verticesCount);
    if (!verticesQueue->vertices){
        free(verticesQueue);
        return NULL;
    }

    verticesQueue->index1 = 0;
    verticesQueue->index2 = 0;

    return verticesQueue;
}
void DestroyVerticesQueue(struct VerticesQueue* verticesQueue){
    if (!verticesQueue){
        return;
    }

    free(verticesQueue->vertices);
    free(verticesQueue);
}
void Enqueue(struct VerticesQueue* verticesQueue, const Vertex vertex){
    verticesQueue->vertices[verticesQueue->index2++] = vertex;
}
Vertex Dequeue(struct VerticesQueue* verticesQueue){
    return verticesQueue->vertices[verticesQueue->index1++];
}
