package ru.nsu.kondrenko.model.work;

import ru.nsu.kondrenko.model.work.factory.Worker;

import java.util.ArrayList;
import java.util.List;

public class WorkersThreadPool<W extends Worker> {
    private final List<W> workers;
    private final List<W> softlyInterruptedWorkers;
    private final WorkersCreator<W> creator;
    private boolean started = false;

    public WorkersThreadPool(WorkersCreator<W> creator, int workersCount) {
        this.workers = new ArrayList<>();
        this.softlyInterruptedWorkers = new ArrayList<>();
        this.creator = creator;
        addWorkers(workersCount);
    }

    public void start() {
        started = true;
        workers.forEach(Thread::start);
    }

    public void terminate() {
        workers.forEach(Thread::interrupt);
        softlyInterruptedWorkers.forEach(Thread::interrupt);
    }

    public void setWorkTime(int workTime) {
        creator.setWorkTime(workTime);
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

    private void softlyInterruptSomeWorkers(int count) {
        for (int i = 0; i < count; i++) {
            final int indexOfLast = workers.size() - 1;
            final W workerToSoftlyInterrupt = workers.get(indexOfLast);
            workerToSoftlyInterrupt.softlyInterrupt();
            workers.remove(indexOfLast);
            softlyInterruptedWorkers.add(workerToSoftlyInterrupt);
        }
    }

    private void addWorkers(int count) {
        for (int i = 0; i < count; i++) {
            final W supplier = creator.createWorker();
            workers.add(supplier);

            if (started) {
                supplier.start();
            }
        }
    }
}
