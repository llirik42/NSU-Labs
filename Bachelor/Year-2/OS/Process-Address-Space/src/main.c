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

    char* p1 = my_malloc(3);
    p1[0] = '1';
    p1[1] = '2';
    p1[2] = '\0';

    char* p2 = my_malloc(3);
    p2[0] = 'p';
    p2[1] = 'q';
    p2[2] = '\0';

    char* p3 = my_malloc(4);
    p3[0] = 'F';
    p3[1] = 'F';
    p3[2] = 'F';
    p3[3] = '\0';

    my_free(p2);

    char* p4 = my_malloc(3);
    memset(p4, 'H', 2);
    p4[2] = '\0';

    dump_my_heap(file);

    printf("%s\n", p1);
    printf("%s\n", p2);
    printf("%s\n", p3);
    printf("%s\n", p4);

    dump_my_heap(file);

    destroy_my_heap();

    fclose(file);
}
