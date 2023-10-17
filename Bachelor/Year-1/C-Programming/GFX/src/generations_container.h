#pragma once

#include "generation.h"
#include <stdbool.h>

struct Container;

struct Container* CreateContainer();
void DestroyContainer(struct Container* container);
bool Push(struct Container* container, struct Generation* generation);
bool Repeat(const struct Container* container);
