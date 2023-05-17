package ru.nsu.kondrenko.model.work.factory;

import ru.nsu.kondrenko.model.listeners.FactoryTasksListener;
import ru.nsu.kondrenko.model.work.WorkersThreadPool;

public class Factory extends WorkersThreadPool<FactoryWorker> {
    private final Object workersSynchronizationObject;
    private final FactoryTasksListener factoryTasksListener;

    public Factory(FactoryWorkersCreator creator,
                   FactoryTasksListener factoryTasksListener,
                   int workersCount) {

        super(creator, workersCount);
        this.workersSynchronizationObject = creator.getWorkersSynchronizationObject();
        this.factoryTasksListener = factoryTasksListener;
    }

    public void addTasks(int count) {
        FactoryWorker.addTasks(count);
        factoryTasksListener.notifyAboutNewTasks(count);

        synchronized (workersSynchronizationObject) {
            workersSynchronizationObject.notifyAll();
        }
    }
}
