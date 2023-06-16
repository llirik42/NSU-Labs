#include "huffman.h"
#include "single_huffman.h"
#include <limits.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

#define FILE_NAME_STOP_SYMBOL '*'
#define FILE_SIZE_STOP_SYMBOL 'b'

void printInputFilesCount(FILE* out, const unsigned short inputFilesCount){
    fprintf(out, "%c", inputFilesCount - 1);
}
void printFileInfo(FILE* out, FILE* file, const char* name){
    const long curSize = GetFileSize(file);
    fprintf(out, "%s%c%ld%c", name, FILE_NAME_STOP_SYMBOL, curSize, FILE_SIZE_STOP_SYMBOL);
}

void fillFileName(char* name, FILE* in){
    unsigned int index = 0;
    int curChar;
    while ((curChar = fgetc(in)) != FILE_NAME_STOP_SYMBOL){
        name[index++] = (char) curChar;
    }
    name[index] = '\0';
}
unsigned int readFileSize(FILE* in){
    unsigned int result = 0;

    int curChar;
    while ((curChar = fgetc(in)) != FILE_SIZE_STOP_SYMBOL){
        result = result * 10 + CHAR_TO_DIGIT(curChar);
    }

    return result;
}
FILE* openFileInDirectory(const char* directoryPath, const char* curName){
    char curPath[1024];

    sprintf(curPath, "%s/%s", directoryPath, curName);

    FILE* result = fopen(curPath, "wb");

    return result;
}

Status multipleDecompression(const struct FilesContainer* filesContainer, const unsigned short compressedFilesCount, FILE* in){
    const char* directoryPath = GetName(filesContainer, 0);

    mkdir(directoryPath, 777);

    for (unsigned short i = 0; i < compressedFilesCount; i++){
        char curName[1024];
        fillFileName(curName, in);

        unsigned int curSize = readFileSize(in);

        FILE* curFile = openFileInDirectory(directoryPath, curName);

        const Status curStatus = SingleDecompress(in, curFile, curSize);
        if (curStatus != SUCCESS){
            return curStatus;
        }
    }

    return SUCCESS;
}

Status compress(const struct FilesContainer* filesContainer){
    const unsigned short inputFilesCount = GetFilesCount(filesContainer) - 1;

    FILE* out = GetFile(filesContainer, inputFilesCount);

    printInputFilesCount(out, inputFilesCount);

    const bool singleSituation = inputFilesCount == 1;

    if (singleSituation){
        FILE* in = GetFile(filesContainer, 0);
        return SingleCompress(in, out);
    }

    for (unsigned short i = 0; i < inputFilesCount; i++){
        FILE* curFile = GetFile(filesContainer, i);
        const char* curName = GetName(filesContainer, i);

        printFileInfo(out, curFile, curName);

        const Status curStatus = SingleCompress(curFile, out);
        if (curStatus != SUCCESS){
            return curStatus;
        }
    }
    return SUCCESS;
}
Status decompress(const struct FilesContainer* filesContainer){
    const unsigned short filesCount = GetFilesCount(filesContainer);

    FILE* in = GetFile(filesContainer, 0);

    const int compressedFilesByte = fgetc(in);

    if (compressedFilesByte == EOF){
        return INCORRECT_COMPRESSION;
    }

    const unsigned short compressedFilesCount = compressedFilesByte + 1;

    if (compressedFilesCount == 1){
        if (filesCount != 2){
            return INCORRECT_COMPRESSION;
        }
        return SingleDecompress(in, GetFile(filesContainer, 1), UINT_MAX);
    }

    return multipleDecompression(filesContainer, compressedFilesCount, in);
}

Status Huffman(bool toCompress, const struct FilesContainer* filesContainer){
    return toCompress ? compress(filesContainer) : decompress(filesContainer);
}
