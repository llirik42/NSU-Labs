#include "edges_array.h"
#include <malloc.h>

void swap(struct Edge* a, struct Edge* b){
    struct Edge tmp = *a;

    *a = *b;
    *b = tmp;
}
void siftDown(struct Edge* array, unsigned int length, unsigned int curIndex){
    unsigned int leftIndex = 2 * curIndex + 1;
    unsigned int rightIndex = 2 * curIndex + 2;

    unsigned int indexOfLargest = curIndex;
    if (rightIndex < length && compareEdges(array[indexOfLargest], array[rightIndex])){
        indexOfLargest = rightIndex;
    }

    if (leftIndex < length && compareEdges(array[indexOfLargest], array[leftIndex])){
        indexOfLargest = leftIndex;
    }

    if (indexOfLargest != curIndex){
        swap(&array[indexOfLargest], &array[curIndex]);
        siftDown(array, length, indexOfLargest);
    }
}
void heapSort(struct Edge* array, unsigned int length){
    for (unsigned int index = length / 2; index > 0; index--){
        siftDown(array, length, index - 1);
    }

    for (unsigned int index = length; index > 0; index--){
        swap(&array[0], &array[index - 1]);

        siftDown(array, index - 1, 0);
    }
}

struct EdgesArray* createEdgesArray(const unsigned int edgesCount){
    struct EdgesArray* newEdgesQueue = malloc(sizeof(struct EdgesArray));
    if (!newEdgesQueue){
        return NULL;
    }

    newEdgesQueue->edges = malloc(sizeof(struct Edge) * edgesCount);
    if (!newEdgesQueue->edges){
        free(newEdgesQueue);
        return NULL;
    }

    newEdgesQueue->edgesCount = edgesCount;

    return newEdgesQueue;
}
void destroyEdgesArray(struct EdgesArray* edgesArray){
    if (!edgesArray){
        return;
    }

    free(edgesArray->edges);
    free(edgesArray);
}
struct Edge getArrayElementValue(const struct EdgesArray* edgesArray,  const unsigned int index){
    if (!edgesArray || edgesArray->edgesCount <= index){
        return EMPTY_EDGE;
    }
    return edgesArray->edges[index];
}
void setArrayElementValue(struct EdgesArray* edgesArray, const unsigned int index, const struct Edge newValue){
    if (!edgesArray || edgesArray->edgesCount <= index){
        return;
    }
    edgesArray->edges[index] = newValue;
}
void printEdgesArray(const struct EdgesArray* edgesArray){
    if (!edgesArray){
        return;
    }

    for (unsigned int i = 0; i < edgesArray->edgesCount; i++){
        const struct Edge curEdge = edgesArray->edges[i];

        if (isEdgeEmpty(curEdge)){
            continue;
        }

        printEdge(curEdge);
    }
}
void sortEdgesArray(struct EdgesArray* edgesArray){
    if (!edgesArray){
        return;
    }

    heapSort(edgesArray->edges, edgesArray->edgesCount);
}
