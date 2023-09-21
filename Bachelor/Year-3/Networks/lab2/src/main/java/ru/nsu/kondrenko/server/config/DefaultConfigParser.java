package ru.nsu.kondrenko.server.config;

import java.util.Properties;

public class DefaultConfigParser implements ConfigParser {
    private static final String PORT_KEY = "PORT";
    private static final String BACKLOG_KEY = "BACKLOG";
    private static final String BIND_ADDRESS_KEY = "BIND_ADDRESS";
    private static final String TRANSFER_RATE_UPDATE_TIME_MS_KEY = "TRANSFER_RATE_UPDATE_TIME_MS";
    private static final String MAX_FILE_NAME_SIZE_KEY = "MAX_FILE_NAME_SIZE";
    private static final String MAX_FILE_SIZE_KEY = "MAX_FILE_SIZE";

    @Override
    public int parsePort(Properties properties) throws ConfigException {
        return ParsingUtils.parseIntegerProperty(PORT_KEY, properties);
    }

    @Override
    public int parseBacklog(Properties properties) throws ConfigException {
        return ParsingUtils.parseIntegerProperty(BACKLOG_KEY, properties);
    }

    @Override
    public String parseBindAddress(Properties properties) throws ConfigException {
        return ParsingUtils.parseStringProperty(BIND_ADDRESS_KEY, properties);
    }

    @Override
    public int parseTransferRateUpdateTimeMs(Properties properties) throws ConfigException {
        return ParsingUtils.parseIntegerProperty(TRANSFER_RATE_UPDATE_TIME_MS_KEY, properties);
    }

    @Override
    public int parseMaxFileNameSize(Properties properties) throws ConfigException {
        return ParsingUtils.parseIntegerProperty(MAX_FILE_NAME_SIZE_KEY, properties);
    }

    @Override
    public long parseMaxFileSize(Properties properties) throws ConfigException {
        return ParsingUtils.parseLongProperty(MAX_FILE_SIZE_KEY, properties);
    }
}
