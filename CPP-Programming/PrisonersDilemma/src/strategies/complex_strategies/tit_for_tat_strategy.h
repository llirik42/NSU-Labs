#pragma once

#include "../abstract_strategy.h"

class RawTitForTatStrategy: public RawAbstractStrategy{
public:
    [[nodiscard]] Step act(const Round& round) override;
    void apply_previous_games_experience(const History& history) override;
private:
    Step _reserved_step = COOPERATION_STEP; // when don't have any knowledge

    [[nodiscard]] Round find_last_round_with_tit_for_tat(const History& history) const;
};
