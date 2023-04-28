package ru.nsu.kondrenko.model.parsersfactory;

import ru.nsu.kondrenko.model.parsing.AbstractParser;

public interface ParsersFactory {
    AbstractParser createParser(String inputFilePath);
}
