package ru.nsu.kondrenko.model.work.workers;

import ru.nsu.kondrenko.model.FactoryPlanner;
import ru.nsu.kondrenko.model.listeners.CarRequestListener;
import ru.nsu.kondrenko.model.products.Car;
import ru.nsu.kondrenko.model.storage.Storage;

import java.util.logging.Logger;

public class Dealer extends Worker {
    private final Storage<Car> storage;
    private final FactoryPlanner factoryPlanner;
    private final Logger logger;
    private CarRequestListener carRequestListener;

    public Dealer(int carRequestTime, FactoryPlanner factoryPlanner, Storage<Car> storage, Logger logger) {
        super(carRequestTime);
        this.factoryPlanner = factoryPlanner;
        this.storage = storage;
        this.logger = logger;
    }

    @Override
    public void run() {
        while (!softlyInterrupted) {
            try {
                Thread.sleep(workTime);
                final Car car = storage.take();
                carRequestListener.notifyAboutCarSale();

                if (logger != null) {
                    logger.info(String.format("Dealer %s: %s%n", Thread.currentThread().getName(), car));
                }

                synchronized (factoryPlanner) {
                    factoryPlanner.notifyAboutCarSale();
                    factoryPlanner.notify();
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
