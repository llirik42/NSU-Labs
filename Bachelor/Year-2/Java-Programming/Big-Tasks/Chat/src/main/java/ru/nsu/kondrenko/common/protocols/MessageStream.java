package ru.nsu.kondrenko.common.protocols;

public interface MessageStream extends AutoCloseable {
    @Override
    void close() throws MessageStreamException;
}
