package ru.nsu.kondrenko.common;

public interface Connection {
    void send(byte[] bytes);

    void sendWithSize(byte[] bytes);

    void sendWithSize(String message);

    void setRawMode(long bytesCountToRead, int bufferSize);

    boolean isClosed();

    void close();
}
