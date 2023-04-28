#include "tit_for_tat_strategy.h"

Step get_step_from_other_players_statistic(unsigned int cooperation_count, unsigned int defection_count){
    return cooperation_count > defection_count ? COOPERATION_STEP : DEFECTION_STEP;
}

Round RawTitForTatStrategy::find_last_round_with_tit_for_tat(const History& history) const{
    for (size_t i = history.size(); i > 0; i--){
        const Round& current_round = history[i - 1];

        if (current_round.count(_strategy_name)){
            return current_round;
        }
    }

    return {};
}

Step RawTitForTatStrategy::act(const Round& round){
    if (round.empty()){
        return _reserved_step;
    }

    unsigned int cooperation_count = 0;
    unsigned int defection_count = 0;

    for (const auto& [strategy_name, step] : round){
        if (strategy_name == _strategy_name){
            continue;
        }

        cooperation_count += (step == COOPERATION_STEP);
        defection_count += (step == DEFECTION_STEP);
    }

    return get_step_from_other_players_statistic(cooperation_count, defection_count);
}

void RawTitForTatStrategy::apply_previous_games_experience(const History& history){
    Round last_round = find_last_round_with_tit_for_tat(history);

    _reserved_step = act(last_round);
}
