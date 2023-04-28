#include "priority_queue.h"
#include <stdbool.h>
#include <malloc.h>

struct Queue{
    struct Node** elements;
    unsigned short elementsCount;
};

void swapElements(struct Node** node1, struct Node** node2){
    struct Node* tmp = *node1;
    *node1 = *node2;
    *node2 = tmp;
}
bool compare(const struct Node* node1, const struct Node* node2){
    return node1->frequency < node2->frequency;
}
void siftUp(struct Queue* queue, unsigned char v){
    while (v){
        const unsigned char parent = (v - 1) / 2;

        if (compare(queue->elements[v], queue->elements[parent])){
            swapElements(&queue->elements[v], &queue->elements[parent]);
            v = (v - 1) / 2;
        }
        else{
            break;
        }
    }
}
void siftDown(struct Queue* queue, unsigned char v){
    while (v * 2 + 1 <= queue->elementsCount){
        Byte u = v * 2 + 1;

        const unsigned char rightChild = v * 2 + 2;

        if (rightChild <= queue->elementsCount && compare(queue->elements[rightChild], queue->elements[u])){
            u = rightChild;
        }
        if (compare(queue->elements[u], queue->elements[v])){
            swapElements(&queue->elements[u], &queue->elements[v]);
            v = u;
        }
        else{
            break;
        }
    }
}

struct Queue* CreateQueue(const unsigned short maxSize){
    struct Queue* queue = malloc(sizeof(struct Queue));
    if (!queue){
        return NULL;
    }

    queue->elements = malloc(sizeof(struct Node*) * maxSize);
    if (!queue->elements){
        free(queue);
        return NULL;
    }

    queue->elementsCount = 0;
    return queue;
}
void DestroyQueue(struct Queue* queue){
    if (queue){
        free(queue->elements);
        free(queue);
    }
}
void Insert(struct Queue* queue, struct Node* node){
    queue->elements[queue->elementsCount] = node;
    siftUp(queue, queue->elementsCount);
    queue->elementsCount++;
}
struct Node* ExtractMinimum(struct Queue* queue){
    if (queue->elementsCount == 0){
        return NULL;
    }

    queue->elementsCount--;
    struct Node* result = queue->elements[0];
    queue->elements[0] = queue->elements[queue->elementsCount];
    siftDown(queue, 0);
    return result;
}
