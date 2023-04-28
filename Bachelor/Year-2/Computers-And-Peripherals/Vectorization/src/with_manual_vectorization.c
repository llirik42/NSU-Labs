#include <math.h>
#include <malloc.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <xmmintrin.h>

typedef struct{
    float* elements;
} Matrix;

unsigned int N = 2048;
unsigned int M = 10;

void swap(float* a, float* b){
    float tmp = *a;
    *a = *b;
    *b = tmp;
}
float findMaxAbsSumByRows(const Matrix* matrix){
    float result = 0;

    for (unsigned int i = 0; i < N; i++){
        const float * current_row = matrix->elements + i * N;

        float current_sum = 0;

        for (unsigned int j = 0; j < N; j++){
            current_sum += fabsf(current_row[j]);
        }

        if (current_sum > result){
            result = current_sum;
        }
    }

    return result;
}
float findMaxAbsSumByColumns(const Matrix* matrix){
    float result = 0;

    for (unsigned int i = 0; i < N; i++){
        float current_sum = 0;

        for (unsigned int j = 0; j < N; j++){
            current_sum += fabsf(matrix->elements[j * N + i]);
        }

        if (current_sum > result){
            result = current_sum;
        }
    }

    return result;
}
Matrix* createMatrix(){
    Matrix* new_matrix = malloc(sizeof(Matrix*));

    new_matrix->elements = calloc(N * N, sizeof(float));

    if (!new_matrix->elements){
        free(new_matrix);
        return NULL;
    }

    return new_matrix;
}
void fillMatrix(Matrix* matrix){
    for (unsigned int i = 0; i < N * N; i++){
        matrix->elements[i] = (float) rand() / (float) RAND_MAX;
    }
}
float dot(const float* a, const float* b){
    __m128* x = (void*) a;
    __m128* y = (void*) b;
    __m128 result_vector = _mm_setzero_ps();

    for (unsigned int i = 0; i < N / 4; i++){
        result_vector = _mm_add_ps(result_vector, _mm_mul_ps(*(x + i), *(y + i)));
    }

    return ((float*) &result_vector)[0] + ((float*) &result_vector)[1] + ((float*) &result_vector)[2] + ((float*) &result_vector)[3];
}

Matrix* CreateFilledMatrix(){
    Matrix* result = createMatrix();
    fillMatrix(result);
    return result;
}
Matrix* CreateIdentityMatrix(){
    Matrix* new_matrix = createMatrix();

    for (unsigned int i = 0; i < N; i++){
        new_matrix->elements[i * N + i] = 1;
    }

    return new_matrix;
}
void Copy(Matrix* destination, const Matrix* source){
    for (unsigned int i = 0; i < N * N; i++){
        destination->elements[i] = source->elements[i];
    }
}
Matrix* CreateAndCopy(const Matrix* matrix){
    Matrix* result = createMatrix();
    Copy(result, matrix);
    return result;
}
void DestroyMatrix(Matrix* matrix){
    if (matrix){
        free(matrix->elements);
        free(matrix);
    }
}

void Transpose(const Matrix* matrix){
    for (unsigned int i = 0; i < N; i++){
        for (unsigned int j = i; j < N; j++){
            swap(&(matrix->elements[i * N + j]), &(matrix->elements[j * N + i]));
        }
    }
}
void SubtractMatrix(Matrix* matrix, const Matrix* term){
    __m128* x = (void*) matrix->elements;
    __m128* y = (void*) term->elements;

    for (unsigned int i = 0; i < N * N / 4; i++){
        __m128 tmp = _mm_sub_ps(*(x + i), *(y + i));
        _mm_storeu_ps(matrix->elements + i * 4, tmp);
    }
}
void AddMatrix(Matrix* matrix, const Matrix* term){
    __m128* x = (void*) matrix->elements;
    __m128* y = (void*) term->elements;

    for (unsigned int i = 0; i < N * N / 4; i++){
        __m128 tmp = _mm_add_ps(*(x + i), *(y + i));
        _mm_storeu_ps(matrix->elements + i * 4, tmp);
    }
}

void MultiplyByNumber(Matrix* matrix, float number){
    __m128* x = (void*) matrix->elements;
    __m128 y = _mm_load_ps1(&number);

    for (unsigned int i = 0; i < N * N / 4; i++){
        __m128 tmp = _mm_mul_ps(*(x + i), y);
        _mm_storeu_ps(matrix->elements + i * 4, tmp);
    }
}
Matrix* MultiplyMatrices(const Matrix* matrix1, const Matrix* matrix2_transposed){
    Matrix* result = createMatrix();

    for (unsigned int i = 0; i < N; i++){
        const float* row1 = matrix1->elements + i * N;

        for (unsigned int j = 0; j < N; j++){
            const float* row2 = matrix2_transposed->elements + j * N;

            result->elements[i * N + j] = dot(row1, row2);
        }
    }
    return result;
}
void MultiplyByMatrix(Matrix* matrix, const Matrix* multiplier_transposed){
    Matrix* tmp = MultiplyMatrices(matrix, multiplier_transposed);
    Copy(matrix, tmp);
    DestroyMatrix(tmp);
}
Matrix* FindInverseMatrix(const Matrix* matrix){
    Matrix* B = CreateAndCopy(matrix);
    Transpose(B);

    const float A1 = findMaxAbsSumByColumns(matrix);
    const float A2 = findMaxAbsSumByRows(matrix);

    MultiplyByNumber(B, 1 / A1);
    MultiplyByNumber(B, 1 / A2);

    Matrix* R = CreateIdentityMatrix();

    Matrix* RSeries = CreateIdentityMatrix();

    Matrix* matrix_transposed = CreateAndCopy(matrix);
    Transpose(matrix_transposed);

    Matrix* BA = MultiplyMatrices(B, matrix_transposed);

    DestroyMatrix(matrix_transposed);

    SubtractMatrix(R, BA);

    Transpose(R);

    Matrix* result = CreateIdentityMatrix();

    for (unsigned int i = 1; i < M; i++){
        Matrix* tmp = MultiplyMatrices(RSeries, R);

        Copy(RSeries, tmp);

        AddMatrix(result, RSeries);

        DestroyMatrix(tmp);
    }

    Transpose(B);
    MultiplyByMatrix(result, B);

    DestroyMatrix(B);
    DestroyMatrix(R);
    DestroyMatrix(BA);
    DestroyMatrix(RSeries);

    return result;
}

float GetMatricesDifference(const Matrix* matrix1, const Matrix* matrix2){
    float result = 0;

    for (unsigned int i = 0; i < N * N; i++){
        result += fabsf(matrix1->elements[i] - matrix2->elements[i]);
    }
    return result;
}
void PrintMatrix(const Matrix* matrix){
    for (unsigned int i = 0; i < N; i++){
        for (unsigned int j = 0; j < N; j++){
            printf("%f ", matrix->elements[N * i + j]);
        }
        printf("\n");
    }
    printf("\n");
}

int main(int arc, char** argv){
    if (arc == 3){
        N = strtoull(argv[1], NULL, 10);
        M = strtoull(argv[2], NULL, 10);
    }

    Matrix* matrix = CreateFilledMatrix();

    time_t t0;
    time(&t0);

    Matrix* result = FindInverseMatrix(matrix);

    time_t t1;
    time(&t1);

    Transpose(result);

    MultiplyByMatrix(matrix, result);

    Matrix* identityMatrix = CreateIdentityMatrix();

    printf("%ld s\n", t1 - t0);
    printf("Error %f\n\n", GetMatricesDifference(identityMatrix, matrix));

    DestroyMatrix(matrix);
    DestroyMatrix(result);
    DestroyMatrix(identityMatrix);

    return 0;
}
