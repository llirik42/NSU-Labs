package ru.nsu.kondrenko.client;

import org.apache.commons.lang3.ArrayUtils;
import ru.nsu.kondrenko.common.Connection;
import ru.nsu.kondrenko.common.DefaultConnection;
import ru.nsu.kondrenko.common.DefaultConnectionListener;
import ru.nsu.kondrenko.common.ProtocolUtils;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;

public class DefaultClient extends DefaultConnectionListener implements Client {
    private final static int READING_BUFFER_SIZE = 1024 * 1024 * 1024;
    private final Connection connection;
    private final File file;

    private boolean isShutDown = false;

    public DefaultClient(int port, String server, File file) throws IOException {
        this.connection = new DefaultConnection(port, server, this);
        this.file = file;
    }

    @Override
    public void launch() {
        sendTransferRequest();
        sendFileContent();
    }

    public void shutdown() {
        if (isShutDown) {
            return;
        }

        isShutDown = true;
        System.out.println("Client is shutting down ...");
        try {
            connection.close();
        } catch (Exception exception) {
            System.err.printf("Error during shutting down the client: %s\n", exception.getLocalizedMessage());
        }
    }

    private void sendTransferRequest() {
        final byte[] fileNameBytes = file.getName().getBytes(ProtocolUtils.MAIN_CHARSET);
        final byte[] fileSizeBytes = ProtocolUtils.longToBytes(file.length());
        final byte[] requestContent = ArrayUtils.addAll(fileNameBytes, fileSizeBytes);
        connection.sendWithSize(requestContent);
    }

    private void sendFileContent() {
        try (final InputStream inputStream = new FileInputStream(file)) {
            while (inputStream.available() > 0) {
                final byte[] bytes = inputStream.readNBytes(READING_BUFFER_SIZE);
                connection.send(bytes);
            }
        } catch (Exception ignored) {

        }
    }

    @Override
    public void onDisconnect(Connection connection) {
    }

    @Override
    public void onBytesReceived(Connection connection, byte[] bytes) {
        final String message = new String(bytes, ProtocolUtils.MAIN_CHARSET);
        System.out.printf("Received message from server: %s\n", message);
        shutdown();
    }

    @Override
    public void onException(Connection connection, Exception exception) {
        super.onException(connection, exception);
        shutdown();
    }
}
