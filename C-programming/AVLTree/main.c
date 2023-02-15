#include "avl_tree.h"
#include <stdio.h>
#include <stdbool.h>
#include "args.h"

bool inputN(unsigned int* N){
    return scanf("%u", N) == 1;
}

int main(int arc, char* argv[]) {
    unsigned int N = 0;
    if (!inputN(&N)){
        return 0;
    }

    struct Tree* tree = CreateTree(N);

    InputTree(tree);

    if (CheckTKey(arc, argv)){
        PrintTree(tree);
    }
    else{
        printf("%u", GetHeight(tree));
    }

    DestroyTree(tree);

    return 0;
}
