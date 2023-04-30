package ru.nsu.kondrenko.model.exceptions;

public class DivisionByZeroException extends InstructionException {
    public DivisionByZeroException() {
        super("Division by zero!");
    }
}
