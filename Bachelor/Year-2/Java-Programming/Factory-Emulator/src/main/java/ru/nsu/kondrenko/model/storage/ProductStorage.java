package ru.nsu.kondrenko.model.storage;

import ru.nsu.kondrenko.model.listeners.StorageListener;
import ru.nsu.kondrenko.model.products.Product;

import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.BlockingQueue;

public class ProductStorage<P extends Product> implements Storage<P> {
    private final BlockingQueue<P> storage;

    private StorageListener storageListener;

    public ProductStorage(int maxCapacity) {
        storage = new ArrayBlockingQueue<>(maxCapacity);
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

    public void setStorageListener(StorageListener storageListener) {
        this.storageListener = storageListener;
    }
}
