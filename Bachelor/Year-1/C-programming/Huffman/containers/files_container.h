#pragma once

#include <stdio.h>
#include <stdbool.h>

struct FileData{
    FILE* file;
    const char* name;
};
struct FilesContainer{
    struct FileData* data;
    unsigned short filesCounter;
};


struct FileData CreateFileData(FILE* file, const char* name);

struct FilesContainer* CreateFilesContainer(unsigned short filesCount);
void AddFile(struct FilesContainer* filesContainer, struct FileData fileData);
FILE* GetFile(const struct FilesContainer* filesContainer, unsigned short index);
const char* GetName(const struct FilesContainer* filesContainer, unsigned short index);
bool Contains(const struct FilesContainer* filesContainer, struct FileData fileData);
unsigned short GetFilesCount(const struct FilesContainer* filesContainer);
void DestroyFilesContainer(struct FilesContainer* filesContainer);
