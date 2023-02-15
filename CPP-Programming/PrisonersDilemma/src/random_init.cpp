#include <random>
#include "random_init.h"

void init_random(){
    srand(static_cast<unsigned int>(time(nullptr)));
}
