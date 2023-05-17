package ru.nsu.kondrenko.model;

import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.TimeUnit;

public class Storage <T> {
    private static final TimeUnit TIME_UNIT = TimeUnit.MILLISECONDS;
    private static final int TIMEOUT = 1000;

    private final String fullName = "Storage-" + Id.createStorageId().getId();

    private final BlockingQueue<T> storage;

    private int placedCount = 0;
    private int takenCount = 0;

    private boolean termination = false;

    public Storage(int maxCapacity) {
        this.storage = new ArrayBlockingQueue<>(maxCapacity);
    }

    public void startTermination() {
        termination = true;
    }

    public void place(T product) throws InterruptedException {
        storage.put(product);
        placedCount++;
    }

    public T take() throws InterruptedException {
        T product = termination
                ? storage.poll(TIMEOUT, TIME_UNIT)
                : storage.take();

        if (product == null) {
            synchronized (this) {
                notifyAll();
            }
        } else {
            takenCount++;
        }

        return product;
    }

    public StorageRecord getRecord() {
        return new StorageRecord(fullName, placedCount, takenCount);
    }

    public synchronized void waitUntilEmpty() throws InterruptedException {
        while (!storage.isEmpty()) {
            wait();
        }
    }
}
