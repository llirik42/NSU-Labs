package ru.nsu.kondrenko.model.work.supply;

import ru.nsu.kondrenko.model.products.Body;
import ru.nsu.kondrenko.model.storage.Storage;

public class BodySuppliersPool extends SuppliersThreadPool<Body> {
    public BodySuppliersPool(Storage<Body> storage, int suppliersCount, int supplyTime) {
        super(storage);
        workTime = supplyTime;
        addWorkers(suppliersCount);
    }

    @Override
    protected void addWorkers(int count) {
        for (int i = 0; i < count; i++) {
            final BodySupplier supplier = new BodySupplier(storage, workTime);
            supplier.setSupplyListener(supplyListener);
            workers.add(supplier);

            if (started) {
                supplier.start();
            }
        }
    }
}
