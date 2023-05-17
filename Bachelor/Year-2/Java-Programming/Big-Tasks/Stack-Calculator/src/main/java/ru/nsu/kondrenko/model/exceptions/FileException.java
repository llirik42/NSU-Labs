package ru.nsu.kondrenko.model.exceptions;

public class FileException extends AbstractException {
    public FileException(String message) {
        super(message);
    }

    public FileException(String message, Throwable cause) {
        super(message, cause);
    }
}
