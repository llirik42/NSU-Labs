package ru.nsu.kondrenko.model.parsing.limits;

public record Limits(
        int minBodySuppliersCount,
        int maxBodySuppliersCount,
        int minBodySupplyTimeMs,
        int maxBodySupplyTimeMs,

        int minEngineSuppliersCount,
        int maxEngineSuppliersCount,
        int minEngineSupplyTimeMs,
        int maxEngineSupplyTimeMs,

        int minAccessorySuppliersCount,
        int maxAccessorySuppliersCount,
        int minAccessorySupplyTimeMs,
        int maxAccessorySupplyTimeMs,

        int minWorkersCount,
        int maxWorkersCount,
        int minCarAssemblingTimeMs,
        int maxCarAssemblingTimeMs,

        int minDealersCount,
        int maxDealersCount,
        int minCarRequestTimeMs,
        int maxCarRequestTimeMs) {
}
