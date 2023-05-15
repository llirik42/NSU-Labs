package ru.nsu.kondrenko.model;

import ru.nsu.kondrenko.model.listeners.*;
import ru.nsu.kondrenko.model.parsing.config.Config;
import ru.nsu.kondrenko.model.products.Accessory;
import ru.nsu.kondrenko.model.products.Body;
import ru.nsu.kondrenko.model.products.Car;
import ru.nsu.kondrenko.model.products.Engine;
import ru.nsu.kondrenko.model.storage.ProductStorage;
import ru.nsu.kondrenko.model.work.WorkersThreadPool;
import ru.nsu.kondrenko.model.work.carsale.Dealer;
import ru.nsu.kondrenko.model.work.carsale.DealersCreator;
import ru.nsu.kondrenko.model.work.factory.Factory;
import ru.nsu.kondrenko.model.work.factory.FactoryPlanner;
import ru.nsu.kondrenko.model.work.factory.FactoryWorkersCreator;
import ru.nsu.kondrenko.model.work.factory.MaxFactoryPlanner;
import ru.nsu.kondrenko.model.work.supply.*;

import java.util.logging.Logger;

public class Emulator {
    private final WorkersThreadPool<BodySupplier> bodySuppliersPool;
    private final WorkersThreadPool<EngineSupplier> engineSuppliersPool;
    private final WorkersThreadPool<AccessorySupplier> accessorySuppliersPool;
    private final Factory factory;
    private final WorkersThreadPool<Dealer> dealersPool;
    private final FactoryPlanner factoryPlanner;
    private final View view;

    public Emulator(Config config,
                    Logger logger,
                    View view,
                    SupplyListener supplyListener,
                    CarAssembleListener carAssembleListener,
                    CarRequestListener carRequestListener,
                    StorageListener storageListener,
                    FactoryTasksListener factoryTasksListener) {

        this.view = view;
        final ProductStorage<Body> bodyStorage = new ProductStorage<>(config.bodyStorageCapacity(), storageListener);
        final ProductStorage<Engine> engineStorage = new ProductStorage<>(config.engineStorageCapacity(), storageListener);
        final ProductStorage<Accessory> accessoryStorage = new ProductStorage<>(config.accessoryStorageCapacity(), storageListener);
        final ProductStorage<Car> carStorage = new ProductStorage<>(config.carStorageCapacity(), storageListener);

        final BodySuppliersCreator bodySuppliersCreator = new BodySuppliersCreator(
                bodyStorage,
                supplyListener,
                config.bodySupplyTimeMs()
        );
        final EngineSuppliersCreator engineSuppliersCreator = new EngineSuppliersCreator(
                engineStorage,
                supplyListener,
                config.engineSupplyTimeMs()

        );
        final AccessorySuppliersCreator accessorySuppliersCreator = new AccessorySuppliersCreator(
                accessoryStorage,
                supplyListener,
                config.accessorySupplyTimeMs()
        );

        this.bodySuppliersPool = new WorkersThreadPool<>(
                bodySuppliersCreator,
                config.bodySuppliersCount()
        );
        this.engineSuppliersPool = new WorkersThreadPool<>(
                engineSuppliersCreator,
                config.engineSuppliersCount()
        );
        this.accessorySuppliersPool = new WorkersThreadPool<>(
                accessorySuppliersCreator,
                config.accessorySuppliersCount()
        );

        final FactoryWorkersCreator factoryWorkerWorkersCreator = new FactoryWorkersCreator(
                bodyStorage,
                engineStorage,
                accessoryStorage,
                carStorage,
                carAssembleListener,
                factoryTasksListener,
                config.carAssemblingTimeMs()
        );

        this.factory = new Factory(
                factoryWorkerWorkersCreator,
                factoryTasksListener, config.workersCount()
        );

        this.factoryPlanner = new MaxFactoryPlanner(factory);

        final DealersCreator dealersCreator = new DealersCreator(
                carStorage,
                carRequestListener,
                factoryPlanner,
                logger,
                config.carRequestTimeMs()
        );

        this.dealersPool = new WorkersThreadPool<>(
                dealersCreator,
                config.dealersCount()
        );
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
