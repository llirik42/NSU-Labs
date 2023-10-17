#include "disjoint_set.h"
#include <malloc.h>

bool initDisjointSet(struct DisjointSet* disjointSet, const unsigned int verticesCount){
    disjointSet->verticesColors = malloc(sizeof(int) * verticesCount);
    if (!disjointSet->verticesColors){
        return false;
    }

    disjointSet->verticesCount = verticesCount;
    disjointSet->setsCount = verticesCount;
    for (unsigned int i = 0; i < verticesCount; i++){
        disjointSet->verticesColors[i] = i;
    }

    return true;
}
void recolor(struct DisjointSet* disjointSet, const unsigned int oldColor, const unsigned int newColor){
    for (unsigned int i = 0; i < disjointSet->verticesCount; i++){
        if (disjointSet->verticesColors[i] == oldColor){
            disjointSet->verticesColors[i] = newColor;
        }
    }
}

struct DisjointSet* createDisjointSet(const unsigned int verticesCount){
    struct DisjointSet* disjointSet = malloc(sizeof(struct DisjointSet));
    if (!disjointSet){
        return NULL;
    }

    if (!initDisjointSet(disjointSet, verticesCount)){
        destroyDisjointSet(disjointSet);
        return NULL;
    }

    return disjointSet;
}
void destroyDisjointSet(struct DisjointSet* disjointSet){
    if (!disjointSet){
        return;
    }

    free(disjointSet->verticesColors);
    free(disjointSet);
}

bool sameComponent(const struct DisjointSet* disjointSet, const unsigned int vertex1, const unsigned int vertex2){
    if (vertex1 >= disjointSet->verticesCount || vertex2 >= disjointSet->verticesCount){
        return false;
    }
    return disjointSet->verticesColors[vertex1] == disjointSet->verticesColors[vertex2];
}
void unite(struct DisjointSet* disjointSet, const unsigned int vertex1, const unsigned int vertex2){
    if (vertex1 >= disjointSet->verticesCount || vertex2 >= disjointSet->verticesCount){
        return;
    }

    const unsigned int newCommonColor = disjointSet->verticesColors[vertex1];
    const unsigned int colorToRecolor = disjointSet->verticesColors[vertex2];

    recolor(disjointSet, newCommonColor, colorToRecolor);

    disjointSet->setsCount--;
}
