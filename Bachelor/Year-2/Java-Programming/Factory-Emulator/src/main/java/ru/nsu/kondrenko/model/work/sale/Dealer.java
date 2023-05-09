package ru.nsu.kondrenko.model.work.sale;

import ru.nsu.kondrenko.model.listeners.CarRequestListener;
import ru.nsu.kondrenko.model.products.Car;
import ru.nsu.kondrenko.model.storage.Storage;
import ru.nsu.kondrenko.model.work.Worker;
import ru.nsu.kondrenko.model.work.factory.FactoryPlanner;

import java.util.logging.Logger;

public class Dealer extends Worker {
    private final Storage<Car> storage;
    private final FactoryPlanner factoryPlanner;
    private final Logger logger;
    private final Object factoryPlannerSynchronizationObject;
    private CarRequestListener carRequestListener;

    public Dealer(int carRequestTime, FactoryPlanner factoryPlanner, Storage<Car> storage, Logger logger) {
        super(carRequestTime);
        this.factoryPlanner = factoryPlanner;
        this.storage = storage;
        this.logger = logger;
        this.factoryPlannerSynchronizationObject = factoryPlanner.getSynchronizationObject();
    }

    @Override
    public void run() {
        while (!softlyInterrupted) {
            try {
                final Car car = storage.take();
                carRequestListener.notifyAboutCarSale();

                if (logger != null) {
                    final String message = String.format("Dealer %s: %s%n", Thread.currentThread().getName(), car);
                    logger.info(message);
                }

                Thread.sleep(workTime);

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

    public void setCarRequestListener(CarRequestListener carRequestListener) {
        this.carRequestListener = carRequestListener;
    }
}
