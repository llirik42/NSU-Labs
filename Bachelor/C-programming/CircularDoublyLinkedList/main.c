#include "circular_doubly_linked_list.h"
#include <stdio.h>

int main() {
    size_t N;
    int K;

    scanf_s("%d%d", &N, &K);

    printf("%d\n", count(N, K));

    return 0;
}
