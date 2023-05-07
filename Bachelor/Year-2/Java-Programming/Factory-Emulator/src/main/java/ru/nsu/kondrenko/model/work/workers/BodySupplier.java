package ru.nsu.kondrenko.model.work.workers;

import ru.nsu.kondrenko.model.products.Body;
import ru.nsu.kondrenko.model.storage.Storage;

public class BodySupplier extends Supplier<Body> {
    public BodySupplier(Storage<Body> storage, int supplyTime) {
        super(supplyTime, storage);
    }

    @Override
    protected Body supplyCarPart() {
        return new Body();
    }
}
