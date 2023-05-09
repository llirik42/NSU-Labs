package ru.nsu.kondrenko.model.work.factory;

import ru.nsu.kondrenko.model.listeners.CarAssembleListener;
import ru.nsu.kondrenko.model.listeners.FactoryTasksListener;
import ru.nsu.kondrenko.model.products.Accessory;
import ru.nsu.kondrenko.model.products.Body;
import ru.nsu.kondrenko.model.products.Car;
import ru.nsu.kondrenko.model.products.Engine;
import ru.nsu.kondrenko.model.storage.Storage;
import ru.nsu.kondrenko.model.work.Worker;

public class FactoryWorker extends Worker {
    private static int tasksCount = 0;

    private final Object synchronizationObject;

    private final Storage<Body> bodyStorage;
    private final Storage<Engine> engineStorage;
    private final Storage<Accessory> accessoryStorage;
    private final Storage<Car> carStorage;

    private CarAssembleListener carAssembleListener;
    private FactoryTasksListener factoryTasksListener;

    public FactoryWorker(int workTIme, Object synchronizationObject,
                         Storage<Body> bodyStorage,
                         Storage<Engine> engineStorage,
                         Storage<Accessory> accessoryStorage,
                         Storage<Car> carStorage) {

        super(workTIme);
        this.synchronizationObject = synchronizationObject;
        this.bodyStorage = bodyStorage;
        this.engineStorage = engineStorage;
        this.accessoryStorage = accessoryStorage;
        this.carStorage = carStorage;
    }

    public static void addTasks(int newTasks) {
        tasksCount += newTasks;
    }

    @Override
    public void run() {
        while (true) {
            synchronized (synchronizationObject) {
                while (tasksCount == 0) {
                    try {
                        synchronizationObject.wait();
                    } catch (InterruptedException exception) {
                        interrupt();
                        return;
                    }
                }

                if (softlyInterrupted) {
                    interrupt();
                    return;
                }

                tasksCount--;
            }

            try {
                execute();
            } catch (InterruptedException exception) {
                interrupt();
                return;
            }

            if (softlyInterrupted) {
                interrupt();
            }
        }
    }

    public void setCarAssembleListener(CarAssembleListener carAssembleListener) {
        this.carAssembleListener = carAssembleListener;
    }

    public void setFactoryTasksListener(FactoryTasksListener factoryTasksListener) {
        this.factoryTasksListener = factoryTasksListener;
    }

    private void execute() throws InterruptedException {
        final Body body = bodyStorage.take();
        final Engine engine = engineStorage.take();
        final Accessory accessory = accessoryStorage.take();
        Thread.sleep(workTime);
        final Car car = new Car(body, engine, accessory);
        carStorage.put(car);
        carAssembleListener.notifyAboutCarAssemble();
        factoryTasksListener.notifyAboutClosingTask();
    }
}
