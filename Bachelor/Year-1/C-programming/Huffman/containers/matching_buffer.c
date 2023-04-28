#include "matching_buffer.h"

#define MAX_SIZE_BYTES (MATCHING_BUFFER_SIZE * SIZE_OF_UINT256_BYTES)
#define MAX_SIZE_BITS (MATCHING_BUFFER_SIZE * SIZE_OF_UINT256_BITS)

#define NEXT_CHAR_INDEX MAX_SIZE_BYTES
#define NEXT_NEXT_CHAR_INDEX (MAX_SIZE_BYTES + 1)
#define NO_EOF (MAX_SIZE_BYTES + 2)

#define GET_BUFFER_BYTE(B, I) (B->elements[(I) / SIZE_OF_UINT256_BYTES].bytes[(I) % SIZE_OF_UINT256_BYTES])
#define SET_BUFFER_BYTE(B, I, V) (B)->elements[(I) / SIZE_OF_UINT256_BYTES].bytes[(I) % SIZE_OF_UINT256_BYTES] = (V);

#define GET_BUFFER_BIT(B, I) GET_BIT_256((B)->elements[(I) / SIZE_OF_UINT256_BITS], (I) % SIZE_OF_UINT256_BITS)
#define SET_BUFFER_BIT(B, I, V) SET_BIT_256(B->elements[(I) / SIZE_OF_UINT256_BITS], (I) % SIZE_OF_UINT256_BITS, V)

unsigned int read(struct MatchingBuffer* matchingBuffer, const unsigned int offset){
    for (unsigned int i = offset; i < MAX_SIZE_BYTES; i++){
        const int curChar = fgetc(matchingBuffer->in);
        if (curChar == EOF){
            return i;
        }

        SET_BUFFER_BYTE(matchingBuffer, i, curChar)
    }

    const int char1 = fgetc(matchingBuffer->in);
    if (char1 == EOF){
        return NEXT_CHAR_INDEX;
    }
    matchingBuffer->nextChar = char1;

    const int char2 = fgetc(matchingBuffer->in);
    if (char2 == EOF){
        return NEXT_NEXT_CHAR_INDEX;
    }
    matchingBuffer->nextNextChar = char2;

    return NO_EOF;
}
void adjustDataAfterReading(struct MatchingBuffer* matchingBuffer, const unsigned int indexOfEOF){
    matchingBuffer->canRead = indexOfEOF == NO_EOF;

    unsigned char bitsInLastByte;
    if (indexOfEOF == NO_EOF){
        matchingBuffer->maxOffset = MAX_SIZE_BITS;
        return;
    }
    else if (indexOfEOF == NEXT_NEXT_CHAR_INDEX){
        bitsInLastByte = matchingBuffer->nextChar;
    }
    else{
        bitsInLastByte = GET_BUFFER_BYTE(matchingBuffer, indexOfEOF - 1);
    }
    bitsInLastByte = bitsInLastByte ? bitsInLastByte : SIZE_OF_BYTE;

    matchingBuffer->maxOffset = bitsInLastByte + (indexOfEOF - 2) * SIZE_OF_BYTE;
}
void moveBitsFromEnd(struct MatchingBuffer* matchingBuffer, const unsigned short bitsCount){
    const unsigned int delta = matchingBuffer->maxOffset - bitsCount;
    for (unsigned short i = 0; i < bitsCount; i++){
        SET_BUFFER_BIT(matchingBuffer, i, GET_BUFFER_BIT(matchingBuffer, delta + i))
    }
}
void moveBuffer(struct MatchingBuffer* matchingBuffer, const unsigned short shift){
    const long newOffset = ((long) matchingBuffer->offset) - ((long) matchingBuffer->maxOffset) + ((long) shift) + SIZE_OF_UINT256_BITS;
    if (!matchingBuffer->canRead || newOffset <= 0){
        matchingBuffer->offset += shift;
        return;
    }

    moveBitsFromEnd(matchingBuffer, SIZE_OF_UINT256_BITS);
    matchingBuffer->offset = newOffset;

    SET_BUFFER_BYTE(matchingBuffer, SIZE_OF_UINT256_BYTES, matchingBuffer->nextChar)
    SET_BUFFER_BYTE(matchingBuffer, SIZE_OF_UINT256_BYTES + 1, matchingBuffer->nextNextChar)
    adjustDataAfterReading(matchingBuffer, read(matchingBuffer, SIZE_OF_UINT256_BYTES + 2));
}

void InitMatchingBuffer(struct MatchingBuffer* matchingBuffer, const struct Node* huffmanTreeRoot, FILE* in){
    matchingBuffer->huffmanTreeRoot = huffmanTreeRoot;
    matchingBuffer->in = in;
    matchingBuffer->offset = 0;
    adjustDataAfterReading(matchingBuffer, read(matchingBuffer, 0));
}
Byte Match(struct MatchingBuffer* matchingBuffer){
    Byte result;
    unsigned int shift = 0;

    if (IS_NODE_LEAF(matchingBuffer->huffmanTreeRoot)){
        result = matchingBuffer->huffmanTreeRoot->byte;
        shift = 1;
    }
    else{
        const struct Node* curNode = matchingBuffer->huffmanTreeRoot;
        while (!IS_NODE_LEAF(curNode)){
            curNode = GET_CHILD(curNode, GET_BUFFER_BIT(matchingBuffer, shift + matchingBuffer->offset));
            shift++;
        }
        result = GET_LEAF_BYTE(curNode);
    }

    moveBuffer(matchingBuffer, shift);

    return result;
}
