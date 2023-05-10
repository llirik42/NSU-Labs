#pragma once

#include <aio.h>
#include <malloc.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

typedef u_int8_t Cell;

#define GET_CELL(G, W, Y, X) ((G)[(Y) * (W) + (X)])
#define SET_CELL(G, W, Y, X, V) ((G)[(Y) * (W) + (X)] = (V))
#define EQUAL(G1, G2, H, W) (memcmp((G1), (G2), (H) * (W) * sizeof(Cell)) == 0)
#define CREATE_GENERATION(H, W) (calloc((H) * (W), sizeof(Cell)))
#define EVOLVE_CELL(IS_ALIVE, ALIVE_NEIGHBOURS) ((ALIVE_NEIGHBOURS) == 3 || ((IS_ALIVE) && (ALIVE_NEIGHBOURS) == 2))

/*
 * Reads generation from the file.
 *
 * Return value — number of alive cells in the generation.
 */
unsigned long read_generation(Cell* generation, unsigned long height, unsigned long width, FILE* file);

/*
 * Writes generation to the buffer.
 */
void write_generation(const Cell* generation, unsigned long height, unsigned long width, char* buffer);

/*
 * Evolves cell in the old generation.
 *
 * Return value — whether cell will be alive or not.
 */
bool evolve(const Cell* old_generation,
            Cell* new_generation,
            unsigned long width,
            unsigned long alive_neighbours_count,
            unsigned long y,
            unsigned long x);
