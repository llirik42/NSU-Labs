#include "config.h"
#include "generation.h"
#include "utils.h"
#include <malloc.h>
#include <stdlib.h>

struct Generation{
    Byte** cells;
    unsigned int hash;
};

#define GET_GENERATION_BYTE(G, H, W) (G)->cells[H][(W) / SIZE_OF_BYTE]
#define GET_CELL(G, H, W) GET_BIT_8(GET_GENERATION_BYTE(G, H, W), (W) % SIZE_OF_BYTE)
#define SET_CELL(G, H, W, V) SET_BIT_8(GET_GENERATION_BYTE(G, H, W), (W) % SIZE_OF_BYTE, V)

void setBorderCellsZero(struct Generation* generation){
    const unsigned char bitsInLastByte = ACTUAL_FIELD_WIDTH_BYTES * SIZE_OF_BYTE - FIELD_WIDTH;

    for (unsigned int h = 0; h < FIELD_HEIGHT; h++){
        for (unsigned int w = 0; w < bitsInLastByte; w++){
            SET_CELL(generation, h, ACTUAL_FIELD_WIDTH_BYTES * SIZE_OF_BYTE - w - 1, 0)
        }
    }
}

unsigned char getNeighboursCount(const struct Generation* generation, const unsigned int h, const unsigned int w){
    unsigned char result = 0;

    const bool firstLine = h == 0;
    const bool bottomLine = (h == (FIELD_HEIGHT - 1));

    const bool leftColumn = w == 0;
    const bool rightColumn = (w == (FIELD_WIDTH - 1));

    if (!firstLine){
        result += GET_CELL(generation, h - 1, w);

        if (!leftColumn){
            result += GET_CELL(generation, h - 1, w - 1);
        }
        if (!rightColumn){
            result += GET_CELL(generation, h - 1, w + 1);
        }
    }

    if (!bottomLine){
        result += GET_CELL(generation, h + 1, w);
        if (!leftColumn){
            result += GET_CELL(generation, h + 1, w - 1);
        }
        if (!rightColumn){
            result += GET_CELL(generation, h + 1, w + 1);
        }
    }

    if (!leftColumn){
        result += GET_CELL(generation, h, w - 1);
    }
    if (!rightColumn){
        result += GET_CELL(generation, h, w + 1);
    }

    return result;
}


struct Generation* CreateEmptyGeneration(){
    struct Generation* generation = malloc(sizeof(struct Generation));
    if (!generation){
        return NULL;
    }

    generation->cells = malloc(sizeof(Byte*) * FIELD_HEIGHT);
    if (!generation->cells){
        DestroyGeneration(generation);
        return NULL;
    }

    for (unsigned int h = 0; h < FIELD_HEIGHT; h++){
        generation->cells[h] = malloc(ACTUAL_FIELD_WIDTH_BYTES);

        if (!generation->cells[h]){
            DestroyGeneration(generation);
            return NULL;
        }
    }

    return generation;
}
void FillGenerationRandomly(struct Generation* generation){
    generation->hash = 0;

    for (unsigned int h = 0; h < FIELD_HEIGHT; h++){
        for (unsigned int w = 0; w < FIELD_WIDTH; w++){
            const bool cellValue = rand() % 2;

            SET_CELL(generation, h, w, cellValue);
            generation->hash += cellValue;
        }
    }

    setBorderCellsZero(generation);
}
void FillGenerationByMask(struct Generation* generation){
    generation->hash = 0;

    for (unsigned int h = 0; h < FIELD_HEIGHT; h++){
        for (unsigned int w = 0; w < FIELD_WIDTH; w++){
            const bool newCell = FIRST_GENERATION[h][w];

            SET_CELL(generation, h, w, newCell)
            generation->hash += newCell;
        }
    }
}
void DestroyGeneration(struct Generation* generation){
    if (generation){
        if (generation->cells){
            for (unsigned int h = 0; h < FIELD_HEIGHT; h++){
                free(generation->cells[h]);
            }
            free(generation->cells);
        }
        free(generation);
    }
}
void CopyGeneration(const struct Generation* from, struct Generation* to){
    for (unsigned int h = 0; h < FIELD_HEIGHT; h++){
        for (unsigned int w = 0; w < ACTUAL_FIELD_WIDTH_BYTES; w++){
            const Byte newByte = from->cells[h][w];

            to->cells[h][w] = newByte;
            to->hash += GetBitsCount(newByte);
        }
    }

    to->hash = from->hash;
}
bool Evolve(struct Generation* generation){
    struct Generation* tmpGeneration = CreateEmptyGeneration();

    tmpGeneration->hash = 0;

    for (unsigned int h = 0; h < FIELD_HEIGHT; h++){
        for (unsigned int w = 0; w < FIELD_WIDTH; w++){
            unsigned int curCount = getNeighboursCount(generation, h, w);

            const bool isCurCellAlive = GET_CELL(generation, h, w);
            const bool newCell = curCount == 3 || (isCurCellAlive && curCount == 2);

            tmpGeneration->hash += newCell;
            SET_CELL(tmpGeneration, h, w, newCell)
        }
    }

    CopyGeneration(tmpGeneration, generation);

    DestroyGeneration(tmpGeneration);

    return true;
}
bool Equal(const struct Generation* generation1, const struct Generation* generation2){
    if (generation1->hash != generation2->hash){
        return false;
    }

    for (unsigned int h = 0; h < FIELD_HEIGHT; h++){
        for (unsigned int w = 0; w < ACTUAL_FIELD_WIDTH_BYTES; w++){
            if (generation1->cells[h][w] != generation2->cells[h][w]){
                return false;
            }
        }
    }

    return true;
}
bool IsCellAlive(const struct Generation* generation, unsigned int h, unsigned int w){
    return GET_CELL(generation, h, w);
}
bool IsGenerationDead(const struct Generation* generation){
    return generation->hash == 0;
}
