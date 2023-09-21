package ru.nsu.kondrenko.common;

public interface ConnectionListener {
    void onConnectionReady(Connection connection);

    void onDisconnect(Connection connection);

    void onBytesReceived(Connection connection, byte[] bytes);

    void onException(Connection connection, Exception exception);
}
