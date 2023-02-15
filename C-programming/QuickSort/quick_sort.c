#include "quick_sort.h"

void swap(int* a, int* b){
    const int tmp = *a;
    *a = *b;
    *b = tmp;
}
unsigned int partition(int* array, unsigned int l, unsigned int r){
    int pivot = array[(l + r) / 2];

    while (1){
        while (array[l] < pivot){
            l++;
        }
        while (array[r] > pivot){
            r--;
        }

        if (r <= l){
            return r;
        }

        swap(&array[l], &array[r]);
        l++;
        r--;
    }
}
void sortArray(int* array, const unsigned int l, const unsigned int r){
    if (l == r){
        return;
    }

    if (l - r == 1){
        if (array[l] > array[r]){
            swap(&array[l], &array[r]);
        }
        return;
    }

    unsigned int m = partition(array, l, r);

    sortArray(array, l, m);
    sortArray(array, m + 1, r);
}

void quickSort(int* array, const unsigned int length){
    if (!array){
        return;
    }

    sortArray(array, 0, length - 1);
}
