#include <malloc.h>
#include <stdio.h>

const size_t VECTORS_SIZE = 16 * 24 * 40;

size_t input_vectors_size(){
    return VECTORS_SIZE;
}

void input_vectors(double* vector1, double* vector2){
    for (size_t i = 0; i < VECTORS_SIZE; i++){
        vector1[i] = 1;
        vector2[i] = 1;
    }
}

double* create_vector(size_t size){
    return malloc(size * sizeof(double));
}

void destroy_vector(double* vector){
    free(vector);
}

double operation(const double* vector1, const double* vector2, size_t size1, size_t size2){
    double result = 0;
    for (size_t i = 0; i < size1; i++){
        for (size_t j = 0; j < size2; j++){
            result += vector1[i] * vector2[j];
        }
    }
    return result;
}

void check_result(double result){
    if (result != (double) (VECTORS_SIZE * VECTORS_SIZE)){
        printf("Result is incorrect!\n\n");
    }
}

void print_time(double start_time, double end_time){
    printf("Time: %.0f ms\n", end_time - start_time);
}
