package ru.nsu.kondrenko.common.connections;

import ru.nsu.kondrenko.common.Connection;
import ru.nsu.kondrenko.common.Message;

public interface ConnectionListener {
    void onConnectionReady(Connection connection);

    void onDisconnect(Connection connection);

    void onException(Connection connection, Exception exception);

    void onMessageReceived(Connection connection, Message message);
}
