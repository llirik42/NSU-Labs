package ru.nsu.kondrenko.model;

public class Consumer extends Thread {
    private final Storage<String> storage;

    private int consumedCount = 0;

    public Consumer(Storage<String> storage) {
        this.storage = storage;
    }

    @Override
    public void run() {
        while (!Thread.interrupted()) {
            try {
                consume(storage.take());
                consumedCount++;
                Thread.sleep(100);
            } catch (InterruptedException exception) {
                this.interrupt();
            }
        }

        synchronized (Consumer.class) {
            System.out.println(getName() + " consumed " + consumedCount);
        }
    }

    private void consume(String product) throws InterruptedException {
        System.out.println(getName() + " consumes " + product);
    }
}
