package ru.nsu.kondrenko.model.products;

public class Car implements Product {
    private final Id id = Id.createCarId();

    private final Body body;
    private final Engine engine;
    private final Accessory accessory;

    public Car(Body body, Engine engine, Accessory accessory) {
        this.body = body;
        this.engine = engine;
        this.accessory = accessory;
    }

    public Body getBody() {
        return body;
    }

    public Engine getEngine() {
        return engine;
    }

    public Accessory getAccessory() {
        return accessory;
    }

    @Override
    public int getId() {
        return id.getIntegerId();
    }

    @Override
    public String toString() {
        return String.format("Car-%d (Components: %s, %s, %s)", getId(), getBody(), getEngine(), getAccessory());
    }
}
