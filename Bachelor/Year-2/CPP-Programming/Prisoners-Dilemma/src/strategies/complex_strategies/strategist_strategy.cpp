#include <random>
#include "strategist_strategy.h"

void RawStrategist::apply_choices(const Round& round){
    for (const auto& [strategy_name, step]: round){
        if (strategy_name == _strategy_name){
            continue;
        }

        _cooperation_count += (step == COOPERATION_STEP);
        _defection_count += (step == DEFECTION_STEP);
    }
}

Step RawStrategist::act(const Round& round){
    apply_choices(round);

    if (_cooperation_count == 0 && _cooperation_count == _defection_count){
        return COOPERATION_STEP;
    }

    return _cooperation_count > _defection_count ? COOPERATION_STEP : DEFECTION_STEP;
}

void RawStrategist::apply_previous_games_experience(const History& history){
    for (const auto& round : history){
        if (round.count(_strategy_name)){
            apply_choices(round);
        }
    }
}
