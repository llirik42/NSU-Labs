#include <stdio.h>
#include <malloc.h>

#define START 1
#define STOP 32

#define L1 49152 // 48 KB
#define L2 1310720 // 1.25 MB
#define L3 8388608 // 8 MB

#define EPOCH 4
#define K 4
#define CACHES_COUNT 3
#define SIZE_OF_INT 4

#define MIN(A, B) ((A) > (B) ? (B) : (A))


const char* PATHS[] = {"../l1", "../l2", "../l3"};
const unsigned int OFFSETS[] = {L1 / SIZE_OF_INT, L2 / SIZE_OF_INT, L3 / SIZE_OF_INT};

union Ticks{
    unsigned long long t64;
    struct s32 { long th, tl; } t32;
};

void init_array(unsigned int* array, unsigned int fragments_count, unsigned int array_size, unsigned int offset){
    for (unsigned int i = 0; i < array_size - offset * (fragments_count - 1); i++){
        for (unsigned int j = 0; j < fragments_count - 1; j++){
            array[i + j * offset] = i + (j + 1) * offset;
        }

        array[i + offset * (fragments_count - 1)] = i + 1;
    }
    array[array_size - 1] = 0;
}
unsigned long long test(const unsigned int* array, unsigned int array_size){
    unsigned int i, k;

    union Ticks start, end;

    asm volatile("rdtsc\n":"=a"(start.t32.th),"=d"(start.t32.tl));

    for (k = 0, i = 0; i < array_size * K; i++){
        k = array[k];
    }

    asm volatile("rdtsc\n":"=a"(end.t32.th),"=d"(end.t32.tl));

    if (k == 1){
        printf("0_0\n");
    }

    return (end.t64 - start.t64) / array_size / K;
}

int main(){
    for (unsigned int i = 0; i < CACHES_COUNT; i++){
        FILE* file = fopen(PATHS[i], "w");

        const unsigned int offset = OFFSETS[i];

        const unsigned int array_size = STOP * offset;

        unsigned int* array = malloc(array_size * SIZE_OF_INT);

        for (unsigned int fragments_count = START; fragments_count <= STOP; fragments_count++){
            init_array(array, fragments_count, array_size, offset);

            unsigned long long min_time = ULLONG_MAX;

            for (unsigned int j = 0; j < EPOCH; j++){
                const unsigned long long current_time = test(array, array_size);

                min_time = MIN(min_time, current_time);
            }

            fprintf(file, "%d %llu\n", fragments_count, min_time);
        }

        free(array);
        fclose(file);
    }

    return 0;
}
