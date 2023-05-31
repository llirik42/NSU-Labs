package ru.nsu.kondrenko.server;

import ru.nsu.kondrenko.common.connections.factories.ConnectionsFactory;
import ru.nsu.kondrenko.common.connections.factories.DefaultConnectionsFactory;
import ru.nsu.kondrenko.common.protocols.Protocol;
import ru.nsu.kondrenko.server.config.*;
import ru.nsu.kondrenko.server.logging.LoggingException;
import ru.nsu.kondrenko.server.logging.SimpleFormatter;
import ru.nsu.kondrenko.server.serverdata.RAMServerData;
import ru.nsu.kondrenko.server.serverdata.ServerData;

import java.io.IOException;
import java.util.Properties;
import java.util.logging.FileHandler;
import java.util.logging.Handler;
import java.util.logging.Level;
import java.util.logging.Logger;

public class Main {
    private static final String CONFIGURATION_FILE_NAME = "server/config.txt";
    private static final String LOG_FILE_NAME = "server.log";
    private static final int ERROR_CODE = 1;

    public static void main(String[] args) {
        final Logger errorsLogger = Logger.getAnonymousLogger();

        try {
            final ConfigReader configReader = new FileConfigReader(CONFIGURATION_FILE_NAME);
            final Properties properties = configReader.read();
            final ConfigParser configParser = new DefaultConfigParser();

            final int port = configParser.parsePort(properties);
            final int backlog = configParser.parseBacklog(properties);
            final String bindAddress = configParser.parseBindAddress(properties);
            final int recentMessagesCount = configParser.parseRecentMessagesCount(properties);
            final String stringProtocol = configParser.parseProtocol(properties);
            final boolean logging = configParser.parseLogging(properties);

            final ServerConfiguration serverConfiguration = new ServerConfiguration(port,
                    backlog,
                    bindAddress,
                    recentMessagesCount
            );

            final ConfigValidator configValidator = new DefaultConfigValidator();
            configValidator.validate(serverConfiguration);
            configValidator.validate(stringProtocol);

            final Protocol protocol = Protocol.valueOf(stringProtocol);
            final ServerData serverData = new RAMServerData();
            final ConnectionsFactory connectionsFactory = new DefaultConnectionsFactory();

            Server server;

            if (logging) {
                final Logger serverLogger = Logger.getAnonymousLogger();
                configureServerLogger(serverLogger);
                server = new LoggingServer(serverConfiguration, serverData, connectionsFactory, protocol, serverLogger);
            } else {
                server = new DefaultServer(serverConfiguration, serverData, connectionsFactory, protocol);
            }

            server.launch();
        } catch (ConfigException | LoggingException exception) {
            errorsLogger.severe(exception.getLocalizedMessage());
            System.exit(ERROR_CODE);
        } catch (IOException | SecurityException exception) {
            final String logMessage = String.format("Couldn't launch a server: %s%n", exception.getLocalizedMessage());
            errorsLogger.severe(logMessage);
            System.exit(ERROR_CODE);
        } catch (Exception exception) {
            final String logMessage = String.format("Unexpected exception: %s", exception.getLocalizedMessage());
            errorsLogger.severe(logMessage);
            System.exit(ERROR_CODE);
        }
    }

    private static void configureServerLogger(Logger serverLogger) throws LoggingException {
        try {
            serverLogger.setUseParentHandlers(false);
            final Handler handler = new FileHandler(LOG_FILE_NAME);
            handler.setFormatter(new SimpleFormatter());
            handler.setLevel(Level.ALL);
            serverLogger.addHandler(handler);
        } catch (IOException exception) {
            throw new LoggingException("Cannot open file for logging", exception);
        }
    }
}
