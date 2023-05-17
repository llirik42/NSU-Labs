package ru.nsu.kondrenko.model.work.supply;

import ru.nsu.kondrenko.model.listeners.SupplyListener;
import ru.nsu.kondrenko.model.products.CarPart;
import ru.nsu.kondrenko.model.storage.Storage;
import ru.nsu.kondrenko.model.work.factory.Worker;

public abstract class Supplier<P extends CarPart> extends Worker {
    private final Storage<P> storage;
    private final SupplyListener supplyListener;

    protected Supplier(Storage<P> storage, SupplyListener supplyListener, int workTime) {
        super(workTime);
        this.storage = storage;
        this.supplyListener = supplyListener;
    }

    @Override
    public void run() {
        while (!isSoftlyInterrupted()) {
            try {
                Thread.sleep(getWorkTime());
                final P carPart = supply();
                storage.put(carPart);
                supplyListener.notifyAboutSupply(carPart);
            } catch (InterruptedException exception) {
                interrupt();
                return;
            }
        }

        interrupt();
    }

    protected abstract P supply();
}
