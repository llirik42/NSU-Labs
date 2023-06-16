#include "utils.h"
#include "containers/files_container.h"
#include "huffman/huffman.h"
#include <stdio.h>
#include <stdbool.h>

const char* chooseMode(const bool toCompress, const bool isFileLast, const unsigned short filesCount){
    if (toCompress || filesCount != 1){
        return isFileLast ? "wb" : "rb";
    }

    return "rb";
}

Status DoGitLabTesting(){
    FILE* in = fopen("in.txt", "rb");
    FILE* out = fopen("out.txt", "wb");

    if (!in || !out){
        if (in){
            fclose(in);
        }
        if (out){
            fclose(out);
        }
        return INCORRECT_INPUT;
    }

    Status status = SUCCESS;
    if (!IsFileEmpty(in, true)){
        const bool toCompress = fgetc(in) != 'd';

        struct FilesContainer* filesContainer = CreateFilesContainer(2);
        AddFile(filesContainer, CreateFileData(in, NULL));
        AddFile(filesContainer, CreateFileData(out, NULL));

        status = Huffman(toCompress, filesContainer);

        DestroyFilesContainer(filesContainer);
    }
    else{
        fclose(in);
        fclose(out);
    }
    
    return status;
}
Status DefaultExecution(int arc, char* argv[]){
    if (arc < 3){
        return INCORRECT_INPUT;
    }

    const bool toCompress = CompareStrings(argv[1], "-c");
    const bool toDecompress = CompareStrings(argv[1], "-d");

    if (!toCompress && !toDecompress){
        return INCORRECT_MODE;
    }

    const unsigned short filesCount = arc - 2;
    if ((toCompress && filesCount < 2) || (toDecompress && filesCount > 2)){
        return INCORRECT_INPUT;
    }

    struct FilesContainer* filesContainer = CreateFilesContainer(filesCount);
    for (unsigned short i = 0; i < filesCount; i++){
        const char* curPath = argv[i + 2];

        const bool isFIleLast = i == (filesCount - 1);
        const char* curMode = chooseMode(toCompress, isFIleLast, filesCount);

        FILE* curFile = fopen(curPath, curMode);

        if (!curFile){
            DestroyFilesContainer(filesContainer);
            return INCORRECT_PATHS;
        }

        struct FileData curFileData = CreateFileData(curFile, SeparateNameFromFullPath(argv[i + 2]));

        if (Contains(filesContainer, curFileData)){
            DestroyFilesContainer(filesContainer);
            return INCORRECT_INPUT;
        }

        AddFile(filesContainer, curFileData);
    }

    const Status status = Huffman(toCompress, filesContainer);

    DestroyFilesContainer(filesContainer);

    return status;
}

int main(int arc, char* argv[]){
    const bool gitLabTesting = arc == 1;

    const Status status = gitLabTesting ? DoGitLabTesting() : DefaultExecution(arc, argv);

    PrintStatus(status);

    return 0;
}
