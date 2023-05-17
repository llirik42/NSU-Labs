package ru.nsu.kondrenko.model.parsing.config;

import ru.nsu.kondrenko.model.parsing.limits.Limits;

public interface ConfigValidator {
    boolean validate(Config config, Limits limits);
}
