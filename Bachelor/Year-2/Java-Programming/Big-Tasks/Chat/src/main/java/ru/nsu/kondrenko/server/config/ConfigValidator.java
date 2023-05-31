package ru.nsu.kondrenko.server.config;

public interface ConfigValidator {
    void validate(ServerConfiguration serverConfiguration) throws ConfigException;

    void validate(String protocol) throws ConfigException;
}
