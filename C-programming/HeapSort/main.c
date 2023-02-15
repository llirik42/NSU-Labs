#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void printBadInput(){
    // Сообщение о некорректном выводе
    
    printf("Bad input");
}

bool inputLength(size_t* length){
    // Ввод длины массива
    // false - произошла ошибка при вводе
    // true - ввод корректен

    if (!scanf("%zu", length)){
        printBadInput();
        return false;
    }
    return true;
}

bool inputElement(int* element){
    // Ввод элемента массива
    // false - произошла ошибка при вводе
    // true - ввод корректен

    if (!scanf("%d", element)){
        printBadInput();
        return false;
    }
    return true;
}

bool inputArray(int* array, size_t length){
    // Ввод массива
    // false - произошла ошибка при вводе
    // true - ввод корректен

    for (size_t index = 0; index < length; index++){
        int element = 0;

        if (!inputElement(&element)){
            return false;
        }

        array[index] = element;
    }

    return true;
}

void printArray(int* array, size_t length){
    // Печать массива

    for (size_t index = 0; index < length; index++){
        printf("%d ", array[index]);
    }
    printf("\n");
}

void swap(int* a, int* b){
    // Обмен переменных значениями

    int tmp = *a;

    *a = *b;
    *b = tmp;
}

void sift_down(int array[], size_t length, size_t curIndex){
    // "Просеивание" элемента

    size_t leftIndex = 2 * curIndex + 1;
    size_t rightIndex = 2 * curIndex + 2;

    size_t indexOfLargest = curIndex;

    if (rightIndex < length && array[rightIndex] > array[indexOfLargest]){
        indexOfLargest = rightIndex;
    }

    if (leftIndex < length && array[leftIndex] > array[indexOfLargest]){
        indexOfLargest = leftIndex;
    }

    if (indexOfLargest != curIndex){
        swap(&array[indexOfLargest], &array[curIndex]);
        sift_down(array, length, indexOfLargest);
    }
}

void heapSort(int array[], size_t length){
    // Сортировка массива

    for (size_t index = length / 2; index > 0; index--){
        sift_down(array, length, index - 1);
    }

    for (size_t index = length; index > 0; index--){
        swap(&array[0], &array[index - 1]);

        sift_down(array, index - 1, 0);
    }
}

int main() {
    size_t length = 0; // Длина массива

    if(!inputLength(&length) || length == 0){
        return EXIT_SUCCESS;
    }

    int* array = malloc(sizeof(int) * length); // Собственно массив

    if (!inputArray(array, length)){
        printBadInput();
        free(array);
        return EXIT_SUCCESS;
    }

    heapSort(array, length);

    printArray(array, length);

    free(array);

    return EXIT_SUCCESS;
}
