#pragma once

#include <vector>
#include "utils.h"
#include "strategies/abstract_strategy.h"
#include "matrix.h"
#include "viewer.h"
#include "storage.h"

using StrategiesTriplet = Triplet<Strategy>;
using StrategiesVector = std::vector<Strategy>;

class Game{
public:
    Game(StrategiesVector& strategies, const StrategiesNames& strategies_names, const Matrix& matrix,
         unsigned int steps_count, GameMode mode, Storage& storage);

    void start();
private:
    GameViewer _viewer;
    const StrategiesVector& _strategies;
    const Matrix& _matrix;
    unsigned int _steps_count;
    GameMode _mode;
    Storage& _storage;

    [[nodiscard]] StrategiesTriplet extract_strategies_triplet(const StrategiesIndexes& indexes) const;

    [[nodiscard]] Score competition(const StrategiesTriplet& strategies_triplet) const;

    Score tournament();
};
