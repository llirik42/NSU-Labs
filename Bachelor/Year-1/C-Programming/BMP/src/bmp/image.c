#include "bmp.h"
#include <malloc.h>

struct Image{
    struct Color** pixels;
    unsigned int height;
    unsigned int width;
};

struct Image* mallocImage(const unsigned int height, const unsigned int width){
    struct Image* result = malloc(sizeof(struct Image));

    if (!result){
        return NULL;
    }

    result->pixels = malloc(sizeof(struct Color*) * height);
    if (!result->pixels){
        free(result);
        return NULL;
    }

    for (unsigned int i = 0; i < height; i++){
        result->pixels[i] = malloc(sizeof(struct Color) * width);

        if (!result->pixels[i]){
            DestroyImage(result);
            return NULL;
        }
    }
    return result;
}
void initImage(struct Image* image, const unsigned int height, const unsigned int width, const struct Color color){
    image->height = height;
    image->width = width;

    for (unsigned int i = 0; i < height; i++) {
        for (unsigned int j = 0; j < width; j++) {
            image->pixels[i][j] = color;
        }
    }
}

void DestroyImage(struct Image* image){
    if (!image){
        return;
    }

    if (!image->pixels){
        free(image);
        return;
    }

    for (unsigned int i = 0; i < image->height; i++){
        free(image->pixels[i]);
    }
    free(image->pixels);
    free(image);
}
struct Image* CreateImage(unsigned int height, unsigned int width, struct Color color){
    struct Image* newImage = mallocImage(height, width);
    if (!newImage){
        return NULL;
    }

    initImage(newImage, height, width, color);

    return newImage;
}
void WriteImage(const struct Image* image, FILE* file){
    if (!file){
        return;
    }

    const unsigned int widthInBytes = image->width * BYTES_PER_PIXEL;
    const unsigned char padding[3] = {0, 0, 0};
    const unsigned int paddingSize = (4 - (widthInBytes) % 4) % 4;

    for (unsigned int i = 0; i < image->height; i++) {
        fwrite(image->pixels[i], BYTES_PER_PIXEL, image->width, file);
        fwrite(padding, 1, paddingSize, file);
    }
}
void SetPixelColor(struct Image* image, unsigned int y, unsigned int x, struct Color color){
    if (!image || !image->pixels || y >= image->height || x >= image->width || !image->pixels[y]){
        return;
    }

    image->pixels[y][x] = color;
}
unsigned int GetImageHeight(const struct Image* image){
    return image ? image->height : 0;
}
unsigned int GetImageWidth(const struct Image* image){
    return image ? image->width : 0;
}
