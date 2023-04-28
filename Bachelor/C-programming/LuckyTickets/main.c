#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#define N 6

unsigned char getRemainder(size_t number){
    return number % 10;
}

bool isAppropriate(size_t number){
    size_t sumOfFirst = 0, sumOfLast = 0;

    for (unsigned char i = 0; i < N; i++){
        if (i < N / 2){
            sumOfLast += getRemainder(number);
        }
        else{
            sumOfFirst += getRemainder(number);
        }

        number /= 10;
    }

    return sumOfFirst == sumOfLast;
}

int main() {
    size_t result = 0;

    size_t stop = (size_t) pow(10, N);

    for (size_t number = 0; number < stop; number++){
        if (isAppropriate(number)){
            result++;
        }
    }

    printf("%d", result);

    return 0;
}
