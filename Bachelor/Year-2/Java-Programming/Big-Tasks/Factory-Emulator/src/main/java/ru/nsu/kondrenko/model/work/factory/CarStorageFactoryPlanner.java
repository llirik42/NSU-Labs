package ru.nsu.kondrenko.model.work.factory;

import ru.nsu.kondrenko.model.products.Car;
import ru.nsu.kondrenko.model.storage.Storage;

public class CarStorageFactoryPlanner extends FactoryPlanner {
    private final Storage<Car> carStorage;

    public CarStorageFactoryPlanner(Factory factory, Storage<Car> carStorage) {
        super(factory);
        this.carStorage = carStorage;
    }

    @Override
    public int getNewTasksCount() {
        return carStorage.getRemainingCapacity() > 0 ? carStorage.getMaxCapacity() : 0;
    }
}
