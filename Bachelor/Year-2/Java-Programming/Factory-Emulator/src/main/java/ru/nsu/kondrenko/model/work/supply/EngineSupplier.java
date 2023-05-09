package ru.nsu.kondrenko.model.work.supply;

import ru.nsu.kondrenko.model.products.Engine;
import ru.nsu.kondrenko.model.storage.Storage;

public class EngineSupplier extends Supplier<Engine> {
    public EngineSupplier(Storage<Engine> storage, int supplyTime) {
        super(supplyTime, storage);
    }

    @Override
    protected Engine supplyCarPart() {
        return new Engine();
    }
}
