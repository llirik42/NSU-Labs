package ru.nsu.kondrenko.common.connections.factories;

import ru.nsu.kondrenko.common.Connection;
import ru.nsu.kondrenko.common.Message;
import ru.nsu.kondrenko.common.connections.ConnectionListener;
import ru.nsu.kondrenko.common.modelobjects.Session;
import ru.nsu.kondrenko.common.protocols.MessageInputStream;
import ru.nsu.kondrenko.common.protocols.MessageOutputStream;
import ru.nsu.kondrenko.common.protocols.MessageStreamsFactory;

import java.io.IOException;
import java.net.Socket;

class DefaultConnection implements Connection {
    private final Socket socket;
    private final MessageOutputStream messageOutputStream;
    private final MessageInputStream messageInputStream;
    private final ConnectionListener connectionListener;
    private final Thread rxThread;
    private Session session;
    private boolean closed = false;

    public DefaultConnection(Socket socket, ConnectionListener connectionListener, MessageStreamsFactory factory) throws IOException {
        this.socket = socket;

        this.messageOutputStream = factory.createMessageOutputStream(socket.getOutputStream());
        this.messageInputStream = factory.createMessageInputStream(socket.getInputStream());
        this.connectionListener = connectionListener;

        this.rxThread = new Thread(new Runnable() {
            @Override
            public void run() {
                while (!rxThread.isInterrupted()) {
                    try {
                        final Message message = messageInputStream.readMessage();
                        connectionListener.onMessageReceived(DefaultConnection.this, message);
                    } catch (Exception exception) {
                        connectionListener.onException(DefaultConnection.this, exception);
                    }
                }
            }
        });

        connectionListener.onConnectionReady(this);
        this.rxThread.start();
    }

    @Override
    public Session getSession() {
        return session;
    }

    @Override
    public void setSession(Session session) {
        this.session = session;
    }

    @Override
    public void send(Message message) {
        try {
            messageOutputStream.writeMessage(message);
            messageOutputStream.flush();
        } catch (Exception exception) {
            connectionListener.onException(this, exception);
        }
    }

    @Override
    public boolean isClosed() {
        return closed;
    }

    @Override
    public void close() {
        if (closed) {
            return;
        }

        closed = true;

        connectionListener.onDisconnect(this);
        rxThread.interrupt();

        try {
            socket.close();
        } catch (IOException exception) {
            connectionListener.onException(this, exception);
        }
    }

    @Override
    public String toString() {
        return String.format("%s:%d", socket.getInetAddress().getHostName(), socket.getPort());
    }
}
