#include "dijkstra_algorithm.h"
#include <malloc.h>
#include <stdio.h>

bool allocateDijkstraData(struct DijkstraData** dijkstraData, const Number verticesCount){
    *dijkstraData = malloc(sizeof(struct DijkstraData));
    if (!(*dijkstraData)){
        return false;
    }

    (*dijkstraData)->visited = malloc(sizeof(bool) * verticesCount);
    if (!(*dijkstraData)->visited){
        return false;
    }

    (*dijkstraData)->mainPath = malloc(sizeof(Number) * verticesCount);
    if (!(*dijkstraData)->mainPath){
        return false;
    }

    (*dijkstraData)->distances = malloc(sizeof(struct Distance) * verticesCount);
    if (!(*dijkstraData)->distances){
        return false;
    }

    return true;
}
void initDijkstraData(struct DijkstraData* dijkstraData, struct AdjacencyMatrix* adjacencyMatrix,
                      const Number verticesCount, const Number sourceVertex, const Number destinationVertex){
    dijkstraData->verticesCount = verticesCount;
    dijkstraData->sourceVertex = sourceVertex;
    dijkstraData->destinationVertex = destinationVertex;
    dijkstraData->adjacencyMatrix = adjacencyMatrix;

    for (unsigned int i = 0; i < verticesCount; i++) {
        bool isCurIndexSource = i == sourceVertex;

        dijkstraData->visited[i] = false;
        dijkstraData->mainPath[i] = isCurIndexSource ? sourceVertex + 1 : 0;
        dijkstraData->distances[i] = isCurIndexSource ? ZERO_DISTANCE : INFINITE_DISTANCE;
    }
}

struct DijkstraData* createDijkstraData(struct AdjacencyMatrix* adjacencyMatrix, const Number verticesCount,
                                        const Number sourceVertex, const Number destinationVertex){
    if (!adjacencyMatrix){
        return NULL;
    }

    struct DijkstraData* newDijkstraData = NULL;
    if (!allocateDijkstraData(&newDijkstraData, verticesCount)){
        destroyDijkstraData(newDijkstraData);
        return NULL;
    }

    initDijkstraData(newDijkstraData, adjacencyMatrix, verticesCount, sourceVertex, destinationVertex);

    return newDijkstraData;
}
void destroyDijkstraData(struct DijkstraData* dijkstraData){
    if (!dijkstraData){
        return;
    }

    free(dijkstraData->visited);
    free(dijkstraData->mainPath);
    free(dijkstraData->distances);
    destroyAdjacencyMatrix(dijkstraData->adjacencyMatrix);
    free(dijkstraData);
}

bool findNearestVertex(struct DijkstraData* dijkstraData, Number* indexOfNearest){
    struct Distance shortestDistance = INFINITE_DISTANCE;
    *indexOfNearest = 0;

    for (Number i = 0; i < dijkstraData->verticesCount; i++){
        if (dijkstraData->visited[i]){
            continue;
        }

        struct Distance curDistance = dijkstraData->distances[i];

        bool isCurDistanceShorter = compareDistances(curDistance, shortestDistance);

        if (isCurDistanceShorter){
            shortestDistance = curDistance;
            *indexOfNearest = i;
        }
    }

    return !isDistanceInfinity(shortestDistance);
}
void adjustVertexDistance(struct DijkstraData* dijkstraData, const Number mainVertex, const Number curVertex){
    const struct Distance distanceToMainVertex = dijkstraData->distances[mainVertex];
    const struct Distance edgeBetweenMainAndCur = getEdge(dijkstraData->adjacencyMatrix, mainVertex, curVertex);
    const struct Distance oldDistanceToCurVertex = dijkstraData->distances[curVertex];
    const struct Distance newPathToNeighbour = sumDistances(distanceToMainVertex, edgeBetweenMainAndCur);

    bool isNewPathShorter = compareDistances(newPathToNeighbour, oldDistanceToCurVertex);

    if (isNewPathShorter){
        dijkstraData->distances[curVertex] = newPathToNeighbour;
        dijkstraData->mainPath[curVertex] = mainVertex + 2;
    }
}
void handleVertex(const Number mainVertex, struct DijkstraData* dijkstraData){
    dijkstraData->visited[mainVertex] = true;

    for (Number curVertex = 0; curVertex < dijkstraData->verticesCount; curVertex++){
        struct Distance edgeBetweenMainAndCur = getEdge(dijkstraData->adjacencyMatrix, mainVertex, curVertex);

        bool hasEdgeBetweenMainAndCur = !isDistanceInfinity(edgeBetweenMainAndCur);
        if (!hasEdgeBetweenMainAndCur){
            continue;
        }

        adjustVertexDistance(dijkstraData, mainVertex, curVertex);
    }
}

bool checkOverflow(struct DijkstraData* dijkstraData){
    Number overflowCounter = 0;

    for (Number i = 0; i < dijkstraData->verticesCount; i++){
        struct Distance distance1 = getEdge(dijkstraData->adjacencyMatrix, dijkstraData->destinationVertex, i);
        struct Distance distance2 = dijkstraData->distances[i];

        bool hasEdgeBetweenDestinationVertexAndCurrent = !isDistanceInfinity(distance1);
        bool hasPathToCurrentFromSource = !isDistanceInfinity(distance2);

        if (hasEdgeBetweenDestinationVertexAndCurrent && hasPathToCurrentFromSource){
            overflowCounter++;

            if (overflowCounter > 1){
                return false;
            }
        }
    }

    return true;
}
void printMainPath(struct DijkstraData* dijkstraData){
    Number curVertex = dijkstraData->destinationVertex;
    while (curVertex != dijkstraData->sourceVertex){
        printf("%u ", curVertex + 1);
        curVertex = dijkstraData->mainPath[curVertex] - 2;
    }
    printf("%u", dijkstraData->sourceVertex + 1);
}
void printDistances(struct DijkstraData* dijkstraData){
    for (Number i = 0; i < dijkstraData->verticesCount; i++) {
        printDistance(dijkstraData->distances[i], NULL);
        printf(" ");
    }
    printf("\n");
}
void printMainPathInfo(struct DijkstraData* dijkstraData){
    bool noPath = dijkstraData->mainPath[dijkstraData->destinationVertex] == 0;
    if (noPath){
        printf("no path");
        return;
    }

    bool isPathIntMaxPlus = isDistanceIntMaxPlus(dijkstraData->distances[dijkstraData->destinationVertex]);
    if (isPathIntMaxPlus && !checkOverflow(dijkstraData)){
        printf("overflow");
        return;
    }

    printMainPath(dijkstraData);
}
void printResult(struct DijkstraData* dijkstraData){
    printDistances(dijkstraData);

    printMainPathInfo(dijkstraData);
}

void dijkstraAlgorithm(struct DijkstraData* dijkstraData){
    struct VertexPath* verticesDistance = malloc(sizeof(struct Distance) * dijkstraData->verticesCount);
    if (!verticesDistance){
        return;
    }

    handleVertex(dijkstraData->sourceVertex, dijkstraData);

    while (1){
        Number indexOfNearest = 0;

        if (!findNearestVertex(dijkstraData, &indexOfNearest)){
            free(verticesDistance);
            return;
        }

        handleVertex(indexOfNearest, dijkstraData);
    }
}
