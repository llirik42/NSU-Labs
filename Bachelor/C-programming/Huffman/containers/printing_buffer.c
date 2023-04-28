#include "printing_buffer.h"

Status handleBufferOverflow(struct PrintingBuffer* printingBuffer){
    if (printingBuffer->bitsCount == PRINTING_BUFFER_SIZE * SIZE_OF_BYTE){
        HANDLE_OPERATION(DumpPrintingBuffer(printingBuffer))
        printingBuffer->bitsCount = 0;
    }
    return SUCCESS;
}
Status addBitToBuffer(struct PrintingBuffer* printingBuffer, const bool bit){
    SET_BIT_8(
            printingBuffer->elements[printingBuffer->bitsCount / SIZE_OF_BYTE], \
            printingBuffer->bitsCount % SIZE_OF_BYTE,
            bit)
    printingBuffer->bitsCount++;
    return handleBufferOverflow(printingBuffer);
}

void InitPrintingBuffer(struct PrintingBuffer* buffer, FILE* output){
    buffer->bitsCount = 0;
    buffer->output = output;
}
Status PushCode(struct PrintingBuffer* printingBuffer, const Code code){
    for (unsigned short i = 0; i < code.bitsCount; i++){
        HANDLE_OPERATION(addBitToBuffer(printingBuffer, GET_BIT_CODE(code, i)))
    }
    return SUCCESS;
}
Status PushByte(struct PrintingBuffer* printingBuffer, Byte byte){
    for (unsigned char i = 0; i < SIZE_OF_BYTE; i++){
        HANDLE_OPERATION(addBitToBuffer(printingBuffer, GET_BIT_8(byte, i)))
    }
    return SUCCESS;
}
Status PushBit(struct PrintingBuffer* printingBuffer, const bool bit){
    return addBitToBuffer(printingBuffer, GET_BIT_8(bit, SIZE_OF_BYTE - 1));
}
Status DumpPrintingBuffer(const struct PrintingBuffer* buffer){
    const unsigned int bytesToWriteCount = CEIL(buffer->bitsCount, SIZE_OF_BYTE);

    const unsigned int writtenBytesCount = fwrite(buffer->elements, sizeof(Byte), bytesToWriteCount, buffer->output);

    return writtenBytesCount == bytesToWriteCount ? SUCCESS : EXECUTION_ERROR;
}
