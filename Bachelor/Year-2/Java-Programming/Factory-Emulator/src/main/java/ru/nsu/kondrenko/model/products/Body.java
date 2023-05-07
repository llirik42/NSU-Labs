package ru.nsu.kondrenko.model.products;

public class Body implements CarPart {
    private final Id id = Id.createBodyId();

    @Override
    public int getId() {
        return id.getIntegerId();
    }

    @Override
    public String toString() {
        return String.format("Body-%d", getId());
    }
}
