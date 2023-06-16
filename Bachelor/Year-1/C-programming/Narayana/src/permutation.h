#pragma once

#include <stdbool.h>

typedef unsigned char Digit;

struct Permutation{
    Digit elements[10];
    Digit length;
};

void printPermutation(const struct Permutation* permutation);
void inversePermutationPart(struct Permutation* permutation, Digit j);
void swapElements(struct Permutation* permutation, Digit index1, Digit index2);
bool inputPermutation(struct Permutation* permutation);
Digit getElement(const struct Permutation* permutation, Digit index);
Digit getLength(const struct Permutation* permutation);
