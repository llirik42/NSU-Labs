#pragma once

#include "../abstract_strategy.h"

class RawKindStrategy: public RawAbstractStrategy{
public:
    [[nodiscard]] Step act(const Round& round) override;
    void apply_previous_games_experience(const History& history) override;
private:
    bool _was_betrayed = false;

    [[nodiscard]] bool was_betrayed_in_previous_games(const History& history) const;
};
