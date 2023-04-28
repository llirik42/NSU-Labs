#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define PI 3.14159265358979323846

double calculatePI(const unsigned long long N){
    unsigned long long circlePointsCounter = 0;

    for (unsigned long long i = 0; i < N; i++){
        const long curX = (long) rand();
        const long curY = (long) rand();

        circlePointsCounter += 4 * (curX * curX + curY * curY <= (long long) RAND_MAX * (long long) RAND_MAX);
    }

    return (double) (circlePointsCounter) / (double) N;
}

int main(int arc, char* argv[]) {

    if (arc < 2){
        printf("Requires N\n");
        return 0;
    }

    srand(0);

    const unsigned long long N = strtoull(argv[1], NULL, 10);

    time_t t0 = clock();
    const double result = calculatePI(N);
    time_t t1 = clock();

    printf("Calculated value of PI: %.10lf\n", result);
    printf("Relative error: %f %%\n", 100 * fabs(1 - result / PI));
    printf("Time: %.0f ms\n", difftime(t1, t0) / 1000);

    return 0;
}
