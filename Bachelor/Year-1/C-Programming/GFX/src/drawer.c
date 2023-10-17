#include "drawer.h"
#include "config.h"
#include "utils.h"
#include <SDL.h>
#include <malloc.h>

struct Drawer{
    SDL_Window* window;
    SDL_Renderer* renderer;
};

void set_pixel(SDL_Renderer *render, int x, int y, COLOR){
    SDL_SetRenderDrawColor(render, r, g, b, a);
    SDL_RenderDrawPoint(render, x, y);
}

void drawEmptyCircle(SDL_Renderer* renderer, int cx, int cy, int radius, COLOR){
    double error = (double)-radius;
    double x = (double)radius - 0.01;
    double y = (double)0.01;
    double cx2 = cx - 0.01;
    double cy2 = cy - 0.01;

    while (x >= y)
    {
        set_pixel(renderer, (int)(cx2 + x), (int)(cy2 + y), r, g, b, a);
        set_pixel(renderer, (int)(cx2 + y), (int)(cy2 + x), r, g, b, a);
        set_pixel(renderer, (int)(cx2 - x), (int)(cy2 + y), r, g, b, a);
        set_pixel(renderer, (int)(cx2 + y), (int)(cy2 - x), r, g, b, a);
        set_pixel(renderer, (int)(cx2 + x), (int)(cy2 - y), r, g, b, a);
        set_pixel(renderer, (int)(cx2 - y), (int)(cy2 + x), r, g, b, a);
        set_pixel(renderer, (int)(cx2 - x), (int)(cy2 - y), r, g, b, a);
        set_pixel(renderer, (int)(cx2 - y), (int)(cy2 - x), r, g, b, a);

        error += y;
        y++;
        error += y;

        if (error >= 0)
        {
            x--;
            error -= x;
            error -= x;
        }
    }
}
void drawFullCircle(SDL_Renderer* renderer, int cx, int cy, int radius, COLOR){
    for (int i = 1; i < radius; i++){
        drawEmptyCircle(renderer, cx, cy, i, r, g, b, a);
    }
}

struct Drawer* CreateDrawer(){
    struct Drawer* drawer = malloc(sizeof(struct Drawer));

    if (!drawer){
        return NULL;
    }

    if(SDL_Init( SDL_INIT_VIDEO ) < 0){
        DestroyDrawer(drawer);
        return NULL;
    }

    drawer->window = SDL_CreateWindow(
            WINDOW_TITLE,
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            SCREEN_WIDTH,
            SCREEN_HEIGHT,
            SDL_WINDOW_SHOWN);

    if (!drawer->window){
        DestroyDrawer(drawer);
        return NULL;
    }

    drawer->renderer = SDL_CreateRenderer(drawer->window, FIRST_DRIVER_INDEX, SDL_RENDERER_ACCELERATED);

    return drawer;
}
void DrawField(struct Drawer* drawer){
    SDL_SetRenderDrawColor(drawer->renderer, BACKGROUND_COLOR);
    SDL_RenderClear(drawer->renderer);

    SDL_SetRenderDrawColor(drawer->renderer, BORDERS_COLOR);
    const int step = SCREEN_WIDTH / FIELD_WIDTH;
   for (int h = 0; h < FIELD_HEIGHT; h++){
        for (int w = 0; w < FIELD_WIDTH; w++){
            SDL_Rect rect = {.x = w * step, .y = h * step, .w = step, .h = step};
            SDL_RenderDrawRect(drawer->renderer, &rect);
        }
    }
    SDL_RenderPresent(drawer->renderer);
}
void DrawGeneration(struct Drawer* drawer, const struct Generation* generation){
    const int step = SCREEN_WIDTH / FIELD_WIDTH;
    const int radius = (int) (RATIO_OF_DIAMETER_TO_SIDE * step / 2);

    for (int h = 0; h < FIELD_HEIGHT; h++){
        for (int w = 0; w < FIELD_WIDTH; w++){
            const int cx = w * step + step / 2;
            const int cy = h * step + step / 2;

            if (IsCellAlive(generation, h, w)){
                drawFullCircle(drawer->renderer, cx, cy, radius, ALIVE_CELLS_COLOR);
            }
            else{
                drawFullCircle(drawer->renderer, cx, cy, radius, BACKGROUND_COLOR);
            }
        }
    }

    SDL_RenderPresent(drawer->renderer);

    SDL_Delay(DELAY_AFTER_GENERATION_DRAWING);
}
void DestroyDrawer(struct Drawer* drawer){
    if (drawer){
        if (drawer->window){
            SDL_DestroyWindow(drawer->window);
        }
        if (drawer->renderer){
            SDL_DestroyRenderer(drawer->renderer);
        }
        free(drawer);
        SDL_Quit();
    }
}
bool CheckQuit(){
    SDL_Event event;

    while(SDL_PollEvent(&event))
    {
        if(event.type == SDL_QUIT )
        {
            return false;
        }
    }
    return true;
}
