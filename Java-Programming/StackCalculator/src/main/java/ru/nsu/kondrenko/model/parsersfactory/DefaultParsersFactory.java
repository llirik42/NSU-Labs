package ru.nsu.kondrenko.model.parsersfactory;

import ru.nsu.kondrenko.model.parsing.AbstractParser;
import ru.nsu.kondrenko.model.parsing.FileParser;
import ru.nsu.kondrenko.model.parsing.InputParser;

public class DefaultParsersFactory implements ParsersFactory {
    @Override
    public AbstractParser createParser(String inputFilePath) {
        return inputFilePath == null
                ? new InputParser()
                : new FileParser(inputFilePath);
    }
}
