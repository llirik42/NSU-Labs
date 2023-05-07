package ru.nsu.kondrenko.model.storage;

public interface Storage<P> {
    void put(P product) throws InterruptedException;

    P take() throws InterruptedException;
}
