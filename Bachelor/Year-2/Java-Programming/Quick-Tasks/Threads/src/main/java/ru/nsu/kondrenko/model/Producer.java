package ru.nsu.kondrenko.model;

public class Producer extends Thread {
    private final Storage<String> storage;

    private int producedCount = 0;

    public Producer(Storage<String> storage) {
        this.storage = storage;
    }

    @Override
    public void run() {
        while (!Thread.interrupted()) {
            try {
                storage.place(produce());
                producedCount++;
            } catch (InterruptedException exception) {
                this.interrupt();
            }
        }

        synchronized (Producer.class) {
            System.out.println(getName() + " produced " + producedCount);
        }
    }

    private String produce() {
        return getName()  + "-" + (producedCount + 1);
    }
}
