#include <stdio.h>
#include <math.h>

int sgn(int x){
    if (x > 0){
        return 1;
    }

    return -1;
}

int getRemainder(int number, int divisor){
    if (number > 0){
        return number % divisor;
    }

    while (1){
        number += divisor;

        if (number > 0){
            return number % divisor;
        }
    }
}

void deleteElement(int arr[], int len, int index){
    if(index < 0 || index >= len){
        printf("I FUCK YOU BULLSHIT\n");

        return;
    }

    int* copy = arr;

    for(int i = index; i < len; i++){
        arr[i] = copy[i + 1];
    }

    arr[len - 1] = 0;
}

int count(int n, int k){
    int remaining[1000], curIndex = k > 0 ? 0 : n, remainingCounter = n;

    if (k == 1){
        return n;
    }

    if (k == -1){
        return 1;
    }

    for (int i = 0; i < n; i++){
        remaining[i] = i + 1;
    }

    for (int i = 0; i < n - 1; i++){
        for (int j = 0; j < abs(k) - (k > 0); j++){
            curIndex += sgn(k);

            curIndex = getRemainder(curIndex, remainingCounter);
        }

        deleteElement(remaining, remainingCounter, curIndex);

        remainingCounter--;
    }

    return remaining[0];
}

int main() {
    printf("%d", count(5, 2));

    return 0;
}
