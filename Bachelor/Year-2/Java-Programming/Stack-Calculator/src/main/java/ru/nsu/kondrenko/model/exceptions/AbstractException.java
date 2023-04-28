package ru.nsu.kondrenko.model.exceptions;

public class AbstractException extends RuntimeException {
    public AbstractException(Throwable cause) {
        super(cause);
    }

    protected AbstractException(String message) {
        super(message);
    }

    protected AbstractException(String message, Throwable cause) {
        super(message, cause);
    }
}
