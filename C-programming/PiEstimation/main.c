#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define START 1
#define STOP 10
#define STEP 1
#define PI 3.14159265358979323846

void estimate(int power){
    float curX, curY, estimatedPiValue;
    int inCircleCounter = 0, numberOfIterations = (int)pow(10, power);

    for(int index = 0; index < numberOfIterations; index++){
        curX = (float) rand() / RAND_MAX;
        curY = (float) rand() / RAND_MAX;

        if (curX * curX + curY * curY <= 1){
            inCircleCounter++;
        }
    }

    estimatedPiValue = 4 * (float) inCircleCounter / (float) numberOfIterations;
    printf("%d %f %f\n", power, estimatedPiValue, estimatedPiValue - PI);
}


int main() {
    srand(0);

    for(int power = START; power < STOP; power += STEP){
        estimate(power);
    }

    return 0;
}
