#pragma once

#define FIELD_HEIGHT 16
#define FIELD_WIDTH 16

#define SCREEN_HEIGHT 800
#define SCREEN_WIDTH (SCREEN_HEIGHT * (((double) (FIELD_WIDTH)) / (FIELD_HEIGHT)))
#define WINDOW_TITLE "The life game"

#define BACKGROUND_COLOR 255, 255, 255, 255
#define ALIVE_CELLS_COLOR 0, 0, 0, 0
#define BORDERS_COLOR 0, 0, 0, 0
#define RATIO_OF_DIAMETER_TO_SIDE 0.5

#define DELAY_AFTER_GENERATION_DRAWING 100
#define RANDOM 0

static const unsigned char FIRST_GENERATION[FIELD_HEIGHT][FIELD_WIDTH] = {
        {0},
        {0},
        {0},
        {0, 0, 0, 0, 0, 1, 1, 1},
        {0, 0, 0, 0, 0, 1, 0, 1},
        {0, 0, 0, 0, 0, 1, 0, 1}
};
