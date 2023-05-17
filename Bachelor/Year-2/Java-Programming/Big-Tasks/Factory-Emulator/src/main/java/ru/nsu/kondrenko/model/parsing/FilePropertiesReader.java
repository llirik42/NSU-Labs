package ru.nsu.kondrenko.model.parsing;

import ru.nsu.kondrenko.Main;

import java.io.IOException;
import java.util.Properties;

public class FilePropertiesReader implements PropertiesReader {
    private final Properties properties = new Properties();

    public FilePropertiesReader(String fileName) throws PropertiesException {
        try {
            properties.load(Main.class.getClassLoader().getResourceAsStream(fileName));
        } catch (IOException | IllegalArgumentException exception) {
            throw new PropertiesException(exception);
        } catch (NullPointerException exception) {
            throw new PropertiesException(String.format("File \"%s\" not found!", fileName), exception);
        }
    }

    @Override
    public Properties read() {
        return properties;
    }
}
