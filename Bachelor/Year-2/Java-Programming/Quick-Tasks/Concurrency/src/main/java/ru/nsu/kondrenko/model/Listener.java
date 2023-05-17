package ru.nsu.kondrenko.model;

public interface Listener {
    void notifyConsumption(String consumerName, String product);
}
