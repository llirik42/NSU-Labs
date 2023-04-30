package ru.nsu.kondrenko.model.exceptions;

public class FlyweightException extends InstructionException {
    public FlyweightException() {
        super("Flyweight isn't configured!");
    }
}
