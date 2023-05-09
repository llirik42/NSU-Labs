package ru.nsu.kondrenko.model.work;

import java.util.ArrayList;
import java.util.List;

public abstract class WorkersThreadPool<W extends Worker> {
    protected final List<W> workers = new ArrayList<>();

    protected final List<W> softlyInterruptedWorkers = new ArrayList<>();

    protected int workTime;

    protected boolean started = false;

    public void start() {
        started = true;
        workers.forEach(Thread::start);
    }

    public void terminate() {
        workers.forEach(Thread::interrupt);
        softlyInterruptedWorkers.forEach(Thread::interrupt);
    }

    public void setWorkTime(int workTime) {
        this.workTime = workTime;
        workers.forEach(worker -> worker.setWorkTime(workTime));
    }

    public void setWorkersCount(int count) {
        if (count == workers.size()) {
            return;
        }

        if (count > workers.size()) {
            addWorkers(count - workers.size());
        } else {
            softlyInterruptSomeWorkers(workers.size() - count);
        }
    }

    protected void softlyInterruptSomeWorkers(int count) {
        for (int i = 0; i < count; i++) {
            final int indexOfLast = workers.size() - 1;
            final W workerToSoftlyInterrupt = workers.get(indexOfLast);
            workerToSoftlyInterrupt.softlyInterrupt();
            workers.remove(indexOfLast);
            softlyInterruptedWorkers.add(workerToSoftlyInterrupt);
        }
    }

    protected abstract void addWorkers(int count);
}
