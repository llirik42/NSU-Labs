package ru.nsu.kondrenko.model.products;

import java.util.concurrent.atomic.AtomicInteger;

class Id {
    private static final AtomicInteger bodyId = new AtomicInteger(0);
    private static final AtomicInteger engineId = new AtomicInteger(0);
    private static final AtomicInteger accessoryId = new AtomicInteger(0);
    private static final AtomicInteger carId = new AtomicInteger(0);

    private final int myId;

    private Id(int id) {
        myId = id;
    }

    public static Id createBodyId() {
        return new Id(bodyId.getAndIncrement());
    }

    public static Id createEngineId() {
        return new Id(engineId.getAndIncrement());
    }

    public static Id createAccessoryId() {
        return new Id(accessoryId.getAndIncrement());
    }

    public static Id createCarId() {
        return new Id(carId.getAndIncrement());
    }

    public int getIntegerId() {
        return myId;
    }
}
