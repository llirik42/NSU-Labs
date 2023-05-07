package ru.nsu.kondrenko.model.parsing.limits;

import ru.nsu.kondrenko.model.parsing.PropertiesException;

import java.util.Properties;

public interface LimitsParser {
    Limits parse(Properties properties) throws PropertiesException;
}
