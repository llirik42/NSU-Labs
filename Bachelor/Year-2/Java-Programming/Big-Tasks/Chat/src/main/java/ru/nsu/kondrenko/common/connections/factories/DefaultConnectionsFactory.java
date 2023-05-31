package ru.nsu.kondrenko.common.connections.factories;

import ru.nsu.kondrenko.common.Connection;
import ru.nsu.kondrenko.common.connections.ConnectionListener;
import ru.nsu.kondrenko.common.protocols.MessageStreamsFactory;
import ru.nsu.kondrenko.common.protocols.Protocol;
import ru.nsu.kondrenko.common.protocols.object.ObjectMessageStreamsFactory;
import ru.nsu.kondrenko.common.protocols.xml.XMLMessageStreamsFactory;

import java.io.IOException;
import java.net.Socket;

public class DefaultConnectionsFactory implements ConnectionsFactory {
    @Override
    public Connection createConnection(Socket socket, ConnectionListener connectionListener, Protocol protocol) throws IOException {
        final MessageStreamsFactory messageStreamsFactory = protocol == Protocol.OBJECT
                ? new ObjectMessageStreamsFactory()
                : new XMLMessageStreamsFactory();

        return new DefaultConnection(socket, connectionListener, messageStreamsFactory);
    }
}
