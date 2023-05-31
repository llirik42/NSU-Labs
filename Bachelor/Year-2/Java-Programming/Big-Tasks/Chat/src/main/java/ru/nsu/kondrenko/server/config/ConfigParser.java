package ru.nsu.kondrenko.server.config;

import java.util.Properties;

public interface ConfigParser {
    int parsePort(Properties properties) throws ConfigException;

    int parseBacklog(Properties properties) throws ConfigException;

    String parseBindAddress(Properties properties) throws ConfigException;

    int parseRecentMessagesCount(Properties properties) throws ConfigException;

    String parseProtocol(Properties properties) throws ConfigException;

    boolean parseLogging(Properties properties) throws ConfigException;
}
