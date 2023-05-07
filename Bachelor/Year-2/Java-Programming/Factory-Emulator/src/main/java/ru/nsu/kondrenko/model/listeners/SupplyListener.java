package ru.nsu.kondrenko.model.listeners;

import ru.nsu.kondrenko.model.products.CarPart;

public interface SupplyListener {
    void notifyAboutSupply(CarPart carPart);
}
