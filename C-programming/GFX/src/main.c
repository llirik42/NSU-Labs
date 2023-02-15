#include "generation.h"
#include "generations_container.h"
#include "drawer.h"
#include "config.h"
#include <stdlib.h>
#include <time.h>

#define DESTROY_ALL DestroyDrawer(drawer); DestroyContainer(container); DestroyGeneration(curGeneration);
#define HANDLE_STRUCT(V, C) (V) = (C); if (!(V)){DESTROY_ALL return 0;}

int main(int argc, char* args[]){
    srand(time(NULL));

    struct Drawer* drawer = NULL;
    struct Container* container = NULL;
    struct Generation* curGeneration = NULL;

    HANDLE_STRUCT(drawer, CreateDrawer())
    HANDLE_STRUCT(container, CreateContainer())
    HANDLE_STRUCT(curGeneration, CreateEmptyGeneration())

    if (RANDOM){
        FillGenerationRandomly(curGeneration);
    }
    else{
        FillGenerationByMask(curGeneration);
    }

    DrawField(drawer);

    while (CheckQuit()){
        struct Generation* oldGeneration = NULL;
        HANDLE_STRUCT(oldGeneration, CreateEmptyGeneration())

        DrawGeneration(drawer, curGeneration);

        CopyGeneration(curGeneration, oldGeneration);

        Push(container, oldGeneration);

        if (Repeat(container) || IsGenerationDead(curGeneration)){
            break;
        }

        Evolve(curGeneration);
    }

    DESTROY_ALL

    return 0;
}
