#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <unistd.h>
#include <stdlib.h>
#include "../algorithm/matrix.h"

#define MAX_PATH_LENGTH 256

void read_grid_config(const char* path, size_t* p1, size_t* p2) {
    assert(path != NULL);

    size_t local_p1, local_p2;

    FILE* file = fopen(path, "r");
    if (file == NULL) {
        fprintf(stderr, "Incorrect path to grid config!\n");
        return;
    }

    fscanf(file, "%zu %zu", &local_p1, &local_p2);

    if (p1) {
        *p1 = local_p1;
    }
    if (p2) {
        *p2 = local_p2;
    }

    fclose(file);
}

bool test(size_t test_index) {
    char a_path[MAX_PATH_LENGTH] = {'\0'};
    char b_path[MAX_PATH_LENGTH] = {'\0'};
    char expected_path[MAX_PATH_LENGTH] = {'\0'};
    char grid_config_path[MAX_PATH_LENGTH] = {'\0'};

    sprintf(a_path, "../src/tests/test_%zu/a.txt", test_index);
    sprintf(b_path, "../src/tests/test_%zu/b.txt", test_index);
    sprintf(expected_path, "../src/tests/test_%zu/expected.txt", test_index);
    sprintf(grid_config_path, "../src/tests/test_%zu/grid_config.txt", test_index);

    size_t p1, p2;
    read_grid_config(grid_config_path, &p1, &p2);

    struct Matrix* a = read_matrix(a_path);
    struct Matrix* b = read_matrix(b_path);
    struct Matrix* expected = read_matrix(expected_path);

    system("mpicc ../src/algorithm/main.c ../src/algorithm/matrix.c -o main.out");
    system("mpirun -np 8 ")








    destroy_matrix(a);
    destroy_matrix(b);
    destroy_matrix(expected);

    return true;
}

bool test_1() {
    return test(1);
}

bool test_2() {
    return test(2);
}

bool test_3() {
    return test(3);
}

int main() {
    bool (*tests[])() = {
            test_1,
            //test_2,
            //test_3
    };

    const size_t tests_count = sizeof(tests) / sizeof(bool*);

    for (size_t i = 0; i < tests_count; i++) {
        const bool test_result = tests[i]();

        test_result
            ? fprintf(stderr, "Test %zu passed\n", i + 1)
            : fprintf(stderr, "Test %zu failed\n", i + 1);
    }
}
