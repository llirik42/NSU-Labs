package ru.nsu.kondrenko.model.parsing.config;

import ru.nsu.kondrenko.model.parsing.PropertiesException;

import java.util.Properties;

public interface ConfigParser {
    Config parse(Properties properties) throws PropertiesException;
}
