package ru.nsu.kondrenko.model.work.supply;

import ru.nsu.kondrenko.model.listeners.SupplyListener;
import ru.nsu.kondrenko.model.products.CarPart;
import ru.nsu.kondrenko.model.storage.Storage;
import ru.nsu.kondrenko.model.work.Worker;

public abstract class Supplier<P extends CarPart> extends Worker {
    protected final Storage<P> storage;

    protected SupplyListener supplyListener;

    protected Supplier(int supplyTime, Storage<P> storage) {
        super(supplyTime);
        this.storage = storage;
        this.workTime = supplyTime;
    }

    @Override
    public void run() {
        while (!softlyInterrupted) {
            try {
                Thread.sleep(workTime);
                final P carPart = supplyCarPart();
                storage.put(carPart);
                supplyListener.notifyAboutSupply(carPart);
            } catch (InterruptedException exception) {
                interrupt();
                return;
            }
        }

        interrupt();
    }

    public void setSupplyListener(SupplyListener supplyListener) {
        this.supplyListener = supplyListener;
    }

    protected abstract P supplyCarPart();
}
