#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "singly_linked_list.h"

bool isNull(void* pointer){
    return pointer == NULL;
}

void checkNull(void* object){
    if (isNull(object)){
        printf("NULL error");

        exit(EXIT_SUCCESS);
    }
}

struct Node {
    size_t value;

    struct Node* next;
};

struct Node* createNewNode(size_t value){
    struct Node* result = (struct Node*) malloc(sizeof(struct Node));

    checkNull(result);

    result->value = value;
    result->next = NULL;

    return result;
}

struct SinglyLinkedList* createNewList(){
    struct SinglyLinkedList* result = (struct SinglyLinkedList*) malloc(sizeof(struct SinglyLinkedList));

    checkNull(result);

    result->first = NULL;
    result->last = NULL;
    result->length = 0;
    result->isEmpty = true;

    return result;
}

void append(struct SinglyLinkedList* list, size_t value){
    checkNull(list);

    struct Node* newNode = createNewNode(value);

    list->length++;

    if (!list->isEmpty){
        list->last->next = newNode;
        list->last = newNode;
        return;
    }

    list->first = newNode;
    list->last = newNode;
    list->isEmpty = false;
}

void printList(struct SinglyLinkedList* list){
    checkNull(list);

    if (list->isEmpty){
        printf("Empty list\n");
        return;
    }

    for (struct Node* curNode = list->first; curNode != NULL; curNode = curNode->next){
        printf("%d ", curNode->value);
    }

    printf("\n");
}

void freeList(struct SinglyLinkedList* list){
    checkNull(list);

    struct Node* curNode = list->first;

    while (curNode != NULL){
        struct Node* nextNode = curNode->next;

        free(curNode);

        curNode = nextNode;
    }

    free(list);
}
