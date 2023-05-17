package ru.nsu.kondrenko.model.work.factory;

public abstract class Worker extends Thread {
    private int workTime;
    private boolean softlyInterrupted = false;

    protected Worker(int workTime) {
        this.workTime = workTime;
    }

    public void softlyInterrupt() {
        softlyInterrupted = true;
    }

    protected int getWorkTime() {
        return workTime;
    }

    public void setWorkTime(int workTime) {
        this.workTime = workTime;
    }

    public boolean isSoftlyInterrupted() {
        return softlyInterrupted;
    }

    @Override
    public abstract void run();
}
