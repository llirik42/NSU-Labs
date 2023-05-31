package ru.nsu.kondrenko.common.connections.factories;

import ru.nsu.kondrenko.common.Connection;
import ru.nsu.kondrenko.common.connections.ConnectionListener;
import ru.nsu.kondrenko.common.protocols.Protocol;

import java.io.IOException;
import java.net.Socket;

public interface ConnectionsFactory {
    Connection createConnection(Socket socket, ConnectionListener connectionListener, Protocol protocol) throws IOException;
}
