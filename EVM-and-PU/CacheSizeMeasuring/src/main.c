#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <limits.h>

#define MULTIPLIER 1.1
#define MIN_N (256) // 1 KByte
#define MAX_N (8388608) // 32 MBytes

#define K 1
#define EPOCH 10

#define MATRICES_SIZE 2048

union Ticks{
    unsigned long long t64;
    struct s32 { long th, tl; } t32;
};

float dot(const float* a, const float* b){
    float result = 0;

    for (unsigned int k = 0; k < MATRICES_SIZE; k++){
        result += a[k] * b[k];
    }
    return result;
}
void MultiplyMatrices(const float* matrix1, const float* matrix2_transposed, float* result){
    for (unsigned int i = 0; i < MATRICES_SIZE; i++){
        const float* row1 = matrix1 + i * MATRICES_SIZE;

        for (unsigned int j = 0; j < MATRICES_SIZE; j++){
            const float* row2 = matrix2_transposed + j * MATRICES_SIZE;

            result[i * MATRICES_SIZE + j] = dot(row1, row2);
        }
    }
}
void MatricesMultiplication(){
    float* matrix_1 = malloc(sizeof(float) * MATRICES_SIZE * MATRICES_SIZE);
    float* matrix_2 = malloc(sizeof(float) * MATRICES_SIZE * MATRICES_SIZE);
    float* matrix_3 = malloc(sizeof(float) * MATRICES_SIZE * MATRICES_SIZE);

    MultiplyMatrices(matrix_1, matrix_2, matrix_3);

    if (matrix_3[0] == 1){
        printf("OMG\n");
    }

    free(matrix_1);
    free(matrix_2);
    free(matrix_3);

}

void swap(unsigned int* a, unsigned int* b){
    unsigned int tmp = *a;
    *a = *b;
    *b = tmp;
}

void shuffle(unsigned int* buffer){
    for(unsigned int i = 0; i < MAX_N - 2; ++i){
        unsigned int swap_index = i + rand() % (MAX_N - i);
        if(swap_index == i){
            swap_index++;
        }

        swap(buffer + i, buffer + swap_index);
    }
}
void fillStraight(unsigned int* buffer){
    for (unsigned int i = 0; i < MAX_N; i++){
        buffer[i] = i;
    }
}
void fillStraightPlusOne(unsigned int* buffer){
    for (unsigned int i = 0; i < MAX_N; i++){
        buffer[i] = i + 1;
    }
}
void fillReverse(unsigned int* buffer){
    buffer[0] = MAX_N - 1;

    for (unsigned int i = 1; i < MAX_N; i++){
        buffer[i] = i - 1;
    }
}
void fillRandom(unsigned int* buffer){
    fillStraight(buffer);
    shuffle(buffer);
}
void fillUINTMax(unsigned int* buffer){
    for (unsigned int i = 0; i < MAX_N; i++){
        buffer[i] = UINT_MAX;
    }
}

unsigned int* createBuffer(){
    return malloc(sizeof(unsigned int) * MAX_N);
}
unsigned int* CreateStraight(){
    unsigned int* buffer = createBuffer();
    fillStraightPlusOne(buffer);
    return buffer;
}
unsigned int* CreateReverse(){
    unsigned int* buffer = createBuffer();
    fillReverse(buffer);
    return buffer;
}
unsigned int* CreateRandom(){
    unsigned int* buffer = createBuffer();
    fillStraightPlusOne(buffer);
    fillRandom(buffer);
    return buffer;
}
unsigned int* CreateUINTMax(){
    unsigned int* buffer = createBuffer();
    fillUINTMax(buffer);
    return buffer;
}

unsigned int min(unsigned int a, unsigned int b){
    return a > b ? b : a;
}

void TestBuffer(const unsigned int* buffer, unsigned int* minTacts){
    for (unsigned int n = MIN_N; n <= MAX_N; n = (unsigned int)(n * MULTIPLIER)){
        union Ticks start, end;

        unsigned int k, i;

        for (k = 0, i = 0; i < n * K; i++){
            k = buffer[k];
        }
        if (k == 0){
            printf("OMG\n");
        }

        asm("rdtsc\n":"=a"(start.t32.th),"=d"(start.t32.tl));
        for (k = 0, i = 0; i < n * K; i++){
            k = buffer[k];
        }
        asm("rdtsc\n":"=a"(end.t32.th),"=d"(end.t32.tl));

        minTacts[n] = min(minTacts[n], (end.t64 - start.t64) / n / K);

        if (k == 0){
            printf("OMG\n");
        }
    }
}

void printMinTacts(FILE* file, unsigned int* minTacts){
    for (unsigned int i = 0; i < MAX_N; i++){
        if (minTacts[i] != UINT_MAX){
            fprintf(file, "%u %u\n", i, minTacts[i]);
        }
    }
}

int main(){
    srand(time(NULL));

    unsigned int* minTacts1 = CreateUINTMax();
    unsigned int* minTacts2 = CreateUINTMax();
    unsigned int* minTacts3 = CreateUINTMax();

    FILE* file1 = fopen("../1.txt", "w");
    FILE* file2 = fopen("../2.txt", "w");
    FILE* file3 = fopen("../3.txt", "w");

    unsigned int* straightBuffer = CreateStraight();
    unsigned int* reverseBuffer = CreateReverse();
    unsigned int* randomBuffer = CreateRandom();

    MatricesMultiplication();

    for (unsigned int i = 0; i < EPOCH; i++){
        TestBuffer(straightBuffer, minTacts1);
        TestBuffer(reverseBuffer, minTacts2);
        TestBuffer(randomBuffer, minTacts3);
    }

    printMinTacts(file1, minTacts1);
    printMinTacts(file2, minTacts2);
    printMinTacts(file3, minTacts3);

    free(straightBuffer);
    free(reverseBuffer);
    free(randomBuffer);

    free(minTacts1);
    free(minTacts2);
    free(minTacts3);

    fclose(file1);
    fclose(file2);
    fclose(file3);

    return 0;
}
