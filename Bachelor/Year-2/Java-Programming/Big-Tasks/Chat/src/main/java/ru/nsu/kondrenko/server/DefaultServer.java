package ru.nsu.kondrenko.server;

import ru.nsu.kondrenko.common.Connection;
import ru.nsu.kondrenko.common.Message;
import ru.nsu.kondrenko.common.ServerContext;
import ru.nsu.kondrenko.common.connections.ConnectionListener;
import ru.nsu.kondrenko.common.connections.factories.ConnectionsFactory;
import ru.nsu.kondrenko.common.messages.*;
import ru.nsu.kondrenko.common.modelobjects.ChatUser;
import ru.nsu.kondrenko.common.modelobjects.Session;
import ru.nsu.kondrenko.common.protocols.Protocol;
import ru.nsu.kondrenko.server.config.ServerConfiguration;
import ru.nsu.kondrenko.server.serverdata.ServerData;

import java.io.IOException;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Collections;
import java.util.List;

class DefaultServer implements ServerContext, Server {
    private final ConnectionsFactory connectionsFactory;
    private final ServerSocket serverSocket;
    private final ServerData serverData;
    private final int recentMessagesCount;
    private final Protocol protocol;

    public DefaultServer(ServerConfiguration serverConfiguration,
                         ServerData serverData,
                         ConnectionsFactory connectionsFactory,
                         Protocol protocol) throws IOException {

        this.connectionsFactory = connectionsFactory;

        this.serverSocket = new ServerSocket(
                serverConfiguration.port(),
                serverConfiguration.backlog(),
                InetAddress.getByName(serverConfiguration.bindAddress())
        );

        this.serverData = serverData;
        this.recentMessagesCount = serverConfiguration.recentMessagesCount();
        this.protocol = protocol;
    }

    @Override
    public void launch() {
        final ConnectionListener connectionListener = getConnectionListener();

        while (true) {
            try {
                final Socket newClientSocket = serverSocket.accept();
                final Connection newConnection = createConnection(newClientSocket, connectionListener, protocol);
                serverData.addConnection(newConnection);
            } catch (IOException ignored) {
            }
        }
    }

    @Override
    public void removeConnectionBySession(Session session) {
        serverData.removeConnectionBySession(session);
    }

    @Override
    public void addChatUser(ChatUser chatUser, Session session) {
        serverData.addChatUser(chatUser, session);
    }

    @Override
    public ChatUser getChatUserBySession(Session session) {
        return serverData.getChatUserBySession(session);
    }

    @Override
    public void removeChatUserBySession(Session session) {
        serverData.removeChatUserBySession(session);
    }

    @Override
    public List<ChatUser> getChatUsers() {
        return Collections.unmodifiableList(serverData.getChatUsers());
    }

    @Override
    public boolean containsChatUser(ChatUser chatUser) {
        return serverData.containsUser(chatUser);
    }

    @Override
    public void addChatEvent(Event event) {
        serverData.addChatEvent(event);
    }

    @Override
    public List<Event> getRecentChatEvents() {
        return Collections.unmodifiableList(serverData.getRecentChatEvents(recentMessagesCount));
    }

    @Override
    public Session createSession() {
        return new Session();
    }

    @Override
    public List<Connection> getOpenConnections() {
        return Collections.unmodifiableList(serverData.getConnectionsList());
    }

    protected InetAddress getInetAddress() {
        return serverSocket.getInetAddress();
    }

    protected int getPort() {
        return serverSocket.getLocalPort();
    }

    protected Protocol getProtocol() {
        return protocol;
    }

    protected ConnectionListener getConnectionListener() {
        return new DefaultConnectionListener();
    }

    protected Connection createConnection(Socket socket, ConnectionListener connectionListener, Protocol protocol) throws IOException {
        return connectionsFactory.createConnection(
                socket,
                connectionListener,
                protocol
        );
    }

    protected class DefaultConnectionListener implements ConnectionListener {
        @Override
        public synchronized void onConnectionReady(Connection connection) {

        }

        @Override
        public synchronized void onDisconnect(Connection connection) {

        }

        @Override
        public synchronized void onException(Connection connection, Exception exception) {
            if (exception instanceof InvalidMessageException) {
                connection.send(new ErrorResponse(exception.getLocalizedMessage()));
            } else {
                if (!connection.isClosed()) {
                    connection.close();
                    final Request logoutRequest = new LogoutRequest(connection.getSession());
                    handle(logoutRequest, connection);
                }
            }
        }

        @Override
        public synchronized void onMessageReceived(Connection connection, Message message) {
            try {
                final Request request = (Request) message;

                if (request instanceof NonLoginRequest nonLoginRequest) {
                    final Session session = nonLoginRequest.getSession();

                    if (!serverData.containsUserBySession(session)) {
                        connection.send(new ErrorResponse("Unknown session id"));
                        return;
                    }

                    if (!session.equals(connection.getSession())) {
                        connection.send(new ErrorResponse("Incorrect session id"));
                        return;
                    }
                }

                handle(request, connection);
            } catch (ClassCastException exception) {
                connection.send(new ErrorResponse("Invalid request"));
            }
        }

        protected void handle(Request request, Connection connection) {
            request.handle(DefaultServer.this, connection);
        }
    }
}
