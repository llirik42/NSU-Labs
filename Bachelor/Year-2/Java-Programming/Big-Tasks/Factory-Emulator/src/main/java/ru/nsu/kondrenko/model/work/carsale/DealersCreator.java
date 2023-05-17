package ru.nsu.kondrenko.model.work.carsale;

import ru.nsu.kondrenko.model.listeners.CarRequestListener;
import ru.nsu.kondrenko.model.products.Car;
import ru.nsu.kondrenko.model.storage.Storage;
import ru.nsu.kondrenko.model.work.WorkersCreator;
import ru.nsu.kondrenko.model.work.factory.FactoryPlanner;

import java.util.logging.Logger;

public class DealersCreator extends WorkersCreator<Dealer> {
    private final Storage<Car> storage;
    private final CarRequestListener carRequestListener;
    private final FactoryPlanner factoryPlanner;
    private final Logger saleLogger;

    public DealersCreator(Storage<Car> storage,
                          CarRequestListener carRequestListener,
                          FactoryPlanner factoryPlanner,
                          Logger saleLogger,
                          int workTime) {

        super(workTime);
        this.storage = storage;
        this.carRequestListener = carRequestListener;
        this.factoryPlanner = factoryPlanner;
        this.saleLogger = saleLogger;
    }

    @Override
    public Dealer createWorker() {
        return new Dealer(storage, carRequestListener, factoryPlanner, saleLogger, getWorkTime());
    }
}
