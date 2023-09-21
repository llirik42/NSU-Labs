package ru.nsu.kondrenko.server.config;

public class DefaultConfigValidator implements ConfigValidator {
    @Override
    public void validate(ServerConfiguration serverConfiguration) throws ConfigException {
        if (serverConfiguration.backlog() <= 0) {
            throw new ConfigException("Invalid backlog: it must be positive");
        }

        if (serverConfiguration.transferRateUpdateTime() <= 0) {
            throw new ConfigException("Invalid transfer-rate-time: it must be positive");
        }

        if (serverConfiguration.maxFileNameSize() <= 0) {
            throw new ConfigException("Invalid max file name size: it must be positive");
        }

        if (serverConfiguration.maxFileSize() <= 0) {
            throw new ConfigException("Invalid max file size: it must be positive");
        }
    }
}
