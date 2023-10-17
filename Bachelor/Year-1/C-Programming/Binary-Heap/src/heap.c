#include "heap.h"

void swap(Byte* byte1, Byte* byte2){
    *byte1 = *byte1 ^ *byte2;
    *byte2 = *byte1 ^ *byte2;
    *byte1 = *byte1 ^ *byte2;
}
void siftUp(struct Heap* heap, unsigned char v){
    while (v){
        const unsigned char parent = (v - 1) / 2;

        if (heap->elements[v] < heap->elements[parent]){
            swap(&heap->elements[v], &heap->elements[parent]);
            v = (v - 1) / 2;
        }
        else{
            break;
        }
    }
}
void siftDown(struct Heap* heap, unsigned char v){
    while (v * 2 + 1 <= heap->elementsCount){
        Byte u = v * 2 + 1;
        if (v * 2 + 2 <= heap->elementsCount && heap->elements[v * 2 + 2] < heap->elements[u]){
            u = v * 2 + 2;
        }

        if (heap->elements[u] < heap->elements[v]){
            swap(&heap->elements[u], &heap->elements[v]);
            v = u;
        }
        else{
            break;
        }
    }
}

void InitHeap(struct Heap* heap){
    heap->elementsCount = 0;
}
void Insert(struct Heap* heap, Byte byte){
    heap->elements[heap->elementsCount++] = byte;
    siftUp(heap, heap->elementsCount - 1);
}
int ExtractMinimum(struct Heap* heap){
    if (heap->elementsCount == 0){
        return -1;
    }

    heap->elementsCount--;
    const Byte result = heap->elements[0];
    heap->elements[0] = heap->elements[heap->elementsCount];
    siftDown(heap, 0);
    return result;
}
