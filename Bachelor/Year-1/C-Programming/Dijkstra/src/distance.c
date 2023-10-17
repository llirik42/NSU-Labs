#include "distance.h"
#include <limits.h>

void printDistance(const struct Distance distance, FILE* file){
    if (isDistanceInfinity(distance)){
        fprintf(file, "oo");
        return;
    }
    if (isDistanceIntMaxPlus(distance)){
        fprintf(file, "INT_MAX+");
        return;
    }
    fprintf(file,"%u", distance.distance);
}
bool isDistanceIntMaxPlus(const struct Distance distance){
    return distance.isIntMaxPlus;
}
bool isDistanceInfinity(const struct Distance distance){
    return distance.isInfinity;
}
bool compareDistances(const struct Distance distance1, const struct Distance distance2){
    if (isDistanceInfinity(distance1)){
        return false;
    }
    if (isDistanceInfinity(distance2)){
        return true;
    }

    if (isDistanceIntMaxPlus(distance1)){
        return false;
    }
    if (isDistanceIntMaxPlus(distance2)){
        return true;
    }

    return distance1.distance < distance2.distance;
}
struct Distance sumDistances(const struct Distance distance1, const struct Distance distance2){
    if (isDistanceInfinity(distance1) || isDistanceInfinity(distance2)){
        return INFINITE_DISTANCE;
    }

    if (isDistanceIntMaxPlus(distance1) || isDistanceIntMaxPlus(distance2)){
        return INT_MAX_PLUS_DISTANCE;
    }

    return createDistance(distance1.distance + distance2.distance);
}
struct Distance createDistance(const unsigned int distance){
    if (distance > INT_MAX){
        return INT_MAX_PLUS_DISTANCE;
    }

    struct Distance newDistance = {.distance = distance, .isIntMaxPlus = false, .isInfinity = false};
    return newDistance;
}
