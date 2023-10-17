#include "data_input.h"
#include <stdio.h>

bool inputLength(unsigned int* length){
    return scanf("%u", length) == 1;
}
bool inputElement(int* element){
    return scanf("%d", element) == 1;
}
bool inputArray(int* array, const unsigned int length){
    for (unsigned int i = 0; i < length; i++){
        int element = 0;

        if (!inputElement(&element)){
            return false;
        }

        array[i] = element;
    }

    return true;
}
