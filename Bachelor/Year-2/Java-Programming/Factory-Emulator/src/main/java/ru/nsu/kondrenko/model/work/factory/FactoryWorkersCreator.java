package ru.nsu.kondrenko.model.work.factory;

import ru.nsu.kondrenko.model.listeners.CarAssembleListener;
import ru.nsu.kondrenko.model.listeners.FactoryTasksListener;
import ru.nsu.kondrenko.model.products.Accessory;
import ru.nsu.kondrenko.model.products.Body;
import ru.nsu.kondrenko.model.products.Car;
import ru.nsu.kondrenko.model.products.Engine;
import ru.nsu.kondrenko.model.storage.Storage;
import ru.nsu.kondrenko.model.work.WorkersCreator;

public class FactoryWorkersCreator extends WorkersCreator<FactoryWorker> {
    private final Object workersSynchronizationObject;
    private final Storage<Body> bodyStorage;
    private final Storage<Engine> engineStorage;
    private final Storage<Accessory> accessoryStorage;
    private final Storage<Car> carStorage;
    private final CarAssembleListener carAssembleListener;
    private final FactoryTasksListener factoryTasksListener;

    public FactoryWorkersCreator(Storage<Body> bodyStorage,
                                 Storage<Engine> engineStorage,
                                 Storage<Accessory> accessoryStorage,
                                 Storage<Car> carStorage,
                                 CarAssembleListener carAssembleListener,
                                 FactoryTasksListener factoryTasksListener,
                                 int workTime) {

        super(workTime);
        this.workersSynchronizationObject = new Object();
        this.bodyStorage = bodyStorage;
        this.engineStorage = engineStorage;
        this.accessoryStorage = accessoryStorage;
        this.carStorage = carStorage;
        this.carAssembleListener = carAssembleListener;
        this.factoryTasksListener = factoryTasksListener;
    }

    public Object getWorkersSynchronizationObject() {
        return workersSynchronizationObject;
    }

    @Override
    public FactoryWorker createWorker() {
        return new FactoryWorker(
                getWorkTime(),
                bodyStorage,
                engineStorage,
                accessoryStorage,
                carStorage,
                carAssembleListener,
                factoryTasksListener,
                workersSynchronizationObject
        );
    }
}
