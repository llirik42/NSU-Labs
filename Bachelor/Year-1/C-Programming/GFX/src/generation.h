#pragma once

#include <stdbool.h>

struct Generation;

struct Generation* CreateEmptyGeneration();
void FillGenerationRandomly(struct Generation* generation);
void FillGenerationByMask(struct Generation* generation);
struct Generation* CreateRandomGeneration();
void DestroyGeneration(struct Generation* generation);
void CopyGeneration(const struct Generation* from, struct Generation* to);
bool Evolve(struct Generation* generation);
bool Equal(const struct Generation* generation1, const struct Generation* generation2);
bool IsCellAlive(const struct Generation* generation, unsigned int h, unsigned int w);
bool IsGenerationDead(const struct Generation* generation);
