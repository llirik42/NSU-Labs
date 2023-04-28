#pragma once

#include "../abstract_strategy.h"

class RawPeriodicStrategy: public RawAbstractStrategy{
public:
    [[nodiscard]] Step act([[maybe_unused]] const Round& round) override;
private:
    unsigned int _steps_count = 0;
    unsigned int _period = 1;
    Step _current_step = COOPERATION_STEP;
};
