#pragma once

#include "generation.h"
#include <stdbool.h>

struct Drawer;

struct Drawer* CreateDrawer();
void DrawField(struct Drawer* drawer);
void DrawGeneration(struct Drawer* drawer, const struct Generation* generation);
void DestroyDrawer(struct Drawer* drawer);
bool CheckQuit();
