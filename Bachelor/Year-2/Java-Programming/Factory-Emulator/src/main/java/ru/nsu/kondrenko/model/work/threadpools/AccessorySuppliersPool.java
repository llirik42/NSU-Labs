package ru.nsu.kondrenko.model.work.threadpools;

import ru.nsu.kondrenko.model.products.Accessory;
import ru.nsu.kondrenko.model.storage.Storage;
import ru.nsu.kondrenko.model.work.workers.AccessorySupplier;

public class AccessorySuppliersPool extends SuppliersThreadPool<Accessory> {
    public AccessorySuppliersPool(Storage<Accessory> storage, int suppliersCount, int supplyTime) {
        super(storage);
        workTime = supplyTime;
        addWorkers(suppliersCount);
    }

    @Override
    protected void addWorkers(int count) {
        for (int i = 0; i < count; i++) {
            final AccessorySupplier supplier = new AccessorySupplier(storage, workTime);
            supplier.setSupplyListener(supplyListener);
            workers.add(supplier);

            if (started) {
                supplier.start();
            }
        }
    }
}
