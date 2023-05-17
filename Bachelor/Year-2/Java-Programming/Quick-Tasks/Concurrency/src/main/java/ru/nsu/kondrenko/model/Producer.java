package ru.nsu.kondrenko.model;

import java.util.concurrent.Callable;

public class Producer implements Callable<Record> {
    private final String fullName = "Producer-" + Id.createProducerId().getId();

    private final Storage<String> storage;

    private int producedCount = 0;

    public Producer(Storage<String> storage) {
        this.storage = storage;
    }

    @Override
    public Record call() {
        while (!Thread.interrupted()) {
            try {
                storage.place(produce());
                producedCount++;
            } catch (InterruptedException exception) {
                break;
            }
        }

        return new Record(fullName, producedCount);
    }

    private String produce() {
        return fullName + "-" + (producedCount + 1);
    }
}
