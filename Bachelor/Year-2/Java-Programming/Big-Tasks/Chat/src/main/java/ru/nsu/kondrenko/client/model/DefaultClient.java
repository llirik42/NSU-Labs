package ru.nsu.kondrenko.client.model;

import ru.nsu.kondrenko.client.model.messagedocument.MessageDocument;
import ru.nsu.kondrenko.common.ClientContext;
import ru.nsu.kondrenko.common.Connection;
import ru.nsu.kondrenko.common.Message;
import ru.nsu.kondrenko.common.connections.ConnectionListener;
import ru.nsu.kondrenko.common.connections.factories.ConnectionsFactory;
import ru.nsu.kondrenko.common.messages.*;
import ru.nsu.kondrenko.common.modelobjects.ChatUser;
import ru.nsu.kondrenko.common.modelobjects.Session;
import ru.nsu.kondrenko.common.protocols.Protocol;

import java.net.Socket;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class DefaultClient implements ClientContext, Client {
    private final List<ChatUser> chatUsersList = new ArrayList<>();
    private final MessageDocument messageDocument;
    private final ClientListener clientListener;
    private final ConnectionsFactory connectionsFactory;
    private Connection connection;
    private Session session;

    public DefaultClient(MessageDocument messageDocument,
                         ClientListener clientListener,
                         ConnectionsFactory connectionsFactory) {
        this.messageDocument = messageDocument;
        this.clientListener = clientListener;
        this.connectionsFactory = connectionsFactory;
    }

    @Override
    public void join(int port, String serverAddress, ChatUser chatUser, Protocol protocol) {
        final ConnectionListener connectionListener = new ClientConnectionListener();

        try {
            final Socket socket = new Socket(serverAddress, port);
            connection = connectionsFactory.createConnection(socket, connectionListener, protocol);
            connection.send(new LoginRequest(chatUser));
        } catch (Exception exception) {
            clientListener.onException(exception);
            shutdown();
        }
    }

    @Override
    public void leave() {
        if (connection != null) {
            connection.send(new LogoutRequest(session));
            connection.close();
        }
    }

    @Override
    public void shutdown() {
        if (connection != null) {
            connection.close();
        }
        clientListener.onClientDisconnect();
    }

    @Override
    public void sendChatMessage() {
        connection.send(new MessageSendingRequest(session, messageDocument.getContent().strip()));
        messageDocument.clearContent();
    }

    @Override
    public void insertLineBreak() {
        messageDocument.insertLineBreak();
    }

    @Override
    public Connection getConnection() {
        return connection;
    }

    @Override
    public void setSession(Session session) {
        this.session = session;
    }

    @Override
    public void addChatUser(ChatUser chatUser) {
        chatUsersList.add(chatUser);
    }

    @Override
    public void removeChatUser(ChatUser chatUser) {
        chatUsersList.remove(chatUser);
    }

    @Override
    public void updateChatUsersList(List<ChatUser> chatUsersList) {
        this.chatUsersList.clear();
        this.chatUsersList.addAll(chatUsersList);
    }

    @Override
    public List<ChatUser> getChatUsersList() {
        return Collections.unmodifiableList(chatUsersList);
    }

    private class ClientConnectionListener implements ConnectionListener {
        @Override
        public void onConnectionReady(Connection connection) {

        }

        @Override
        public void onDisconnect(Connection connection) {
            clientListener.onClientDisconnect();
        }

        @Override
        public void onException(Connection connection, Exception exception) {
            clientListener.onException(exception);
            shutdown();
        }

        @Override
        public void onMessageReceived(Connection connection, Message message) {
            try {
                final ServerMessage serverMessage = (ServerMessage) message;
                serverMessage.handle(DefaultClient.this, clientListener);
            } catch (ClassCastException exception) {
                connection.send(new ErrorResponse("Invalid message"));
                clientListener.onErrorResponse("Server send invalid message");
            }
        }
    }
}
