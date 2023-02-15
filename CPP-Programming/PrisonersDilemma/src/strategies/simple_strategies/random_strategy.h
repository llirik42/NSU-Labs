#pragma once

#include "../abstract_strategy.h"

class RawRandomStrategy: public RawAbstractStrategy{
public:
    [[nodiscard]] Step act([[maybe_unused]] const Round& round) override;
};
