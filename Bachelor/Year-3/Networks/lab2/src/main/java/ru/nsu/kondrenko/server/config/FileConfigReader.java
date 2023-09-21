package ru.nsu.kondrenko.server.config;

import java.io.FileInputStream;
import java.io.IOException;
import java.util.Properties;

public class FileConfigReader implements ConfigReader {
    private final Properties properties = new Properties();

    public FileConfigReader(String fileName) throws ConfigException {
        try {
            properties.load(new FileInputStream(fileName));
        } catch (IOException | IllegalArgumentException exception) {
            throw new ConfigException(exception);
        } catch (NullPointerException exception) {
            throw new ConfigException(String.format("File \"%s\" not found!", fileName), exception);
        }
    }

    @Override
    public Properties read() {
        return properties;
    }
}