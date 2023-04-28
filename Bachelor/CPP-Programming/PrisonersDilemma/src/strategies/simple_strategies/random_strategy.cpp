#include <random>
#include "random_strategy.h"

Step RawRandomStrategy::act([[maybe_unused]] const Round& round){
    return (rand() % 2) ? COOPERATION_STEP : DEFECTION_STEP;
}
