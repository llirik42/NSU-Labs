package ru.nsu.kondrenko.common.protocols;

import java.io.IOException;

public class MessageStreamException extends IOException {
    public MessageStreamException(String message) {
        super(message);
    }

    public MessageStreamException(Throwable cause) {
        super(cause);
    }

    public MessageStreamException(String message, Throwable cause) {
        super(message, cause);
    }
}
