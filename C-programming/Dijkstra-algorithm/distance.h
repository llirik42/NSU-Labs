#pragma once

#include <stdbool.h>
#include <stdio.h>

typedef unsigned int Number;

struct Distance{
    Number distance;
    bool isIntMaxPlus;
    bool isInfinity;
};

void printDistance(struct Distance distance, FILE* file);
bool isDistanceIntMaxPlus(struct Distance distance);
bool isDistanceInfinity(struct Distance distance);
bool compareDistances(struct Distance distance1, struct Distance distance2);
struct Distance createDistance(unsigned int distance);
struct Distance sumDistances(struct Distance distance1, struct Distance distance2);

static const struct Distance ZERO_DISTANCE = {.distance = 0, .isIntMaxPlus = false, .isInfinity = false};
static const struct Distance INT_MAX_PLUS_DISTANCE = {.distance = 0, .isIntMaxPlus = true, .isInfinity = false};
static const struct Distance INFINITE_DISTANCE = {.distance = 0, .isIntMaxPlus = false, .isInfinity = true};
