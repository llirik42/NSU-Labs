#include <stdio.h>
#include <stdlib.h>

#include "circular_doubly_linked_list.h"

void checkNull(void* pointer){
    if (pointer == NULL){
        printf("NULL error");
        exit(EXIT_FAILURE);
    }
}

struct Node* createNewNode(size_t value){
    struct Node* result = (struct Node*) malloc(sizeof(struct Node));

    checkNull(result);

    result->value = value;
    result->next = NULL;

    return result;
}

void linkNodes(struct Node* node1, struct Node* node2){
    node1->next = node2;
    node2->prev = node1;
}

struct CDList* createNewList(size_t length){
    struct CDList* result = (struct CDList*) malloc(sizeof(struct CDList));

    checkNull(result);

    result->length = length;

    result->first = createNewNode(1);

    struct Node* prevNode = result->first;

    for (size_t index = 2; index <= length; index++){
        struct Node* curNode = createNewNode(index);

        linkNodes(prevNode, curNode);

        prevNode = curNode;
    }

    linkNodes(prevNode, result->first);

    return result;
}

void printList(struct CDList* list){
    checkNull(list);

    struct Node* curNode = list->first;

    for (size_t index = 0; index < list->length; index++){
        printf("%d ", curNode->value);

        curNode = curNode->next;
    }
    printf("\n");
}

void freeList(struct CDList* list){
    checkNull(list);

    struct Node* prevNode = list->first;
    for (size_t index = 0; index < list->length; index++){
        struct Node* curNode = prevNode->next;

        free(prevNode);

        prevNode = curNode;
    }

    free(list);
}

size_t count(size_t N, int K){
    struct CDList* list = createNewList(N);

    struct Node* prevNode = K > 0 ? list->first : list->first->prev;
    size_t curK = 1;
    while (list->length != 1){
        struct Node* curNode = K > 0 ? prevNode->next : prevNode->prev;

        if (curK % abs(K) == 0){
            linkNodes(prevNode->prev, prevNode->next);

            if (prevNode == list->first){
                list->first = prevNode->next;
            }

            free(prevNode);
            list->length--;
        }

        curK++;

        prevNode = curNode;
    }

    size_t result = list->first->value;

    freeList(list);

    return result;
}
