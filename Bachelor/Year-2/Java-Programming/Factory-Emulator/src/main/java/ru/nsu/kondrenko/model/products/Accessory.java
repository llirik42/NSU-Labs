package ru.nsu.kondrenko.model.products;

public class Accessory implements CarPart {
    private final Id id = Id.createAccessoryId();

    @Override
    public int getId() {
        return id.getIntegerId();
    }

    @Override
    public String toString() {
        return String.format("Accessory-%d", getId());
    }
}
