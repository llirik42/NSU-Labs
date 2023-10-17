#pragma once

#include <stdbool.h>

struct DisjointSet{
    unsigned int verticesCount;
    unsigned int* verticesColors;
    unsigned int setsCount;
};

struct DisjointSet* createDisjointSet(unsigned int verticesCount);
void destroyDisjointSet(struct DisjointSet* disjointSet);

bool sameComponent(const struct DisjointSet* disjointSet, unsigned int vertex1, unsigned int vertex2);
void unite(struct DisjointSet* disjointSet, unsigned int vertex1, unsigned int vertex2);
