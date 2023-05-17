package ru.nsu.kondrenko.model.parsing.config;

import ru.nsu.kondrenko.model.parsing.limits.Limits;

public class DefaultConfigValidator implements ConfigValidator {
    private static boolean inRange(int value, int minValue, int maxValue) {
        return minValue <= value && value <= maxValue;
    }

    private static boolean validateNonCapacity(Config config, Limits limits) {
        return inRange(config.bodySuppliersCount(), limits.minBodySuppliersCount(), limits.maxBodySuppliersCount()) &&
                inRange(config.engineSuppliersCount(), limits.minEngineSuppliersCount(), limits.maxEngineSuppliersCount()) &&
                inRange(config.accessorySuppliersCount(), limits.minAccessorySuppliersCount(), limits.maxAccessorySuppliersCount()) &&
                inRange(config.workersCount(), limits.minWorkersCount(), limits.maxWorkersCount()) &&
                inRange(config.dealersCount(), limits.minDealersCount(), limits.maxDealersCount()) &&

                inRange(config.bodySupplyTimeMs(), limits.minBodySupplyTimeMs(), limits.maxBodySupplyTimeMs()) &&
                inRange(config.engineSupplyTimeMs(), limits.minEngineSupplyTimeMs(), limits.maxEngineSupplyTimeMs()) &&
                inRange(config.accessorySupplyTimeMs(), limits.minAccessorySupplyTimeMs(), limits.maxAccessorySupplyTimeMs()) &&
                inRange(config.carAssemblingTimeMs(), limits.minCarAssemblingTimeMs(), limits.maxCarAssemblingTimeMs()) &&
                inRange(config.carRequestTimeMs(), limits.minCarRequestTimeMs(), limits.maxCarRequestTimeMs());
    }

    private static boolean validateCapacity(Config config) {
        return config.bodyStorageCapacity() >= 0 &&
                config.engineStorageCapacity() >= 0 &&
                config.accessoryStorageCapacity() >= 0 &&
                config.carStorageCapacity() >= 0;
    }

    @Override
    public boolean validate(Config config, Limits limits) {
        return validateCapacity(config) && validateNonCapacity(config, limits);
    }
}
