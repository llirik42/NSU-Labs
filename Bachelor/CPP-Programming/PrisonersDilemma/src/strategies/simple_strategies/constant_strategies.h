#pragma once

#include "../abstract_strategy.h"

class RawCooperatorStrategy:public RawAbstractStrategy{
public:
    [[nodiscard]] Step act([[maybe_unused]] const Round& round) override;
};

class RawDefectorStrategy:public RawAbstractStrategy{
public:
    [[nodiscard]] Step act([[maybe_unused]] const Round& round) override;
};
