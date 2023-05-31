package ru.nsu.kondrenko.common.protocols;

import ru.nsu.kondrenko.common.Message;

public interface MessageOutputStream extends MessageStream {
    void writeMessage(Message message) throws MessageStreamException;

    void flush() throws MessageStreamException;
}
