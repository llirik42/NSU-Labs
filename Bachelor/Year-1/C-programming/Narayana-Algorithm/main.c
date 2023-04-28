#include <stdio.h>
#include <stdbool.h>
#include "permutation.h"

void printBadInput(){
    printf("bad input");
}
bool inputData(struct Permutation* permutation, unsigned int* permutationsCount){
    const int codeOfPermutationInput = inputPermutation(permutation);
    if (codeOfPermutationInput != 1){
        return false;
    }

    const int codeOfPermutationsCountInput = scanf("%u", permutationsCount);
    if (codeOfPermutationsCountInput != 1){
        return false;
    }

    return true;
}

Digit findJ(struct Permutation* permutation){
    Digit j = 0;

    const Digit length = getLength(permutation);

    for (unsigned int i = length - 1; i > 0; i--){
        const Digit curElement = getElement(permutation, i - 1);
        const Digit nextElement = getElement(permutation, i);

        if (curElement < nextElement){
            j = i;
            break;
        }
    }
    return j;
}
Digit findL(struct Permutation* permutation, const Digit j){
    const Digit jElement = getElement(permutation, j - 1);

    Digit l = 0;

    const Digit length = getLength(permutation);

    for (unsigned int i = length; i > j; i--){
        const Digit curElement = getElement(permutation, i - 1);

        if (jElement < curElement){
            l = i;
            break;
        }
    }

    return l;
}
void narayanaAlgorithm(struct Permutation* permutation, const unsigned int permutationsCount){
    for (unsigned int k = 0; k < permutationsCount; k++){
        const Digit j = findJ(permutation);
        const Digit l = findL(permutation, j);

        if (j == 0){
            return;
        }

        swapElements(permutation, j - 1, l - 1);

        inversePermutationPart(permutation, j);

        printPermutation(permutation);
    }
}

int main() {
    struct Permutation firstPermutation;
    unsigned int permutationsCount;

    if (!inputData(&firstPermutation, &permutationsCount)){
        printBadInput();
        return 0;
    }
    if (permutationsCount == 0){
        return 0;
    }

    narayanaAlgorithm(&firstPermutation, permutationsCount);

    return 0;
}
