#include "utils.h"
#include <stdio.h>

static const char* STATUS_TO_MESSAGE[8] = {"success", "bad number of vertices", "bad number of edges", "bad vertex",
                                           "bad length", "bad number of lines", "no memory", "no spanning tree"};

void PrintStatus(Status status){
    printf("%s", STATUS_TO_MESSAGE[status]);
}
