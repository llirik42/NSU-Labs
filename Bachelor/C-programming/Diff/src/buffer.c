#include "buffer.h"

void shiftBuffer(struct Buffer* buffer, const Number shift){
    for (Number i = 0; i < buffer->bytesCount - shift; i++){
        buffer->bytes[i] = buffer->bytes[i + shift];
    }

    buffer->bytesCount -= shift;
}
Number readToBuffer(struct Buffer* buffer, FILE* file, const Number offset){
    const Number bytesReadCount = fread(buffer->bytes + offset, sizeof(Byte), MAX_BUFFER_SIZE - offset, file);

    if (!bytesReadCount){
        return 0;
    }

    buffer->bytesCount = bytesReadCount;
    buffer->bytes[bytesReadCount] = '\0';
    return bytesReadCount;
}

void InitBuffer(struct Buffer* buffer){
    buffer->bytesCount = 0;
}
Number WriteInputToBuffer(struct Buffer* buffer, FILE* file){
    return readToBuffer(buffer, file, 0);
}
Number AppendInputToBuffer(struct Buffer* buffer, FILE* file){
    return readToBuffer(buffer, file, buffer->bytesCount);
}
Number SmartShift(struct Buffer* buffer, const Number discrepancyByteIndex){
    if (discrepancyByteIndex == 0){
        return 0;
    }

    Number shift = discrepancyByteIndex + 1 > MAX_STRING_OUTPUT_LENGTH
            ? discrepancyByteIndex + 1 - MAX_STRING_OUTPUT_LENGTH
            : 0;

    for (Number index = discrepancyByteIndex; index > 0; index--){
        if (IS_NEW_LINE(buffer->bytes[index - 1])){
            shift = index;
            break;
        }
    }

    shiftBuffer(buffer, shift);
    return shift;
}
Number CompareBuffers(const struct Buffer* buffer1, const struct Buffer* buffer2){
    const Number stopIndex = MAX(buffer1->bytesCount, buffer2->bytesCount);
    for (Number index = 0; index < stopIndex; index++) {
        if (buffer1->bytes[index] != buffer2->bytes[index]) {
            return index + 1;
        }
    }
    return 0;
}

Byte GetByteInTxt(const struct Buffer* buffer, const Number index){
    return buffer->bytesCount ? buffer->bytes[index] : '\0';
}
const char* GetByteInHex(const struct Buffer* buffer, const Number index){
    const Byte byte = buffer->bytesCount > index ? buffer->bytes[index] : 0;
    return ToDoubleHex(byte);
}
Number GetNewLineSymbolsCount(const struct Buffer* buffer, Number index){
    Number result = 0;

    for (Number i = 0; i < index; i++){
        result += buffer->bytes[i] == '\n';
    }
    return result;
}
