package ru.nsu.kondrenko.model.work.supply;

import ru.nsu.kondrenko.model.products.Accessory;
import ru.nsu.kondrenko.model.storage.Storage;

public class AccessorySupplier extends Supplier<Accessory> {
    public AccessorySupplier(Storage<Accessory> storage, int supplyTime) {
        super(supplyTime, storage);
    }

    @Override
    protected Accessory supplyCarPart() {
        return new Accessory();
    }
}
