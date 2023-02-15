#include "generations_container.h"
#include <malloc.h>

struct ContainerElement{
    struct Generation* generation;
    struct ContainerElement* next;
};

struct Container{
    struct ContainerElement* head;
    struct ContainerElement* tail;
};

struct ContainerElement* createContainerElement(struct Generation* generation){
    struct ContainerElement* containerElement = malloc(sizeof(struct ContainerElement));

    if (!containerElement){
        return NULL;
    }

    containerElement->generation = generation;
    containerElement->next = NULL;

    return containerElement;
}
void destroyContainerElement(struct ContainerElement* containerElement){
    DestroyGeneration(containerElement->generation);
    free(containerElement);
}

struct Container* CreateContainer(){
    struct Container* container = malloc(sizeof(struct Container));

    if (!container){
        return NULL;
    }

    container->head = NULL;
    container->tail = NULL;

    return container;
}
void DestroyContainer(struct Container* container){
    if (container){
        struct ContainerElement* curElement = container->head;

        while (curElement){
            struct ContainerElement* nextElement = curElement->next;

            destroyContainerElement(curElement);

            curElement = nextElement;
        }
        free(container);
    }
}
bool Push(struct Container* container, struct Generation* generation){
    struct ContainerElement* newContainerElement = createContainerElement(generation);

    if (!newContainerElement){
        return false;
    }

    if (!container->head){
        container->head = newContainerElement;
    }
    else{
        container->tail->next = newContainerElement;
    }

    container->tail = newContainerElement;

    return true;
}
bool Repeat(const struct Container* container){
    if (!container->head){
        return false;
    }

    if (container->head == container->tail){
        return false;
    }

    const struct Generation* tailElementGeneration = container->tail->generation;

    struct ContainerElement* curElement = container->head;

    while (curElement != container->tail){
        if (Equal(curElement->generation, tailElementGeneration)){
            return true;
        }

        curElement = curElement->next;
    }

    return false;
}
