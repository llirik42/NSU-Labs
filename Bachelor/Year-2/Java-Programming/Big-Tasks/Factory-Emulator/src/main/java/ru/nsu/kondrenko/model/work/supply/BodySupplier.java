package ru.nsu.kondrenko.model.work.supply;

import ru.nsu.kondrenko.model.listeners.SupplyListener;
import ru.nsu.kondrenko.model.products.Body;
import ru.nsu.kondrenko.model.storage.Storage;

public class BodySupplier extends Supplier<Body> {
    public BodySupplier(Storage<Body> storage, SupplyListener supplyListener, int workTime) {
        super(storage, supplyListener, workTime);
    }

    @Override
    public Body supply() {
        return new Body();
    }
}
