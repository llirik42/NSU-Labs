package ru.nsu.kondrenko.model.work.supply;

import ru.nsu.kondrenko.model.listeners.SupplyListener;
import ru.nsu.kondrenko.model.products.Accessory;
import ru.nsu.kondrenko.model.storage.Storage;

public class AccessorySupplier extends Supplier<Accessory> {
    public AccessorySupplier(Storage<Accessory> storage, SupplyListener supplyListener, int workTime) {
        super(storage, supplyListener, workTime);
    }

    @Override
    protected Accessory supply() {
        return new Accessory();
    }
}
