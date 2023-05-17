package ru.nsu.kondrenko.model.work.supply;

import ru.nsu.kondrenko.model.listeners.SupplyListener;
import ru.nsu.kondrenko.model.products.Accessory;
import ru.nsu.kondrenko.model.storage.Storage;
import ru.nsu.kondrenko.model.work.WorkersCreator;

public class AccessorySuppliersCreator extends WorkersCreator<AccessorySupplier> {
    private final Storage<Accessory> storage;
    private final SupplyListener supplyListener;

    public AccessorySuppliersCreator(Storage<Accessory> storage, SupplyListener supplyListener, int workTime) {
        super(workTime);
        this.storage = storage;
        this.supplyListener = supplyListener;
    }

    @Override
    public AccessorySupplier createWorker() {
        return new AccessorySupplier(storage, supplyListener, getWorkTime());
    }
}
