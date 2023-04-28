#include "constant_strategies.h"

Step RawCooperatorStrategy::act([[maybe_unused]] const Round& round){
    return COOPERATION_STEP;
}

Step RawDefectorStrategy::act([[maybe_unused]] const Round& round){
    return DEFECTION_STEP;
}
