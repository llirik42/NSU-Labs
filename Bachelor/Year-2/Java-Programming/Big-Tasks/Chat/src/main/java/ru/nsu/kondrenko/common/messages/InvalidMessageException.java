package ru.nsu.kondrenko.common.messages;

public class InvalidMessageException extends Exception {
    private final String receivedMessage;

    public InvalidMessageException() {
        super("Invalid message");
        receivedMessage = null;
    }

    public InvalidMessageException(String message) {
        super(message);
        receivedMessage = null;
    }

    public InvalidMessageException(String message, Throwable cause) {
        super(message, cause);
        receivedMessage = null;
    }

    public InvalidMessageException(Throwable cause, String receivedMessage) {
        super(cause);
        this.receivedMessage = receivedMessage;
    }

    public String getReceivedMessage() {
        return receivedMessage;
    }
}
