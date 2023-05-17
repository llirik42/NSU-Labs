package ru.nsu.kondrenko.model.parsing.limits;

public class DefaultLimitsValidator implements LimitsValidator {
    private static boolean isEveryMinNonGreaterThanMax(Limits limits) {
        return limits.maxBodySuppliersCount() >= limits.minBodySuppliersCount() &&
                limits.maxBodySupplyTimeMs() >= limits.minBodySupplyTimeMs() &&

                limits.maxEngineSuppliersCount() >= limits.minEngineSuppliersCount() &&
                limits.maxEngineSupplyTimeMs() >= limits.minEngineSupplyTimeMs() &&

                limits.maxAccessorySuppliersCount() >= limits.minAccessorySuppliersCount() &&
                limits.maxAccessorySupplyTimeMs() >= limits.minAccessorySupplyTimeMs() &&

                limits.maxWorkersCount() >= limits.minWorkersCount() &&
                limits.maxCarAssemblingTimeMs() >= limits.minCarAssemblingTimeMs() &&

                limits.maxDealersCount() >= limits.minDealersCount() &&
                limits.maxCarRequestTimeMs() >= limits.minCarRequestTimeMs();
    }

    private static boolean isEveryValueNonNegative(Limits limits) {
        return limits.minBodySuppliersCount() >= 0 &&
                limits.maxBodySuppliersCount() >= 0 &&
                limits.minBodySupplyTimeMs() >= 0 &&
                limits.maxBodySupplyTimeMs() >= 0 &&

                limits.minEngineSuppliersCount() >= 0 &&
                limits.maxEngineSuppliersCount() >= 0 &&
                limits.minEngineSupplyTimeMs() >= 0 &&
                limits.maxEngineSupplyTimeMs() >= 0 &&

                limits.minAccessorySuppliersCount() >= 0 &&
                limits.maxAccessorySuppliersCount() >= 0 &&
                limits.minAccessorySupplyTimeMs() >= 0 &&
                limits.maxAccessorySupplyTimeMs() >= 0 &&

                limits.minWorkersCount() >= 0 &&
                limits.maxWorkersCount() >= 0 &&
                limits.minCarAssemblingTimeMs() >= 0 &&
                limits.maxCarAssemblingTimeMs() >= 0 &&

                limits.minDealersCount() >= 0 &&
                limits.maxDealersCount() >= 0 &&
                limits.minCarRequestTimeMs() >= 0 &&
                limits.maxCarRequestTimeMs() >= 0;
    }

    @Override
    public boolean validate(Limits limits) {
        return isEveryValueNonNegative(limits) && isEveryMinNonGreaterThanMax(limits);
    }
}
