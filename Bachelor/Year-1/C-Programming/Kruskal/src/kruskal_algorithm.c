#include "kruskal_algorithm.h"
#include "disjoint_set.h"
#include <stdio.h>

void printNoSpanningTreeError(){
    printf("no spanning tree");
}
void printResult(const struct EdgesArray* edgesArray, const struct DisjointSet* disjointSet){
    if (disjointSet->setsCount != 1){
        printNoSpanningTreeError();
    }
    else{
        printEdgesArray(edgesArray);
    }
}
void findSpanningTree(struct EdgesArray* edgesArray, struct DisjointSet* disjointSet, const unsigned int edgesCount){
    for (unsigned int i = 0; i < edgesCount; i++){
        const struct Edge curEdge = getArrayElementValue(edgesArray, i);

        const unsigned int v1 = curEdge.vertex1;
        const unsigned int v2 = curEdge.vertex2;

        if (sameComponent(disjointSet, v1, v2)){
            setArrayElementValue(edgesArray, i, EMPTY_EDGE);
            continue;
        }
        unite(disjointSet, v1, v2);
    }
}

void kruskalAlgorithm(struct EdgesArray* edgesArray, const unsigned int verticesCount, const unsigned int edgesCount){
    if (verticesCount == 0){
        printNoSpanningTreeError();
        return;
    }

    struct DisjointSet* disjointSet = createDisjointSet(verticesCount);
    if (!disjointSet){
        return;
    }

    sortEdgesArray(edgesArray);

    findSpanningTree(edgesArray, disjointSet, edgesCount);

    printResult(edgesArray, disjointSet);

    destroyDisjointSet(disjointSet);
}
