package ru.nsu.kondrenko.gui.exceptions;

public class IconNotFoundException extends RuntimeException {
    public IconNotFoundException(String message) {
        super(message);
    }

    public IconNotFoundException(Throwable cause) {
        super(cause);
    }
}
