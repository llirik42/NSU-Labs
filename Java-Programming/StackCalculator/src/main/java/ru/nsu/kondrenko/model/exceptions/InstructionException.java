package ru.nsu.kondrenko.model.exceptions;

public class InstructionException extends AbstractException {
    public InstructionException(String message) {
        super(message);
    }

    public InstructionException(String message, Throwable cause) {
        super(message, cause);
    }
}
