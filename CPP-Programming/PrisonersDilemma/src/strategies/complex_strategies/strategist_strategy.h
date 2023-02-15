#pragma once

#include "../abstract_strategy.h"

class RawStrategist: public RawAbstractStrategy{
public:
    [[nodiscard]] Step act(const Round& round) override;
    void apply_previous_games_experience(const History& history) override;
private:
    unsigned int _cooperation_count = 0;
    unsigned int _defection_count = 0;

    void apply_choices(const Round& round);
};
