package ru.nsu.kondrenko.model.work.supply;

import ru.nsu.kondrenko.model.listeners.SupplyListener;
import ru.nsu.kondrenko.model.products.CarPart;
import ru.nsu.kondrenko.model.storage.Storage;
import ru.nsu.kondrenko.model.work.WorkersThreadPool;

public abstract class SuppliersThreadPool<P extends CarPart> extends WorkersThreadPool<Supplier<P>> {
    protected final Storage<P> storage;

    protected SupplyListener supplyListener;

    protected SuppliersThreadPool(Storage<P> storage) {
        this.storage = storage;
    }

    public void setSupplyListener(SupplyListener supplyListener) {
        this.supplyListener = supplyListener;

        for (final var it : workers) {
            it.setSupplyListener(supplyListener);
        }
    }

    @Override
    protected abstract void addWorkers(int count);
}
