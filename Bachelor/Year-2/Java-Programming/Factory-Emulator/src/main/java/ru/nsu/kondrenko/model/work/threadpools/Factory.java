package ru.nsu.kondrenko.model.work.threadpools;

import ru.nsu.kondrenko.model.listeners.CarAssembleListener;
import ru.nsu.kondrenko.model.listeners.FactoryTasksListener;
import ru.nsu.kondrenko.model.products.Accessory;
import ru.nsu.kondrenko.model.products.Body;
import ru.nsu.kondrenko.model.products.Car;
import ru.nsu.kondrenko.model.products.Engine;
import ru.nsu.kondrenko.model.storage.Storage;
import ru.nsu.kondrenko.model.work.workers.FactoryWorker;

public class Factory extends WorkersThreadPool<FactoryWorker> {
    private final Object synchronizationObject;

    private final Storage<Body> bodyStorage;
    private final Storage<Engine> engineStorage;
    private final Storage<Accessory> accessoryStorage;
    private final Storage<Car> carStorage;

    private CarAssembleListener carAssembleListener;
    private FactoryTasksListener factoryTasksListener;

    public Factory(Storage<Body> bodyStorage,
                   Storage<Engine> engineStorage,
                   Storage<Accessory> accessoryStorage,
                   Storage<Car> carStorage,
                   int carAssemblingTime,
                   int workersCount) {

        workTime = carAssemblingTime;
        this.bodyStorage = bodyStorage;
        this.engineStorage = engineStorage;
        this.accessoryStorage = accessoryStorage;
        this.carStorage = carStorage;
        synchronizationObject = new Object();
        addWorkers(workersCount);
    }

    public void addTasks(int count) {
        FactoryWorker.addTasks(count);
        factoryTasksListener.notifyAboutNewTasks(count);

        synchronized (synchronizationObject) {
            synchronizationObject.notifyAll();
        }
    }

    public void setCarAssembleListener(CarAssembleListener carAssembleListener) {
        for (final var it : workers) {
            it.setCarAssembleListener(carAssembleListener);
        }

        this.carAssembleListener = carAssembleListener;
    }

    public void setFactoryTasksListener(FactoryTasksListener factoryTasksListener) {
        this.factoryTasksListener = factoryTasksListener;

        for (final var it : workers) {
            it.setFactoryTasksListener(factoryTasksListener);
        }
    }

    @Override
    protected void addWorkers(int count) {
        for (int i = 0; i < count; i++) {
            final FactoryWorker factoryWorker = new FactoryWorker(
                    workTime,
                    synchronizationObject,
                    bodyStorage,
                    engineStorage,
                    accessoryStorage,
                    carStorage
            );

            factoryWorker.setFactoryTasksListener(factoryTasksListener);
            factoryWorker.setCarAssembleListener(carAssembleListener);
            factoryWorker.setWorkTime(workTime);
            workers.add(factoryWorker);

            if (started) {
                factoryWorker.start();
            }
        }
    }
}
