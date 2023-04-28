#pragma once

#include <memory>
#include <string>
#include "../utils.h"

// Description of all strategies can be found in the StrategiesFactory()

class RawAbstractStrategy;

using Strategy = std::shared_ptr<RawAbstractStrategy>;

class RawAbstractStrategy{
    friend class StrategiesFactory;
public:
    virtual ~RawAbstractStrategy()=default;
    virtual Step act([[maybe_unused]] const Round& round) {return COOPERATION_STEP;}
    virtual void apply_previous_games_experience([[maybe_unused]] const History& history) {}
    const std::string& get_strategy_name() {return _strategy_name;}
protected:
    RawAbstractStrategy()=default;
    std::string _strategy_name;
};
