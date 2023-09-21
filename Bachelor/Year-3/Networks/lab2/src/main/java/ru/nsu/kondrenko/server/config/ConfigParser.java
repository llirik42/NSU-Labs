package ru.nsu.kondrenko.server.config;

import java.util.Properties;

public interface ConfigParser {
    int parsePort(Properties properties) throws ConfigException;

    int parseBacklog(Properties properties) throws ConfigException;

    String parseBindAddress(Properties properties) throws ConfigException;

    int parseTransferRateUpdateTimeMs(Properties properties) throws ConfigException;

    int parseMaxFileNameSize(Properties properties) throws ConfigException;

    long parseMaxFileSize(Properties properties) throws ConfigException;
}
