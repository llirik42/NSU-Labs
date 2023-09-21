package ru.nsu.kondrenko.server.config;

public interface ConfigValidator {
    void validate(ServerConfiguration serverConfiguration) throws ConfigException;
}
