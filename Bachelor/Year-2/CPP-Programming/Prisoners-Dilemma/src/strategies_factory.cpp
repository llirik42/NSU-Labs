#include "strategies_factory.h"
#include "strategies/all_strategies.h"

template<typename Type>
Strategy create(){
    return std::make_shared<Type>();
}

StrategiesFactory::StrategiesFactory(){
    _info["Cooperator"] = StrategyInfo({
        "Always cooperates",
        create<RawCooperatorStrategy>
    });

    _info["Defector"] = StrategyInfo({
        "Always defects",
        create<RawDefectorStrategy>
    });

    _info["Random"] = StrategyInfo({
        "Tries to make a random decision",
        create<RawRandomStrategy>
    });

    _info["Periodic"] = StrategyInfo({
        "Starts with cooperation and then alternates with period of 1",
        create<RawPeriodicStrategy>
    });

    _info["Kind"] = StrategyInfo({
        "Cooperates until somebody's betrayal",
        create<RawKindStrategy>
    });

    _info["Strategist"] = StrategyInfo({
        "If Cooperation > Betrayal in the whole game - cooperates, else - betrays. With no knowledge cooperates",
        create<RawStrategist>
    });
    _info["TitForTat"] = StrategyInfo({
        "If Cooperation > Betrayal in the previous round - cooperates, else - betrays. With no knowledge cooperates",
        create<RawTitForTatStrategy>
    });
}

Strategy StrategiesFactory::create_strategy(const std::string& name) const{
    Strategy result = _info.at(name).create();
    result->_strategy_name = name;

    return result;
}

std::map<std::string, std::string> StrategiesFactory::get_strategies_description() const{
    std::map<std::string, std::string> result;

    for (const auto& [strategy_name, strategy_info] : _info){
        result[strategy_name] = strategy_info.description;

    }
    return result;
}
