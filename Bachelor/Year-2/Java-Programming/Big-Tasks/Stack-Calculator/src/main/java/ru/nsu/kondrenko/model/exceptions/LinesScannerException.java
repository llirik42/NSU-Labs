package ru.nsu.kondrenko.model.exceptions;

public class LinesScannerException extends AbstractException {
    public LinesScannerException(String message) {
        super(message);
    }

    public LinesScannerException(String message, Throwable cause) {
        super(message, cause);
    }
}
