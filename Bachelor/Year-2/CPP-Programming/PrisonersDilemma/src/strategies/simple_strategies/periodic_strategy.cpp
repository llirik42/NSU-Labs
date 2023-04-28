#include "periodic_strategy.h"

Step RawPeriodicStrategy::act([[maybe_unused]] const Round& round){
    const Step result = _current_step;

    _steps_count++;

    if (_steps_count == _period){
        _current_step = _current_step == COOPERATION_STEP ? DEFECTION_STEP : COOPERATION_STEP;
        _steps_count = 0;
    }

    return result;
}
