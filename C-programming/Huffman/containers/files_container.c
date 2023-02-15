#include "files_container.h"
#include <malloc.h>

struct FilesContainer* CreateFilesContainer(unsigned short filesCount){
    struct FilesContainer* filesContainer = malloc(sizeof(struct FilesContainer));
    if (!filesContainer){
        return NULL;
    }

    filesContainer->data = malloc(sizeof(struct FileData) * filesCount);
    if (!filesContainer->data){
        free(filesContainer);
        return NULL;
    }

    filesContainer->filesCounter = 0;

    return filesContainer;
}
void AddFile(struct FilesContainer* filesContainer, const struct FileData fileData){
    if (!filesContainer){
        return;
    }

    filesContainer->data[filesContainer->filesCounter++] = fileData;
}
FILE* GetFile(const struct FilesContainer* filesContainer, unsigned short index){
    return filesContainer->data[index].file;
}
const char* GetName(const struct FilesContainer* filesContainer, unsigned short index){
    return filesContainer->data[index].name;
}
bool Contains(const struct FilesContainer* filesContainer, const struct FileData fileData){
    for (unsigned short i = 0; i < filesContainer->filesCounter; i++){
        if (filesContainer->data[i].file == fileData.file){
            return true;
        }
    }
    return false;
}
unsigned short GetFilesCount(const struct FilesContainer* filesContainer){
    return filesContainer->filesCounter;
}
void DestroyFilesContainer(struct FilesContainer* filesContainer){
    if (!filesContainer){
        return;
    }

    for (unsigned short i = 0; i < filesContainer->filesCounter; i++){
        fclose(GetFile(filesContainer, i));
    }

    free(filesContainer->data);
    free(filesContainer);
}
struct FileData CreateFileData(FILE* file, const char* name){
    struct FileData fileData = {.file = file, .name = name};
    return fileData;
}
