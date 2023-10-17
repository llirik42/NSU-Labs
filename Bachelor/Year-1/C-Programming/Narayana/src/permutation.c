#include "permutation.h"
#include <stdio.h>

bool isCharDigit(const int c){
    return '0' <= c && c <= '9';
}
Digit charToDigit(const int c){
    return c - '0';
}
void swap(Digit* c1, Digit* c2){
    const unsigned char tmp = *c1;
    *c1 = *c2;
    *c2 = tmp;
}

void printPermutation(const struct Permutation* permutation){
    if (!permutation){
        return;
    }

    for (unsigned char i = 0; i < permutation->length; i++){
        printf("%d", permutation->elements[i]);
    }
    printf("\n");
}
void inversePermutationPart(struct Permutation* permutation, const Digit j){
    unsigned int t = (permutation->length - j + 1) / 2;

    for (unsigned int i = 0; i < t; i++){
        swapElements(permutation, j + i, permutation->length - i - 1);
    }
}
void swapElements(struct Permutation* permutation, const Digit index1, const Digit index2){
    swap(&permutation->elements[index1], &permutation->elements[index2]);
}
bool inputPermutation(struct Permutation* permutation){
    unsigned char curIndex = 0;

    bool digitsCount[10] = {0};

    int curChar = getchar();
    while (curChar != '\n'){
        if (!isCharDigit(curChar)){
            return false;
        }

        const unsigned int curDigit = charToDigit(curChar);

        if (digitsCount[curDigit]){
            return false;
        }

        digitsCount[curDigit] = true;
        permutation->elements[curIndex++] = charToDigit(curChar);
        curChar = getchar();
    }

    permutation->length = curIndex;

    return true;
}
Digit getElement(const struct Permutation* permutation, const Digit index){
    if (!permutation || permutation->length <= index){
        return 0;
    }
    return permutation->elements[index];
}
Digit getLength(const struct Permutation* permutation){
    if (!permutation){
        return 0;
    }
    return permutation->length;
}
