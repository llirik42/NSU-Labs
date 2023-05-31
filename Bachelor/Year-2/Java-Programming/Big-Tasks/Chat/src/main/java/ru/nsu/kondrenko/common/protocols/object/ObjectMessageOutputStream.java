package ru.nsu.kondrenko.common.protocols.object;

import ru.nsu.kondrenko.common.Message;
import ru.nsu.kondrenko.common.protocols.MessageOutputStream;
import ru.nsu.kondrenko.common.protocols.MessageStreamException;

import java.io.IOException;
import java.io.ObjectOutputStream;
import java.io.OutputStream;

class ObjectMessageOutputStream implements MessageOutputStream {
    private final ObjectOutputStream objectOutputStream;

    public ObjectMessageOutputStream(OutputStream outputStream) throws MessageStreamException {
        try {
            this.objectOutputStream = new ObjectOutputStream(outputStream);
        } catch (IOException exception) {
            throw new MessageStreamException(exception);
        }
    }

    @Override
    public void writeMessage(Message message) throws MessageStreamException {
        try {
            objectOutputStream.writeObject(message);
        } catch (IOException exception) {
            throw new MessageStreamException(exception);
        }
    }

    @Override
    public void flush() throws MessageStreamException {
        try {
            objectOutputStream.flush();
        } catch (IOException exception) {
            throw new MessageStreamException(exception);
        }
    }

    @Override
    public void close() throws MessageStreamException {
        try {
            objectOutputStream.close();
        } catch (IOException exception) {
            throw new MessageStreamException(exception);
        }
    }
}
