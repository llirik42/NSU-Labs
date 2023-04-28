#include <time.h>

#include "utils.h"
#include "config.h"

int main(){
    time_t start_time = clock();

    const size_t vectors_size = input_vectors_size();
    double* vector1 = create_vector(vectors_size);
    double* vector2 = create_vector(vectors_size);
    input_vectors(vector1, vector2);

    double result = operation(vector1,
                              vector2,
                              vectors_size,
                              vectors_size);

    time_t end_time = clock();

    print_time(start_time / 1000, end_time / 1000);

    check_result(result);

    destroy_vector(vector1);
    destroy_vector(vector2);
}
