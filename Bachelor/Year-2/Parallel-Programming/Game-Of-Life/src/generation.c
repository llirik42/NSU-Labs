#include "generation.h"

#define CHAR_TO_DIGIT(C) ((C) - '0')
#define DIGIT_TO_CHAR(D) ((D) + '0')

unsigned long read_generation(Cell* generation, unsigned long height, unsigned long width, FILE* file) {
    unsigned long ret = 0;

    char c;
    for (unsigned long y = 0; y < height; y++) {
        for (unsigned long x = 0; x < width; x++) {
            fread(&c, sizeof(char), 1, file);
            char cell = CHAR_TO_DIGIT(c);
            ret += cell;
            SET_CELL(generation, width, y, x, cell);
        }
        fread(&c, sizeof(char), 1, file);
    }

    return ret;
}

void write_generation(const Cell* generation, unsigned long height, unsigned long width, char* buffer) {
    for (unsigned long y = 0; y < height; y++) {
        for (unsigned long x = 0; x < width; x++) {
            buffer[y * (width + 1) + x] = DIGIT_TO_CHAR(GET_CELL(generation, width, y, x));
        }
        buffer[y * (width + 1) + width] = '\n';
    }
}

bool evolve(const Cell* old_generation,
                Cell* new_generation,
                unsigned long width,
                unsigned long alive_neighbours_count,
                unsigned long y,
                unsigned long x) {

    const bool is_alive = GET_CELL(old_generation, width, y, x);
    const bool will_be_alive = EVOLVE_CELL(is_alive, alive_neighbours_count);
    SET_CELL(new_generation, width, y, x, will_be_alive);
    return will_be_alive;
}
