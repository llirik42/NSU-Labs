#include "data_input.h"
#include "quick_sort.h"
#include <stdio.h>
#include <malloc.h>

void printArray(int* array, const unsigned int length){
    for (unsigned int index = 0; index < length; index++){
        printf("%d ", array[index]);
    }
    printf("\n");
}

int main() {
    unsigned int length = 0;
    if (!inputLength(&length) || length == 0){
        return 0;
    }

    int* array = malloc(sizeof(int) * length);
    if (!inputArray(array, length)){
        free(array);
        return 0;
    }

    quickSort(array, length);

    printArray(array, length);

    free(array);

    return 0;
}
