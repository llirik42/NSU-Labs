package ru.nsu.kondrenko.common.protocols.object;

import ru.nsu.kondrenko.common.Message;
import ru.nsu.kondrenko.common.messages.InvalidMessageException;
import ru.nsu.kondrenko.common.protocols.MessageInputStream;
import ru.nsu.kondrenko.common.protocols.MessageStreamException;

import java.io.IOException;
import java.io.InputStream;
import java.io.ObjectInputStream;

class ObjectMessageInputStream implements MessageInputStream {
    private static final String NON_MESSAGE_OBJECT_MESSAGE = "Received non-Message object";

    private final ObjectInputStream objectInputStream;

    public ObjectMessageInputStream(InputStream inputStream) throws MessageStreamException {
        try {
            this.objectInputStream = new ObjectInputStream(inputStream);
        } catch (IOException exception) {
            throw new MessageStreamException(exception);
        }
    }

    @Override
    public Message readMessage() throws MessageStreamException, InvalidMessageException {
        try {
            return (Message) objectInputStream.readObject();
        } catch (ClassCastException exception) {
            throw new InvalidMessageException(NON_MESSAGE_OBJECT_MESSAGE, exception);
        } catch (Exception exception) {
            throw new MessageStreamException(exception);
        }
    }

    @Override
    public void close() throws MessageStreamException {
        try {
            objectInputStream.close();
        } catch (IOException exception) {
            throw new MessageStreamException(exception);
        }
    }
}
