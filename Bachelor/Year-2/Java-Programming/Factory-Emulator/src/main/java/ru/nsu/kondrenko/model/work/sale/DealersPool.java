package ru.nsu.kondrenko.model.work.sale;

import ru.nsu.kondrenko.model.listeners.CarRequestListener;
import ru.nsu.kondrenko.model.products.Car;
import ru.nsu.kondrenko.model.storage.Storage;
import ru.nsu.kondrenko.model.work.WorkersThreadPool;
import ru.nsu.kondrenko.model.work.factory.FactoryPlanner;

import java.util.logging.Logger;

public class DealersPool extends WorkersThreadPool<Dealer> {
    private final Storage<Car> storage;

    private final FactoryPlanner factoryPlanner;

    private final Logger logger;

    private CarRequestListener carRequestListener;

    public DealersPool(Storage<Car> storage, int dealersCount, int carRequestTime, FactoryPlanner factoryPlanner, Logger logger) {
        this.storage = storage;
        this.factoryPlanner = factoryPlanner;
        this.logger = logger;
        workTime = carRequestTime;
        addWorkers(dealersCount);
    }

    public void setCarRequestListener(CarRequestListener carRequestListener) {
        this.carRequestListener = carRequestListener;

        for (var it : workers) {
            it.setCarRequestListener(carRequestListener);
        }
    }

    @Override
    protected void addWorkers(int count) {
        for (int i = 0; i < count; i++) {
            final Dealer dealer = new Dealer(workTime, factoryPlanner, storage, logger);
            dealer.setCarRequestListener(carRequestListener);
            workers.add(dealer);

            if (started) {
                dealer.start();
            }
        }
    }
}
