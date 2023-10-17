#include "single_huffman.h"

#include "../containers/huffman_tree.h"
#include "../containers/matching_buffer.h"
#include "../containers/printing_buffer.h"

#define READING_BUFFER_SIZE 8192

void smartFSeek(FILE* file){
    fseek(file, 0, SEEK_SET);
    fseek(file, getc(file) == 'c', SEEK_SET);
}
void readNonCompressed(FILE* in, unsigned long long* frequencies, unsigned short* uniqueCharsCounter){
    while (1){
        Byte buffer[READING_BUFFER_SIZE];
        const unsigned int readCount = fread(buffer, sizeof(Byte), READING_BUFFER_SIZE, in);

        for (unsigned int i = 0; i < readCount; i++){
            const Byte curByte = buffer[i];

            if (!frequencies[curByte]){
                (*uniqueCharsCounter)++;
            }

            frequencies[curByte]++;
        }

        if (readCount != READING_BUFFER_SIZE){
            return;
        }
    }
}

Status writeCompressed(FILE* in, FILE* out, const Code* codes){
    smartFSeek(in);

    struct PrintingBuffer printingBuffer;
    InitPrintingBuffer(&printingBuffer, out);

    while (1){
        Byte buffer[READING_BUFFER_SIZE];
        const unsigned int readCount = fread(buffer, sizeof(Byte), READING_BUFFER_SIZE, in);

        for (unsigned int i = 0; i < readCount; i++){
            HANDLE_OPERATION(PushCode(&printingBuffer, codes[buffer[i]]))
        }

        if (readCount != READING_BUFFER_SIZE){
            break;
        }
    }

    HANDLE_OPERATION(DumpPrintingBuffer(&printingBuffer))

    return fprintf(out, "%c", printingBuffer.bitsCount % 8) == 1 ? SUCCESS : EXECUTION_ERROR;
}
Status writeDecompressed(FILE* in, FILE* out, const struct Node* huffmanTreeRoot, const unsigned int bytesLimit){
    struct PrintingBuffer printingBuffer;
    InitPrintingBuffer(&printingBuffer, out);

    struct MatchingBuffer matchingBuffer;

    InitMatchingBuffer(&matchingBuffer, huffmanTreeRoot, in);

    unsigned int bytesCanMatch = bytesLimit;
    while (CAN_MATCH(matchingBuffer) && bytesCanMatch){
        Byte matchedSymbol = Match(&matchingBuffer);
        HANDLE_OPERATION(PushByte(&printingBuffer, matchedSymbol))
        bytesCanMatch--;
    }

    const long curPosition = ftell(in);
    fseek(in, curPosition - (long) BYTES_LEFT(matchingBuffer) - 1, SEEK_SET);
    return DumpPrintingBuffer(&printingBuffer);
}

Status SingleCompress(FILE* in, FILE* out){
    if (IsFileEmpty(in, false)){
        return SUCCESS;
    }

    unsigned short uniqueCharsCounter = 0;
    unsigned long long frequencies[ALPHABET_SIZE] = {0};
    struct Tree* huffmanTree = NULL;
    Code codes[ALPHABET_SIZE] = {{.bitsCount = 0, .code = {.bytes = {0}}}};

    readNonCompressed(in, frequencies, &uniqueCharsCounter);

    huffmanTree = CreateEmptyTree(uniqueCharsCounter);
    if (!huffmanTree){
        return NO_MEMORY;
    }

    const Status fillingStatus = FillTreeByFrequencies(huffmanTree, frequencies);
    if (fillingStatus != SUCCESS){
        DestroyTree(huffmanTree);
        return fillingStatus;
    }

    FillCodes(huffmanTree, codes);

    const Status status1 = PrintTree(huffmanTree, out);

    DestroyTree(huffmanTree);

    HANDLE_OPERATION(status1)

    return writeCompressed(in, out, codes);
}
Status SingleDecompress(FILE* in, FILE* out, unsigned int bytesLimit){
    if (IsFileEmpty(in, false) || !bytesLimit){
        return SUCCESS;
    }

    struct Tree* huffmanTree = NULL;

    const Status status1 = InputTree(in, &huffmanTree);
    if (status1 != SUCCESS){
        DestroyTree(huffmanTree);
        return status1;
    }

    const Status status2 = writeDecompressed(in, out, GET_ROOT(huffmanTree), bytesLimit);

    DestroyTree(huffmanTree);
    
    return status2;
}
