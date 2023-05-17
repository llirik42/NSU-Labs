package ru.nsu.kondrenko.model.work.supply;

import ru.nsu.kondrenko.model.listeners.SupplyListener;
import ru.nsu.kondrenko.model.products.Body;
import ru.nsu.kondrenko.model.storage.Storage;
import ru.nsu.kondrenko.model.work.WorkersCreator;

public class BodySuppliersCreator extends WorkersCreator<BodySupplier> {
    private final Storage<Body> storage;
    private final SupplyListener supplyListener;

    public BodySuppliersCreator(Storage<Body> storage, SupplyListener supplyListener, int workTime) {
        super(workTime);
        this.storage = storage;
        this.supplyListener = supplyListener;
    }

    @Override
    public BodySupplier createWorker() {
        return new BodySupplier(storage, supplyListener, getWorkTime());
    }
}
