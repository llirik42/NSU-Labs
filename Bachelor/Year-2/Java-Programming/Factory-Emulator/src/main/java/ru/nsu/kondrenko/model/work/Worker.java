package ru.nsu.kondrenko.model.work;

public abstract class Worker extends Thread {
    protected int workTime;

    protected boolean softlyInterrupted = false;

    protected Worker(int workTime) {
        this.workTime = workTime;
    }

    @Override
    public abstract void run();

    public void setWorkTime(int workTime) {
        this.workTime = workTime;
    }

    public void softlyInterrupt() {
        softlyInterrupted = true;
    }
}
