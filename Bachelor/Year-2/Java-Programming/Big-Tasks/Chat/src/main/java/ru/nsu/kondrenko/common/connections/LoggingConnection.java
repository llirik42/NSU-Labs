package ru.nsu.kondrenko.common.connections;

import ru.nsu.kondrenko.common.Connection;
import ru.nsu.kondrenko.common.Message;
import ru.nsu.kondrenko.common.modelobjects.Session;

import java.util.logging.Logger;

public class LoggingConnection implements Connection {
    private final Connection otherConnection;
    private final Logger logger;

    public LoggingConnection(Connection otherConnection, Logger logger) {
        this.otherConnection = otherConnection;
        this.logger = logger;
    }

    @Override
    public Session getSession() {
        return otherConnection.getSession();
    }

    @Override
    public void setSession(Session session) {
        final String logMessage = String.format("Connection %s obtained session=%s", otherConnection, session);
        logger.info(logMessage);
        otherConnection.setSession(session);
    }

    @Override
    public void send(Message message) {
        final String logMessage1 = String.format("Connection %s is sending %s", otherConnection, message);
        logger.info(logMessage1);

        otherConnection.send(message);

        final String logMessage2 = String.format("Connection %s sent %s", otherConnection, message);
        logger.info(logMessage2);
    }

    @Override
    public boolean isClosed() {
        return otherConnection.isClosed();
    }

    @Override
    public void close() {
        final String logMessage1 = String.format("Connection %s is closing ...", otherConnection);
        logger.info(logMessage1);

        otherConnection.close();

        final String logMessage2 = String.format("Connection %s is closed", otherConnection);
        logger.info(logMessage2);
    }

    @Override
    public String toString() {
        return String.format("[%s]", otherConnection);
    }
}
