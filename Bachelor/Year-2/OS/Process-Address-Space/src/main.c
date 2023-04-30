#include <stdio.h>
#include <string.h>
#include "my_heap.h"

int main() {
    FILE* file = fopen("dump.bin", "w");

    const int init_code = init_my_heap();

    if (init_code == ERROR_CODE) {
        perror(NULL);
        return ERROR_CODE;
    }

    char* p1 = my_malloc(2);
    printf("%p\n", p1);
    p1[0] = '1';
    p1[1] = '2';

    char* p2 = my_malloc(2);
    printf("%p\n", p2);
    p2[0] = 'a';
    p2[1] = 'b';

    char* p3 = my_malloc(1000);
    memset(p3, 128, 1000);
    printf("%p\n", p3);

    my_free(p3);

    char* p4 = my_malloc(500);
    memset(p4, 255, 500);
    printf("%p\n", p4);

    dump_my_heap(file);

    destroy_my_heap();

    fclose(file);
}
