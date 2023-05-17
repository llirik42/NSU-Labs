package ru.nsu.kondrenko.model.products;

public class Engine implements CarPart {
    private final Id id = Id.createEngineId();

    @Override
    public int getId() {
        return id.getIntegerId();
    }

    @Override
    public String toString() {
        return String.format("Engine-%d", getId());
    }
}
