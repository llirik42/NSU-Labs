#pragma once

#include <stdio.h>
#include "../utils/utils.h"

#define BYTES_PER_PIXEL 3

struct Image;

void GenerateBMP(const struct Image* image, const char* path);

struct Image* CreateImage(unsigned int height, unsigned int width, struct Color color);
void DestroyImage(struct Image* image);
void WriteImage(const struct Image* image, FILE* file);
void SetPixelColor(struct Image* image, unsigned int y, unsigned int x, struct Color color);
unsigned int GetImageHeight(const struct Image* image);
unsigned int GetImageWidth(const struct Image* image);
