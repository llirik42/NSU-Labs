package ru.nsu.kondrenko.model.exceptions;

public class ConfigException extends AbstractException {
    public ConfigException(String message) {
        super(message);
    }

    public ConfigException(String message, Throwable cause) {
        super(message, cause);
    }
}
