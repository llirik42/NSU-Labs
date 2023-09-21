package ru.nsu.kondrenko.common;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.InetAddress;
import java.net.Socket;

public class DefaultConnection implements Connection {
    private final Socket socket;
    private final OutputStream outputStream;
    private final InputStream inputStream;
    private final ConnectionListener connectionListener;
    private final Thread rxThread;

    private boolean isClosed = false;
    private boolean rawMode = false;
    private long bytesCountToRead;
    private int bufferSize;

    public DefaultConnection(Socket socket, ConnectionListener connectionListener) throws IOException {
        this.socket = socket;
        this.inputStream = socket.getInputStream();
        this.outputStream = socket.getOutputStream();
        this.connectionListener = connectionListener;

        this.rxThread = new Thread(new Runnable() {
            @Override
            public void run() {
                while (!rxThread.isInterrupted()) {
                    try {
                        if (!rawMode) {
                            final int messageSize = readMessageSize();
                            final byte[] message = readMessage(messageSize);
                            connectionListener.onBytesReceived(DefaultConnection.this, message);
                        } else {
                            final byte[] bytes = inputStream.readNBytes((int) Long.min(bytesCountToRead, DefaultConnection.this.bufferSize));
                            connectionListener.onBytesReceived(DefaultConnection.this, bytes);
                            bytesCountToRead -= bytes.length;
                        }
                    } catch (Exception exception) {
                        connectionListener.onException(DefaultConnection.this, exception);
                    }
                }
            }
        });

        connectionListener.onConnectionReady(this);
        rxThread.start();
    }

    public DefaultConnection(int port, String server, ConnectionListener connectionListener) throws IOException {
        this(new Socket(InetAddress.getByName(server), port), connectionListener);
    }

    @Override
    public void send(byte[] bytes) {
        try {
            outputStream.write(bytes);
            outputStream.flush();
        } catch (Exception exception) {
            connectionListener.onException(this, exception);
        }
    }

    @Override
    public void setRawMode(long bytesCountToRead, int bufferSize) {
        this.rawMode = true;
        this.bytesCountToRead = bytesCountToRead;
        this.bufferSize = bufferSize;
    }

    @Override
    public void sendWithSize(byte[] bytes) {
        send(ProtocolUtils.intToBytes(bytes.length));
        send(bytes);
    }

    @Override
    public void sendWithSize(String message) {
        sendWithSize(message.getBytes(ProtocolUtils.MAIN_CHARSET));
    }

    @Override
    public boolean isClosed() {
        return isClosed;
    }

    @Override
    public void close() {
        if (isClosed) {
            return;
        }

        try {
            connectionListener.onDisconnect(this);
            rxThread.interrupt();
            socket.close();
        } catch (IOException exception) {
            connectionListener.onException(this, exception);
        } finally {
            isClosed = true;
        }
    }

    @Override
    public String toString() {
        return String.format("%s:%d", socket.getInetAddress().getHostName(), socket.getPort());
    }

    private int readMessageSize() throws IOException {
        final byte[] messageSizeBytes = inputStream.readNBytes(Integer.BYTES);
        if (messageSizeBytes.length == 0) {
            throw new IOException("Input stream is suddenly closed");
        }
        return ProtocolUtils.bytesToInt(messageSizeBytes);
    }

    private byte[] readMessage(int messageSize) throws IOException {
        return inputStream.readNBytes(messageSize);
    }
}
