#include "mini_buffer.h"

#define GET_MINI_BUFFER_BIT(BUFFER, I) GET_BIT_8((BUFFER)->bytes[(I) / SIZE_OF_BYTE], (I) % SIZE_OF_BYTE)
#define SET_MINI_BUFFER_BIT(BUFFER, I, V) SET_BIT_8((BUFFER)->bytes[(I) / SIZE_OF_BYTE], (I) % SIZE_OF_BYTE, V)

void shiftMiniBuffer(struct MiniBuffer* miniBuffer, unsigned char shift){
    for (unsigned short i = 0; i < miniBuffer->bitsLeft - shift; i++){
        SET_MINI_BUFFER_BIT(miniBuffer, i, GET_MINI_BUFFER_BIT(miniBuffer, i + shift))
    }
    miniBuffer->bitsLeft -= shift;
}
bool readAndAppend(struct MiniBuffer* miniBuffer){
    const int newChar = fgetc(miniBuffer->in);

    if (newChar == EOF){
        return false;
    }

    for (unsigned char i = 0; i < SIZE_OF_BYTE; i++){
        SET_MINI_BUFFER_BIT(miniBuffer, i + miniBuffer->bitsLeft, GET_BIT_8(newChar, i))
    }

    miniBuffer->bitsLeft += SIZE_OF_BYTE;

    return true;
}

void InitMiniBuffer(struct MiniBuffer* miniBuffer, FILE* in){
    miniBuffer->in = in;
    miniBuffer->bitsLeft = 0;
}
bool InputMiniBuffer(struct MiniBuffer* miniBuffer){
    const int char1 = fgetc(miniBuffer->in);
    if (char1 == EOF){
        return false;
    }
    const int char2 = fgetc(miniBuffer->in);
    if (char2 == EOF){
        return false;
    }

    miniBuffer->bytes[0] = (Byte) char1;
    miniBuffer->bytes[1] = (Byte) char2;
    miniBuffer->bitsLeft = SIZE_OF_BYTE * MINI_BUFFER_SIZE;

    return true;
}
char ExtractBit(struct MiniBuffer* miniBuffer){
    if (!miniBuffer->bitsLeft){
        if (!InputMiniBuffer(miniBuffer)){
            return EMPTY_BIT;
        }
    }

    const bool result = GET_MINI_BUFFER_BIT(miniBuffer, 0);

    shiftMiniBuffer(miniBuffer, 1);

    return (char) result;
}
int ExtractSymbol(struct MiniBuffer* miniBuffer){
    if (miniBuffer->bitsLeft < SIZE_OF_BYTE && !readAndAppend(miniBuffer)){
        return EOF;
    }

    Byte result = 0;
    for (unsigned char i = 0; i < SIZE_OF_BYTE; i++){
        SET_BIT_8(result, i, GET_MINI_BUFFER_BIT(miniBuffer, i))
    }

    shiftMiniBuffer(miniBuffer, SIZE_OF_BYTE);

    return result;
}
