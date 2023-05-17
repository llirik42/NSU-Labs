package ru.nsu.kondrenko.model.storage;

import ru.nsu.kondrenko.model.listeners.StorageListener;
import ru.nsu.kondrenko.model.products.Product;

import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.BlockingQueue;

public class ProductStorage<P extends Product> implements Storage<P> {
    private final BlockingQueue<P> storage;
    private final StorageListener storageListener;
    private final int maxCapacity;

    public ProductStorage(int maxCapacity, StorageListener storageListener) {
        storage = new ArrayBlockingQueue<>(maxCapacity);
        this.storageListener = storageListener;
        this.maxCapacity = maxCapacity;
    }

    @Override
    public void put(P product) throws InterruptedException {
        storage.put(product);
        storageListener.notifyAboutProductPut(product);
    }

    @Override
    public P take() throws InterruptedException {
        final P ret = storage.take();
        storageListener.notifyAboutProductTaken(ret);
        return ret;
    }

    @Override
    public int getMaxCapacity() {
        return maxCapacity;
    }

    @Override
    public int getSize() {
        return storage.size();
    }

    @Override
    public int getRemainingCapacity() {
        return storage.remainingCapacity();
    }
}
