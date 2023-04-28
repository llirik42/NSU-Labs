#pragma once

#include "edge.h"

struct EdgesArray{
    struct Edge* edges;
    unsigned int edgesCount;
};

struct EdgesArray* createEdgesArray(unsigned int edgesCount);
void destroyEdgesArray(struct EdgesArray* edgesArray);
struct Edge getArrayElementValue(const struct EdgesArray* edgesArray, unsigned int index);
void setArrayElementValue(struct EdgesArray* edgesArray, unsigned int index, struct Edge newValue);
void printEdgesArray(const struct EdgesArray* edgesArray);
void sortEdgesArray(struct EdgesArray* edgesArray);
