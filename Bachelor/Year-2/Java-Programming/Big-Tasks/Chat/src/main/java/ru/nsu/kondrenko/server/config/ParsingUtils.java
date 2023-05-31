package ru.nsu.kondrenko.server.config;

import java.util.Properties;
import java.util.function.Function;

final class ParsingUtils {
    private ParsingUtils() {
    }

    public static int parseIntegerProperty(String key, Properties properties) throws ConfigException {
        return parseProperty(key, properties, Integer::parseInt);
    }

    public static boolean parseBooleanProperty(String key, Properties properties) throws ConfigException {
        return parseProperty(key, properties, Boolean::parseBoolean);
    }

    public static String parseStringProperty(String key, Properties properties) throws ConfigException {
        return parseProperty(key, properties, Function.identity());
    }

    private static <T> T parseProperty(String key, Properties properties, Function<String, T> convert) throws ConfigException {
        final String property = extractProperty(key, properties);

        try {
            return convert.apply(property);
        } catch (NumberFormatException exception) {
            throw new ConfigException(String.format("Incorrect value of \"%s\"", key), exception);
        }
    }

    private static String extractProperty(String key, Properties properties) throws ConfigException {
        final String property = properties.getProperty(key);

        if (property == null) {
            throw new ConfigException(String.format("Property \"%s\" not found", key));
        }

        return property;
    }
}
