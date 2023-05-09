package ru.nsu.kondrenko.model;

import ru.nsu.kondrenko.model.listeners.*;
import ru.nsu.kondrenko.model.parsing.config.Config;
import ru.nsu.kondrenko.model.products.Accessory;
import ru.nsu.kondrenko.model.products.Body;
import ru.nsu.kondrenko.model.products.Car;
import ru.nsu.kondrenko.model.products.Engine;
import ru.nsu.kondrenko.model.storage.ProductStorage;
import ru.nsu.kondrenko.model.work.factory.Factory;
import ru.nsu.kondrenko.model.work.factory.FactoryPlanner;
import ru.nsu.kondrenko.model.work.factory.MaxFactoryPlanner;
import ru.nsu.kondrenko.model.work.sale.DealersPool;
import ru.nsu.kondrenko.model.work.supply.AccessorySuppliersPool;
import ru.nsu.kondrenko.model.work.supply.BodySuppliersPool;
import ru.nsu.kondrenko.model.work.supply.EngineSuppliersPool;

import java.util.logging.Logger;

public class Emulator {
    private final ProductStorage<Body> bodyStorage;
    private final ProductStorage<Engine> engineStorage;
    private final ProductStorage<Accessory> accessoryStorage;
    private final ProductStorage<Car> carStorage;
    private final BodySuppliersPool bodySuppliersPool;
    private final EngineSuppliersPool engineSuppliersPool;
    private final AccessorySuppliersPool accessorySuppliersPool;
    private final Factory factory;
    private final DealersPool dealersPool;
    private final FactoryPlanner factoryPlanner;
    private View view;

    public Emulator(Config config, Logger logger) {
        this.bodyStorage = new ProductStorage<>(config.bodyStorageCapacity());
        this.engineStorage = new ProductStorage<>(config.engineStorageCapacity());
        this.accessoryStorage = new ProductStorage<>(config.accessoryStorageCapacity());
        this.carStorage = new ProductStorage<>(config.carStorageCapacity());

        this.bodySuppliersPool = new BodySuppliersPool(
                bodyStorage,
                config.bodySuppliersCount(),
                config.bodySupplyTimeMs()
        );

        this.engineSuppliersPool = new EngineSuppliersPool(
                engineStorage,
                config.engineSuppliersCount(),
                config.engineSupplyTimeMs()
        );

        this.accessorySuppliersPool = new AccessorySuppliersPool(
                accessoryStorage,
                config.accessorySuppliersCount(),
                config.accessorySupplyTimeMs()
        );

        this.factory = new Factory(
                bodyStorage,
                engineStorage,
                accessoryStorage,
                carStorage,
                config.carAssemblingTimeMs(),
                config.workersCount()
        );

        this.factoryPlanner = new MaxFactoryPlanner(factory);

        this.dealersPool = new DealersPool(
                carStorage,
                config.dealersCount(),
                config.carRequestTimeMs(),
                factoryPlanner,
                logger
        );
    }

    public void setView(View view) {
        this.view = view;
    }

    public void setSupplyListener(SupplyListener supplyListener) {
        bodySuppliersPool.setSupplyListener(supplyListener);
        engineSuppliersPool.setSupplyListener(supplyListener);
        accessorySuppliersPool.setSupplyListener(supplyListener);
    }

    public void setCarAssembleListener(CarAssembleListener carAssembleListener) {
        factory.setCarAssembleListener(carAssembleListener);
    }

    public void setCarRequestListener(CarRequestListener carRequestListener) {
        dealersPool.setCarRequestListener(carRequestListener);
    }

    public void setStorageListener(StorageListener storageListener) {
        bodyStorage.setStorageListener(storageListener);
        engineStorage.setStorageListener(storageListener);
        accessoryStorage.setStorageListener(storageListener);
        carStorage.setStorageListener(storageListener);
    }

    public void setFactoryTasksListener(FactoryTasksListener factoryTasksListener) {
        factory.setFactoryTasksListener(factoryTasksListener);
    }

    public void setBodySupplyTime(int newValue) {
        bodySuppliersPool.setWorkTime(newValue);
    }

    public void setEngineSupplyTime(int newValue) {
        engineSuppliersPool.setWorkTime(newValue);
    }

    public void setAccessorySupplyTime(int newValue) {
        accessorySuppliersPool.setWorkTime(newValue);
    }

    public void setCarAssembleTime(int newValue) {
        factory.setWorkTime(newValue);
    }

    public void setCarRequestTime(int newValue) {
        dealersPool.setWorkTime(newValue);
    }

    public void setBodySuppliersCount(int newValue) {
        bodySuppliersPool.setWorkersCount(newValue);
    }

    public void setEngineSuppliersCount(int newValue) {
        engineSuppliersPool.setWorkersCount(newValue);
    }

    public void setAccessorySuppliersCount(int newValue) {
        accessorySuppliersPool.setWorkersCount(newValue);
    }

    public void setWorkersCount(int newValue) {
        factory.setWorkersCount(newValue);
    }

    public void setDealersCount(int newValue) {
        dealersPool.setWorkersCount(newValue);
    }

    public void start() {
        bodySuppliersPool.start();
        engineSuppliersPool.start();
        accessorySuppliersPool.start();
        dealersPool.start();
        factoryPlanner.start();
        factory.start();
    }

    public void terminate() {
        bodySuppliersPool.terminate();
        engineSuppliersPool.terminate();
        accessorySuppliersPool.terminate();
        dealersPool.terminate();
        factory.terminate();
        factoryPlanner.interrupt();
        view.close();
    }
}
