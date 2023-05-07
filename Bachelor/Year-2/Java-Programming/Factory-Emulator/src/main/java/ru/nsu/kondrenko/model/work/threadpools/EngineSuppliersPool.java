package ru.nsu.kondrenko.model.work.threadpools;

import ru.nsu.kondrenko.model.products.Engine;
import ru.nsu.kondrenko.model.storage.Storage;
import ru.nsu.kondrenko.model.work.workers.EngineSupplier;

public class EngineSuppliersPool extends SuppliersThreadPool<Engine> {
    public EngineSuppliersPool(Storage<Engine> storage, int suppliersCount, int supplyTime) {
        super(storage);
        workTime = supplyTime;
        addWorkers(suppliersCount);
    }

    @Override
    protected void addWorkers(int count) {
        for (int i = 0; i < count; i++) {
            final EngineSupplier supplier = new EngineSupplier(storage, workTime);
            supplier.setSupplyListener(supplyListener);
            workers.add(supplier);

            if (started) {
                supplier.start();
            }
        }
    }
}
