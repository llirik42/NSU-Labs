package ru.nsu.kondrenko.server.logging;

public class LoggingException extends Exception {
    public LoggingException(String message, Throwable cause) {
        super(message, cause);
    }
}
