package ru.nsu.kondrenko.server.config;

import ru.nsu.kondrenko.common.Utils;
import ru.nsu.kondrenko.common.protocols.Protocol;

public class DefaultConfigValidator implements ConfigValidator {
    private static boolean isBacklogValid(int backlog) {
        return backlog > 0;
    }

    private static boolean isRecentMessagesCountValid(int recentMessagesCount) {
        return recentMessagesCount >= 0;
    }

    @Override
    public void validate(ServerConfiguration serverConfiguration) throws ConfigException {
        if (!Utils.isPortValid(serverConfiguration.port())) {
            throw new ConfigException("Incorrect por number");
        }

        if (!isBacklogValid(serverConfiguration.backlog())) {
            throw new ConfigException("Incorrect backlog");
        }

        if (!Utils.isAddressValid(serverConfiguration.bindAddress())) {
            throw new ConfigException("Incorrect bind address");
        }

        if (!isRecentMessagesCountValid(serverConfiguration.recentMessagesCount())) {
            throw new ConfigException("Incorrect recent messages count");
        }
    }

    @Override
    public void validate(String protocol) throws ConfigException {
        boolean isValid = false;

        for (final var it : Protocol.values()) {
            if (it.name().equalsIgnoreCase(protocol)) {
                isValid = true;
                break;
            }
        }

        if (!isValid) {
            throw new ConfigException("Unknown protocol");
        }
    }
}
