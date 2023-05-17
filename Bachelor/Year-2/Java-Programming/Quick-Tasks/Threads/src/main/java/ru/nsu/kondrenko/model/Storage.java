package ru.nsu.kondrenko.model;

import java.util.LinkedList;
import java.util.Queue;

public class Storage <T> {
    private final int maxCapacity;

    private final Queue<T> storage;

    private int placedCount = 0;
    private int takenCount = 0;

    public Storage(int maxCapacity) {
        this.maxCapacity = maxCapacity;
        this.storage = new LinkedList<>();
    }

    public synchronized void place(T something) throws InterruptedException {
        while (isFull()) {
            wait();
        }

        storage.add(something);
        placedCount++;
        notifyAll();
    }

    public synchronized T take() throws InterruptedException {
        while (isEmpty()) {
            wait();
        }

        T product = storage.poll();
        takenCount++;
        notifyAll();

        return product;
    }

    public synchronized boolean isEmpty() {
        return storage.isEmpty();
    }

    public int getPlacedCount() {
        return placedCount;
    }

    public int getTakenCount() {
        return takenCount;
    }

    private boolean isFull() {
        return storage.size() == maxCapacity;
    }
}
