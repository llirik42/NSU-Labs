package ru.nsu.kondrenko.server.config;

public record ServerConfiguration(
        int port,
        int backlog,
        String bindAddress,
        int transferRateUpdateTime,
        int maxFileNameSize,
        long maxFileSize) {
}
