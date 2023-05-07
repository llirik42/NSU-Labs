package ru.nsu.kondrenko.model.listeners;

import ru.nsu.kondrenko.model.products.Product;

public interface StorageListener {
    void notifyAboutProductPut(Product product);

    void notifyAboutProductTaken(Product product);
}
