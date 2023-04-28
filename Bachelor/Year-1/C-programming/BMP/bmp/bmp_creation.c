#include "bmp.h"

typedef unsigned short Word;
typedef unsigned int Dword;
typedef int Long;

#pragma pack(push, 1)
struct BitmapFileHeader{
    // Отметка для отличия формата от других (сигнатура формата)
    // Может содержать единственное значение 0x4D42/0x424D
    Word signature;

    // Размер файла в байтах
    Dword fileSize;

    // Зарезервированы и должны содержать ноль.
    Word reserved1;
    Word reserved2;

    // Положение пиксельных данных относительно начала данной структуры (в байтах)
    Dword pixelsDataOffset;
};
struct BitmapInfoHeader{
    // Размер данного заголовка
    Dword headerSize;

    // Ширина растра в пикселях. Указывается целым числом со знаком
    // Ноль и отрицательные не документированы
    Long width;

    // Целое число со знаком, содержащее два параметра: высота растра в пикселях
    // (абсолютное значение числа) и порядок следования строк в двумерных массивах (знак числа)
    // Нулевое значение не документировано
    Long height;

    // Количество "color planes" (в BMP допустимо только значение 1)
    Word numberOfPlanes;

    // Количество бит на пиксель
    Word bitsPerPixel;

    // Способ хранения пикселей
    Dword compression;

    // Размер пиксельных данных в байтах (может быть 0, если хранение осуществляется двумерным массивом)
    Dword imageSize;

    // Количество пикселей на метр по горизонтали и вертикали
    Long horizontalResolution;
    Long verticalResolution;

    // Размер таблицы цветов в ячейках
    Dword colorTableSize;

    // Количество ячеек от начала таблицы цветов до последней используемой (включая её саму)
    Dword importantColorCount;
};
#pragma pack(pop)

void initBitmapFileHeader(struct BitmapFileHeader* bitmapFileHeader, const unsigned int height, const unsigned int stride){
    const unsigned int headersSize = sizeof(struct BitmapFileHeader) + sizeof(struct BitmapInfoHeader);

    bitmapFileHeader->signature = (Word) 0x4D42;
    bitmapFileHeader->fileSize = (Dword) (headersSize + (stride * height));
    bitmapFileHeader->reserved1 = 0;
    bitmapFileHeader->reserved2 = 0;
    bitmapFileHeader->pixelsDataOffset = (Dword) (headersSize);
}
void initBitmapInfoHeader(struct BitmapInfoHeader* bitmapInfoHeader, const unsigned int height, const unsigned int width){
    bitmapInfoHeader->headerSize = (Dword) sizeof(struct BitmapInfoHeader);
    bitmapInfoHeader->width = (Long) (width);
    bitmapInfoHeader->height = (Long) (height);
    bitmapInfoHeader->numberOfPlanes = (Word) 1;

    // Умножение на 8 так как поле "numberOfPlanes" = 1 (то есть зависит от цветового формата)
    bitmapInfoHeader->bitsPerPixel = (Word) (BYTES_PER_PIXEL * 8);

    bitmapInfoHeader->compression = 0;
    bitmapInfoHeader->imageSize = 0;
    bitmapInfoHeader->horizontalResolution = 0;
    bitmapInfoHeader->verticalResolution = 0;
    bitmapInfoHeader->colorTableSize = 0;
    bitmapInfoHeader->importantColorCount = 0;
}

void GenerateBMP(const struct Image* image, const char* path){
    if (!image || !path){
        return;
    }

    const unsigned int height = GetImageHeight(image);
    const unsigned int width = GetImageWidth(image);

    const unsigned int widthInBytes = GetImageWidth(image) * BYTES_PER_PIXEL;
    const unsigned int paddingSize = (4 - (widthInBytes) % 4) % 4;
    const unsigned int stride = widthInBytes + paddingSize;

    FILE* imageFile = fopen(path, "wb+");
    if (!imageFile){
        printf("Can't create bmp\n");
    }

    struct BitmapFileHeader bitmapFileHeader;
    initBitmapFileHeader(&bitmapFileHeader, height, stride);
    fwrite(&bitmapFileHeader, sizeof(struct BitmapFileHeader), 1, imageFile);

    struct BitmapInfoHeader bitmapInfoHeader;
    initBitmapInfoHeader(&bitmapInfoHeader, height, width);
    fwrite(&bitmapInfoHeader, sizeof(struct BitmapInfoHeader), 1, imageFile);

    WriteImage(image, imageFile);

    fclose(imageFile);
}
