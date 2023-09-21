package ru.nsu.kondrenko.server;

import ru.nsu.kondrenko.server.config.*;

import java.io.IOException;
import java.util.Properties;

public class Main {
    private static final String CONFIGURATION_FILE_NAME = "config.properties";

    public static void main(String[] args) {
        try {
            final ServerConfiguration serverConfiguration = readServerConfiguration();
            final ConfigValidator configValidator = new DefaultConfigValidator();
            configValidator.validate(serverConfiguration);

            final Server server = new DefaultServer(serverConfiguration);
            server.launch();
        } catch (ConfigException exception) {
            System.err.printf("Server didn't launch because of the exception: %s\n", exception.getLocalizedMessage());
        } catch (IllegalArgumentException exception) {
            System.err.println(exception.getLocalizedMessage()); // Invalid port
        } catch (IOException exception) {
            System.err.printf("Error during initializing server: %s\n", exception.getLocalizedMessage());
        }
    }

    private static ServerConfiguration readServerConfiguration() throws ConfigException {
        final ConfigReader configReader = new FileConfigReader(CONFIGURATION_FILE_NAME);
        final Properties properties = configReader.read();
        final ConfigParser configParser = new DefaultConfigParser();

        final int port = configParser.parsePort(properties);
        final int backlog = configParser.parseBacklog(properties);
        final String bindAddress = configParser.parseBindAddress(properties);
        final int transferRateUpdateTime = configParser.parseTransferRateUpdateTimeMs(properties);
        final int maxFileNameSize = configParser.parseMaxFileNameSize(properties);
        final long maxFileSize = configParser.parseMaxFileSize(properties);

        return new ServerConfiguration(
                port,
                backlog,
                bindAddress,
                transferRateUpdateTime,
                maxFileNameSize,
                maxFileSize
        );
    }
}
