package ru.nsu.kondrenko.server;

import org.apache.commons.io.FileUtils;
import ru.nsu.kondrenko.common.Connection;
import ru.nsu.kondrenko.common.DefaultConnection;
import ru.nsu.kondrenko.common.DefaultConnectionListener;
import ru.nsu.kondrenko.common.ProtocolUtils;
import ru.nsu.kondrenko.server.config.ServerConfiguration;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Arrays;
import java.util.Timer;
import java.util.TimerTask;

public class DefaultServer implements Server {
    private static final String uploadsDirectory = "uploads";
    private static final int readingBufferSize = 1024 * 1024;

    private final ServerSocket serverSocket;
    private final int transferRateUpdateTime;
    private final int maxFileNameSize;
    private final long maxFileSize;

    public DefaultServer(ServerConfiguration serverConfiguration) throws IOException {
        serverSocket = new ServerSocket(
                serverConfiguration.port(),
                serverConfiguration.backlog(),
                InetAddress.getByName(serverConfiguration.bindAddress())
        );
        maxFileNameSize = serverConfiguration.maxFileNameSize();
        transferRateUpdateTime = serverConfiguration.transferRateUpdateTime();
        maxFileSize = serverConfiguration.maxFileSize();
    }

    private static boolean isDefaultFileName(String string) {
        return new File(string).getName().equals(string);
    }

    @Override
    public void launch() {
        try {
            new File(uploadsDirectory).mkdirs();
        } catch (Exception exception) {
            System.err.println("Error during creating directory for uploads. Shutting down ...");
        }

        while (true) {
            try {
                final Socket newClientSocket = serverSocket.accept();
                final ConnectionHandler connectionHandler = new ConnectionHandler();
                final Connection newConnection = new DefaultConnection(newClientSocket, connectionHandler);
                connectionHandler.setConnection(newConnection);
            } catch (IOException exception) {
                System.err.printf("Error during opening connection with client: %s\n", exception.getLocalizedMessage());
            }
        }
    }

    private class ConnectionHandler extends DefaultConnectionListener {
        private final Timer timer;
        private final long startTimeMillis;
        private File outputFile;
        private OutputStream fileOutputStream;
        private long fileSize;
        private boolean startedTransfer = false;
        private long currentBytesCount;
        private long previousBytesCount;
        private long previousTimeMillis;

        private Connection connection;

        public ConnectionHandler() {
            this.startTimeMillis = System.currentTimeMillis();
            this.previousTimeMillis = 0;

            this.currentBytesCount = 0;
            this.previousBytesCount = 0;

            this.timer = new Timer();
            final TimerTask timerTask = new TimerTask() {
                @Override
                public void run() {
                    final long currentTimeMillis = System.currentTimeMillis();
                    currentBytesCount = outputFile.length();

                    final double totalTimeElapsed = (currentTimeMillis - startTimeMillis) / 1000.0;
                    final double localTimeElapsed = (currentTimeMillis - previousTimeMillis) / 1000.0;

                    final long averageRate = (long) (currentBytesCount / totalTimeElapsed);
                    final long instantRate = (long) ((currentBytesCount - previousBytesCount) / localTimeElapsed);

                    System.out.printf("Connection: %s\n", connection);
                    System.out.printf("Average rate: %s/s\n", FileUtils.byteCountToDisplaySize(averageRate));
                    System.out.printf("Instant rate: %s/s\n\n", FileUtils.byteCountToDisplaySize(instantRate));

                    previousTimeMillis = currentTimeMillis;
                    previousBytesCount = outputFile.length();
                }
            };

            timer.schedule(timerTask, transferRateUpdateTime, transferRateUpdateTime);
        }

        @Override
        public void onBytesReceived(Connection connection, byte[] bytes) {
            if (bytes.length == 0) {
                sendResult();
                return;
            }

            if (startedTransfer) {
                handleFileFragment(bytes);
            } else {
                handleRequest(bytes);
            }
        }

        @Override
        public void onException(Connection connection, Exception exception) {
            timer.cancel();
            super.onException(connection, exception);
        }

        private void sendResult() {
            if (outputFile.length() == fileSize) {
                connection.sendWithSize("Everything is OK");
            } else {
                connection.sendWithSize("Sizes differ!");
            }
            timer.cancel();
            connection.close();
        }

        private void handleFileFragment(byte[] bytes) {
            try {
                fileOutputStream.write(bytes);
                fileOutputStream.flush();
            } catch (Exception ignored) {
            }

            currentBytesCount += bytes.length;
        }

        public void setConnection(Connection connection) {
            this.connection = connection;
        }

        private void handleRequest(byte[] bytes) {
            // Get and validate file name
            final byte[] fileNameBytes = Arrays.copyOfRange(bytes, 0, bytes.length - Long.BYTES);
            if (fileNameBytes.length > maxFileNameSize) {
                final String error = String.format("Invalid file name: it must be shorter or equal %d bytes", maxFileNameSize);
                connection.sendWithSize(error);
                connection.close();
            }
            final String fileName = new String(fileNameBytes, ProtocolUtils.MAIN_CHARSET);
            if (!isDefaultFileName(fileName)) {
                final String error = String.format("Invalid format of file name: \"%s\"", fileName);
                connection.sendWithSize(error);
                connection.close();
            }

            // Get and validate file size
            final byte[] fileSizeBytes = Arrays.copyOfRange(bytes, bytes.length - Long.BYTES, bytes.length);
            final long fileSize = ProtocolUtils.bytesToLong(fileSizeBytes);
            if (fileSize <= 0 || fileSize > maxFileSize) {
                final String error = String.format("Invalid file size: it must be in (0; %d]", maxFileSize);
                connection.sendWithSize(error);
                connection.close();
            }

            startedTransfer = true;
            connection.setRawMode(fileSize, readingBufferSize);
            this.fileSize = fileSize;
            try {
                outputFile = new File(String.format("%s/%s", uploadsDirectory, fileName));
                fileOutputStream = new FileOutputStream(outputFile);
            } catch (Exception ignored) {
            }
        }
    }
}
