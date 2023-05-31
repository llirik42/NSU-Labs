package ru.nsu.kondrenko.server;

import ru.nsu.kondrenko.common.Connection;
import ru.nsu.kondrenko.common.Message;
import ru.nsu.kondrenko.common.connections.ConnectionListener;
import ru.nsu.kondrenko.common.connections.LoggingConnection;
import ru.nsu.kondrenko.common.connections.factories.ConnectionsFactory;
import ru.nsu.kondrenko.common.messages.Event;
import ru.nsu.kondrenko.common.messages.InvalidMessageException;
import ru.nsu.kondrenko.common.messages.LoggingRequestWrapper;
import ru.nsu.kondrenko.common.messages.Request;
import ru.nsu.kondrenko.common.modelobjects.ChatUser;
import ru.nsu.kondrenko.common.modelobjects.Session;
import ru.nsu.kondrenko.common.protocols.Protocol;
import ru.nsu.kondrenko.server.config.ServerConfiguration;
import ru.nsu.kondrenko.server.serverdata.ServerData;

import java.io.IOException;
import java.net.Socket;
import java.util.logging.Logger;

class LoggingServer extends DefaultServer {
    private final Logger logger;

    public LoggingServer(ServerConfiguration serverConfiguration,
                         ServerData serverData,
                         ConnectionsFactory connectionsFactory,
                         Protocol protocol,
                         Logger logger) throws IOException {

        super(serverConfiguration, serverData, connectionsFactory, protocol);
        this.logger = logger;
    }

    @Override
    public void launch() {
        final String logMessage = String.format(
                "Server launched with %s-protocol on address %s:%d",
                getProtocol(),
                getInetAddress().getHostAddress(),
                getPort()
        );
        logger.info(logMessage);
        super.launch();
    }

    @Override
    public void removeConnectionBySession(Session session) {
        final String logMessage = String.format("Session %s removed", session);
        logger.info(logMessage);
        super.removeConnectionBySession(session);
    }

    @Override
    public void addChatUser(ChatUser chatUser, Session session) {
        final String logMessage = String.format("User \"%s\" joined with session=%s", chatUser.name(), session);
        logger.info(logMessage);
        super.addChatUser(chatUser, session);
    }

    @Override
    public void removeChatUserBySession(Session session) {
        final ChatUser chatUser = super.getChatUserBySession(session);
        final String logMessage = String.format("User \"%s\" left with session=%s", chatUser.name(), session);
        logger.info(logMessage);
        super.removeChatUserBySession(session);
    }

    @Override
    public void addChatEvent(Event event) {
        final String logMessage = String.format("Added to history %s", event);
        logger.info(logMessage);
        super.addChatEvent(event);
    }

    @Override
    public Session createSession() {
        final Session newSession = super.createSession();
        final String message = String.format("Created a new session=%s", newSession);
        logger.info(message);
        return newSession;
    }

    @Override
    protected ConnectionListener getConnectionListener() {
        return new LoggingConnectionListener();
    }

    @Override
    protected Connection createConnection(Socket socket, ConnectionListener connectionListener, Protocol protocol) throws IOException {
        final Connection connection = super.createConnection(socket, connectionListener, protocol);
        return new LoggingConnection(connection, logger);

    }

    private class LoggingConnectionListener extends DefaultConnectionListener {
        @Override
        public synchronized void onConnectionReady(Connection connection) {
            final String logMessage = String.format("Connection with %s is ready", connection);
            logger.info(logMessage);
            super.onConnectionReady(connection);
        }

        @Override
        public synchronized void onDisconnect(Connection connection) {
            final String logMessage = String.format("Connection with %s is closed", connection);
            logger.info(logMessage);
            super.onDisconnect(connection);
        }

        @Override
        public synchronized void onException(Connection connection, Exception exception) {
            String logMessage = String.format(
                    "%s with message \"%s\" is thrown in connection with %s",
                    exception.getClass(),
                    exception.getLocalizedMessage(),
                    connection
            );

            if (exception instanceof InvalidMessageException invalidMessageException && invalidMessageException.getReceivedMessage() != null) {
                logMessage += '\n' + invalidMessageException.getReceivedMessage();
            }

            logger.info(logMessage);
            super.onException(connection, exception);
        }

        @Override
        public synchronized void onMessageReceived(Connection connection, Message message) {
            final String logMessage = String.format("Received %s from %s", message, connection);
            logger.info(logMessage);
            super.onMessageReceived(connection, message);
        }

        @Override
        protected void handle(Request request, Connection connection) {
            new LoggingRequestWrapper(request, logger).handle(LoggingServer.this, connection);
        }
    }
}
