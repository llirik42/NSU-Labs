package ru.nsu.kondrenko.common;

public abstract class DefaultConnectionListener implements ConnectionListener {
    @Override
    public void onConnectionReady(Connection connection) {
        System.out.printf("Connection %s is ready\n\n", connection);
    }

    @Override
    public void onDisconnect(Connection connection) {
        System.out.printf("Connection %s is closed\n\n", connection);
    }

    @Override
    public abstract void onBytesReceived(Connection connection, byte[] bytes);

    @Override
    public void onException(Connection connection, Exception exception) {
        if (connection.isClosed()) {
            return;
        }

        System.err.printf("Exception on %s: %s\n\n", connection, exception.getLocalizedMessage());

        try {
            connection.close();
        } catch (Exception e) {
            System.err.printf("Exception %s during closing %s\n\n", e, connection);
        }
    }
}
