package ru.nsu.kondrenko.model.work.carsale;

import ru.nsu.kondrenko.model.listeners.CarRequestListener;
import ru.nsu.kondrenko.model.products.Car;
import ru.nsu.kondrenko.model.storage.Storage;
import ru.nsu.kondrenko.model.work.factory.FactoryPlanner;
import ru.nsu.kondrenko.model.work.factory.Worker;

import java.util.logging.Logger;

public class Dealer extends Worker {
    private final Storage<Car> storage;
    private final FactoryPlanner factoryPlanner;
    private final Logger logger;
    private final CarRequestListener carRequestListener;
    private final Object factoryPlannerSynchronizationObject;

    public Dealer(Storage<Car> storage,
                  CarRequestListener carRequestListener,
                  FactoryPlanner factoryPlanner,
                  Logger logger,
                  int carRequestTime) {

        super(carRequestTime);
        this.factoryPlanner = factoryPlanner;
        this.storage = storage;
        this.logger = logger;
        this.carRequestListener = carRequestListener;
        this.factoryPlannerSynchronizationObject = factoryPlanner.getSynchronizationObject();
    }

    @Override
    public void run() {
        while (!isSoftlyInterrupted()) {
            try {
                final Car car = storage.take();
                carRequestListener.notifyAboutCarSale();

                if (logger != null) {
                    final String message = String.format("Dealer %s: %s%n", Thread.currentThread().getName(), car);
                    logger.info(message);
                }

                Thread.sleep(getWorkTime());

                synchronized (factoryPlannerSynchronizationObject) {
                    factoryPlanner.notifyAboutCarSale();
                    factoryPlannerSynchronizationObject.notifyAll();
                }
            } catch (InterruptedException exception) {
                interrupt();
                return;
            }
        }

        interrupt();
    }
}
