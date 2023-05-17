package ru.nsu.kondrenko.model.work.supply;

import ru.nsu.kondrenko.model.listeners.SupplyListener;
import ru.nsu.kondrenko.model.products.Engine;
import ru.nsu.kondrenko.model.storage.Storage;
import ru.nsu.kondrenko.model.work.WorkersCreator;

public class EngineSuppliersCreator extends WorkersCreator<EngineSupplier> {
    private final Storage<Engine> storage;
    private final SupplyListener supplyListener;

    public EngineSuppliersCreator(Storage<Engine> storage, SupplyListener supplyListener, int workTime) {
        super(workTime);
        this.storage = storage;
        this.supplyListener = supplyListener;
    }

    @Override
    public EngineSupplier createWorker() {
        return new EngineSupplier(storage, supplyListener, getWorkTime());
    }
}
