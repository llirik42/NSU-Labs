package ru.nsu.kondrenko.model.work.workers;

public abstract class Worker extends Thread {
    protected int workTime;

    protected boolean softlyInterrupted = false;

    public Worker(int workTime) {
        this.workTime = workTime;
    }

    public void setWorkTime(int workTime) {
        this.workTime = workTime;
    }

    public void softlyInterrupt() {
        softlyInterrupted = true;
    }
}
