#include "diff.h"
#include <math.h>

#define PRINT(format) fprintf(OUTPUT, format)
#define PRINT_DATA(format, data) fprintf(OUTPUT, format, data)

static FILE* OUTPUT;

void printOk(){
    PRINT("OK");
}
void printFilesSizeInfo(FILE* file1, FILE* file2){
    const long bytesDifference = labs(GetFileSize(file1) - GetFileSize(file2));

    bytesDifference ? PRINT_DATA("Files differ by %lu byte(s)", bytesDifference) : PRINT("Files have the same size");
}
void printWideHex(Number number){
    const char* wideHex[MAX_WIDE_HEX_NOTATION_LENGTH_HALVED];

    for (Number i = 0; i < MAX_WIDE_HEX_NOTATION_LENGTH_HALVED; i++){
        wideHex[MAX_WIDE_HEX_NOTATION_LENGTH_HALVED - 1 - i] = ToDoubleHex(number % 256);
        number /= 256;
    }

    for (Number i = 0; i < MAX_WIDE_HEX_NOTATION_LENGTH_HALVED; i++){
        PRINT_DATA("%s", wideHex[i]);
    }
}
void printBlocksBorder(){
    PRINT("| ");
}
void printDoubleHexDifference(const char* doubleHex1, const char* doubleHex2){
    for (Number i = 0; i < MAX_DOUBLE_HEX_NOTATION_LENGTH; i++){
        doubleHex1[i] != doubleHex2[i] ? PRINT("+") : PRINT(" ");
    }
}
void printByteTxt(const Byte byte){
    if (IS_EOF(byte)){
        PRINT("end of file");
    }
    else if (IS_NEW_LINE(byte)){
        PRINT("new line");
    }
    else{
        PRINT_DATA("\'%c\'", byte);
    }
}

void printBufferBinary(const struct Buffer* buffer, const Number offsetInBuffer, const Number globalOffset){
    printWideHex(globalOffset);
    PRINT("  ");

    for (Number i = 0; i < BLOCK_SIZE; i++){
        if (i == BLOCK_SIZE / 2){
            printBlocksBorder();
        }

        PRINT_DATA("%s ", GetByteInHex(buffer, i + offsetInBuffer));
    }
    PRINT("\n");
}
void printBuffersDiscrepancyBinary(const struct Buffer* buffer1, const struct Buffer* buffer2, const Number offset){
    PRINT("          ");

    for (Number index = 0; index < BLOCK_SIZE; index++){
        if (index == BLOCK_SIZE / 2){
            printBlocksBorder();
        }
        const Number indexInBuffers = index + offset;
        const char* hex1 = GetByteInHex(buffer1, indexInBuffers);
        const char* hex2 = GetByteInHex(buffer2, indexInBuffers);

        printDoubleHexDifference(hex1, hex2);

        PRINT(" ");
    }
    PRINT("\n");
}
void printDiscrepancyInfoBinary(const char* hex1, const char* hex2, const Number globalDiscrepancyIndex){
    PRINT_DATA("Discrepancy at byte %u ", globalDiscrepancyIndex);
    PRINT_DATA("(%s VS ", hex1);
    PRINT_DATA("%s)", hex2);
    PRINT("\n\n");
}
void printDiscrepancyBinary(const struct Buffer* buffer1, const struct Buffer* buffer2, const Number bytesReadCount,
                            const Number discrepancyIndex){
    const Number globalOffset = (bytesReadCount + discrepancyIndex) / BLOCK_SIZE * BLOCK_SIZE;
    const Number offsetInBuffer = discrepancyIndex / BLOCK_SIZE * BLOCK_SIZE;
    const Number globalDiscrepancyIndex = discrepancyIndex + bytesReadCount + 1;
    const char* hex1 = GetByteInHex(buffer1, discrepancyIndex);
    const char* hex2 = GetByteInHex(buffer2, discrepancyIndex);

    printDiscrepancyInfoBinary(hex1, hex2, globalDiscrepancyIndex);
    printBufferBinary(buffer1, globalOffset, offsetInBuffer);
    printBuffersDiscrepancyBinary(buffer1, buffer2, offsetInBuffer);
    printBufferBinary(buffer2, globalOffset, offsetInBuffer);

    PRINT("\n");
}

void printBufferTxt(const struct Buffer* buffer){
    const Number stopIndex = MIN(buffer->bytesCount, MAX_STRING_OUTPUT_LENGTH);

    for (Number i = 0; i < stopIndex; i++){
        const Byte curByte = GetByteInTxt(buffer, i);

        if (IS_NEW_LINE(curByte)){
            break;
        }

        PRINT_DATA("%c", curByte);
    }
    PRINT("\n");
}
void printBuffersDiscrepancyTxt(const struct Buffer* buffer1, const struct Buffer* buffer2){
    const Number stopIndex = MIN(MAX(buffer1->bytesCount, buffer2->bytesCount), MAX_STRING_OUTPUT_LENGTH);

    for (Number i = 0; i < stopIndex; i++){
        const Byte byte1 = GetByteInTxt(buffer1, i);
        const Byte byte2 = GetByteInTxt(buffer2, i);

        if (IS_NEW_LINE_OR_EOF(byte1) && IS_NEW_LINE_OR_EOF(byte2)){
            break;
        }

        byte1 != byte2 ? PRINT("+") : PRINT(" ");
    }
    PRINT("\n");
}
void printDiscrepancyInfoTxt(const Byte byte1, const Byte byte2, const Number discrepancyIndex, const Number linesReadCount){
    PRINT_DATA("Discrepancy at byte %u, ", discrepancyIndex + 1);
    PRINT_DATA("at line %u ", linesReadCount + 1);

    PRINT("(");
    printByteTxt(byte1);
    PRINT(" VS ");
    printByteTxt(byte2);
    PRINT(")\n");
}
void PrintDiscrepancyTxt(const struct Buffer* buffer1, const struct Buffer* buffer2, const Number globalDiscrepancyIndex,
                         const Number discrepancyIndexInBuffers, const Number linesReadCount){
    const Byte byte1 = GetByteInTxt(buffer1, discrepancyIndexInBuffers);
    const Byte byte2 = GetByteInTxt(buffer2, discrepancyIndexInBuffers);

    printDiscrepancyInfoTxt(byte1, byte2, globalDiscrepancyIndex, linesReadCount);

    if (!IS_NEW_LINE_OR_EOF(byte1) || !IS_NEW_LINE_OR_EOF(byte2)){
        PRINT("\n");
        printBufferTxt(buffer1);
        printBuffersDiscrepancyTxt(buffer1, buffer2);
        printBufferTxt(buffer2);
    }

    PRINT("\n");
}

bool Diff(FILE* file1, FILE* file2, FILE* output, const bool txtMode) {
    OUTPUT = output;

    struct Buffer buffer1;
    struct Buffer buffer2;

    InitBuffer(&buffer1);
    InitBuffer(&buffer2);

    Number bytesReadCount1 = WriteInputToBuffer(&buffer1, file1);
    Number bytesReadCount2 = WriteInputToBuffer(&buffer2, file2);

    const bool isFile1Empty = bytesReadCount1 == 0;
    const bool isFile2Empty = bytesReadCount2 == 0;
    if (isFile1Empty && isFile2Empty) {
        printOk();
        return false;
    }
    if (isFile1Empty || isFile2Empty) {
        fprintf(output, "One file is empty when other isn't\n\n");
        printFilesSizeInfo(file1, file2);
        return true;
    }

    Number bytesReadCount = 0;
    Number linesReadCount = 0;

    while (bytesReadCount1 && bytesReadCount2){
        Number discrepancyIndex = CompareBuffers(&buffer1, &buffer2);

        // найдено отличие
        if (discrepancyIndex){
            discrepancyIndex--;

            if (!txtMode){
                printDiscrepancyBinary(&buffer1, &buffer2, bytesReadCount, discrepancyIndex);
            }
            else{
                // буферы не отличаются до "discrepancyIndex", поэтому неважно, в каком считать кол-во символов новой строки
                linesReadCount += GetNewLineSymbolsCount(&buffer1, discrepancyIndex);

                // буферы не отличаются до "discrepancyIndex", поэтому их нужно сдвинуть на одно и то же число байт
                const Number shift = SmartShift(&buffer1, discrepancyIndex);
                SmartShift(&buffer2, discrepancyIndex);

                AppendInputToBuffer(&buffer1, file1);
                AppendInputToBuffer(&buffer2, file2);

                const Number globalDiscrepancyIndex = bytesReadCount + discrepancyIndex;
                const Number discrepancyIndexInBuffers = discrepancyIndex - shift;
                PrintDiscrepancyTxt(&buffer1, &buffer2, globalDiscrepancyIndex, discrepancyIndexInBuffers, linesReadCount);
            }

            printFilesSizeInfo(file1, file2);
            return true;
        }

        bytesReadCount += MAX_BUFFER_SIZE;
        bytesReadCount1 = WriteInputToBuffer(&buffer1, file1);
        bytesReadCount2 = WriteInputToBuffer(&buffer2, file2);
        if (txtMode){
            // поскольку содержимое буферов совпадает, то неважно, в каком буфере это смотреть
        }
    }

    fclose(file1);
    fclose(file2);

    printOk();
    return false;
}
