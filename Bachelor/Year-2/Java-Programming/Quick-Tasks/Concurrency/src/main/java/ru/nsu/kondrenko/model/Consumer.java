package ru.nsu.kondrenko.model;

import java.util.concurrent.Callable;

public class Consumer implements Callable<Record> {
    private final static int DELAY_MS = 100;

    private final String fullName = "Consumer-" + Id.createConsumerId().getId();

    private final Storage<String> storage;

    private final Listener listener;

    private int consumedCount = 0;

    public Consumer(Storage<String> storage, Listener listener) {
        this.storage = storage;
        this.listener = listener;
    }

    @Override
    public Record call() {
        while (!Thread.interrupted()) {
            try {
                String product = storage.take();

                if (product == null) {
                    break;
                }

                consume(product);
                consumedCount++;
                Thread.sleep(DELAY_MS);
            } catch (InterruptedException exception) {
                break;
            }
        }

        return new Record(fullName, consumedCount);
    }

    private void consume(String product) throws InterruptedException {
        if (listener != null) {
            listener.notifyConsumption(fullName, product);
        }
    }
}
