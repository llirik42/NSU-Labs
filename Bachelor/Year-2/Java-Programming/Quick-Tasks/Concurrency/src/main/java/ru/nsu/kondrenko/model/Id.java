package ru.nsu.kondrenko.model;

import java.util.concurrent.atomic.AtomicInteger;

public class Id {
    private static final AtomicInteger producersId = new AtomicInteger(0);
    private static final AtomicInteger consumersId = new AtomicInteger(0);
    private static final AtomicInteger storageId = new AtomicInteger(0);

    private final int myId;

    private Id(int id) {
        myId = id;
    }

    public static Id createProducerId() {
        return new Id(producersId.getAndIncrement());
    }

    public static Id createConsumerId() {
        return new Id(consumersId.getAndIncrement());
    }

    public static Id createStorageId() {
        return new Id(storageId.getAndIncrement());
    }

    public int getId() {
        return myId;
    }
}
