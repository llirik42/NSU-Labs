package ru.nsu.kondrenko.server.config;

import java.util.Properties;

public class DefaultConfigParser implements ConfigParser {
    private static final String PORT_KEY = "PORT";
    private static final String BACKLOG_KEY = "BACKLOG";
    private static final String BIND_ADDRESS_KEY = "BIND_ADDRESS";
    private static final String RECENT_MESSAGES_COUNT_KEY = "RECENT_MESSAGES_COUNT";
    private static final String PROTOCOL_KEY = "PROTOCOL";
    private static final String LOGGING_KEY = "LOGGING";

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
    public int parseRecentMessagesCount(Properties properties) throws ConfigException {
        return ParsingUtils.parseIntegerProperty(RECENT_MESSAGES_COUNT_KEY, properties);
    }

    @Override
    public String parseProtocol(Properties properties) throws ConfigException {
        return ParsingUtils.parseStringProperty(PROTOCOL_KEY, properties);
    }

    @Override
    public boolean parseLogging(Properties properties) throws ConfigException {
        return ParsingUtils.parseBooleanProperty(LOGGING_KEY, properties);
    }
}
