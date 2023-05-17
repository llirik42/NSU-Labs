package ru.nsu.kondrenko.model.parsing.config;

public record Config(
        int bodyStorageCapacity,
        int engineStorageCapacity,
        int accessoryStorageCapacity,
        int carStorageCapacity,

        int bodySuppliersCount,
        int engineSuppliersCount,
        int accessorySuppliersCount,
        int workersCount,
        int dealersCount,

        int bodySupplyTimeMs,
        int engineSupplyTimeMs,
        int accessorySupplyTimeMs,
        int carAssemblingTimeMs,
        int carRequestTimeMs,

        boolean logging) {
}
