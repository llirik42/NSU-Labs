#include <algorithm>
#include "kind_strategy.h"

bool was_betrayed_in_round(const Round& round){
    return std::any_of(round.begin(), round.end(), [](const auto& pair)
    {
        return pair.second == DEFECTION_STEP;
    });
}

bool RawKindStrategy::was_betrayed_in_previous_games(const History& history) const{
    const std::string& strategy_name = _strategy_name;

    return std::any_of(history.begin(), history.end(), [&strategy_name](const auto& round)
    {
        return round.count(strategy_name) && was_betrayed_in_round(round);
    });
}

Step RawKindStrategy::act(const Round& round){
    _was_betrayed = _was_betrayed || was_betrayed_in_round(round);

    return _was_betrayed ? DEFECTION_STEP : COOPERATION_STEP;
}

void RawKindStrategy::apply_previous_games_experience(const History& history){
    _was_betrayed = was_betrayed_in_previous_games(history);
}
