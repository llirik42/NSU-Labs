package ru.nsu.kondrenko.model.parsing;

import java.util.Properties;
import java.util.function.Function;

public final class Utils {
    private Utils() {
    }

    public static int parseIntegerProperty(String key, Properties properties) throws PropertiesException {
        return parseProperty(key, properties, Integer::parseInt);
    }

    public static boolean parseBooleanProperty(String key, Properties properties) throws PropertiesException {
        return parseProperty(key, properties, Boolean::parseBoolean);
    }

    private static <T> T parseProperty(String key, Properties properties, Function<String, T> convert) throws PropertiesException {
        final String property = extractProperty(key, properties);

        try {
            return convert.apply(property);
        } catch (NumberFormatException exception) {
            throw new PropertiesException(String.format("Incorrect value of %s", key), exception);
        }
    }

    private static String extractProperty(String key, Properties properties) throws PropertiesException {
        final String property = properties.getProperty(key);

        if (property == null) {
            throw new PropertiesException(String.format("No %s", key));
        }

        return property;
    }
}
