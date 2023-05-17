package ru.nsu.kondrenko.model;

import java.util.concurrent.Semaphore;

public class RestRoom {
    private final static int MIN_TIME_MS = 1000;
    private final static int MAX_TIME_MS = 2000;

    private final Listener listener;

    private final Semaphore semaphore;

    public RestRoom(Listener listener, int k) {
        this.listener = listener;
        this.semaphore = new Semaphore(k);
    }

    public void enterAndRest() throws InterruptedException {
        final int time_ms = Utils.generateRandomNumber(MIN_TIME_MS, MAX_TIME_MS);

        semaphore.acquire();
        listener.notifyThreadEnter(Thread.currentThread().getName(), time_ms);
        Thread.sleep(time_ms);
        listener.notifyThreadExit(Thread.currentThread().getName());
        semaphore.release();
    }
}
