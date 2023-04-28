package ru.nsu.kondrenko.model.exceptions;

public class LoggingException extends AbstractException {
    public LoggingException(String message, Throwable cause) {
        super(message, cause);
    }
}
