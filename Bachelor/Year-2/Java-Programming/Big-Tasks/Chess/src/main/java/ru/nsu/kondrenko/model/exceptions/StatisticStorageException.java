package ru.nsu.kondrenko.model.exceptions;

public class StatisticStorageException extends RuntimeException {
    public StatisticStorageException(String message) {
        super(message);
    }

    public StatisticStorageException(String message, Throwable cause) {
        super(message, cause);
    }
}
