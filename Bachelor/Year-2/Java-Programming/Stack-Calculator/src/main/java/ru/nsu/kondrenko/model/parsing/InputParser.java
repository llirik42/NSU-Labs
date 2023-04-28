package ru.nsu.kondrenko.model.parsing;

import ru.nsu.kondrenko.model.exceptions.LinesScannerException;
import ru.nsu.kondrenko.model.Utilities;

import java.util.NoSuchElementException;
import java.util.Scanner;

public class InputParser extends AbstractParser {
    private final Scanner scanner = new Scanner(System.in);

    private int currentLineNumber = 0;

    @Override
    public boolean hasNext() {
        return true;
    }

    @Override
    public ParsedLine next() {
        String nextLine = readUntilNonEmptyLine();
        return new ParsedLine(nextLine, currentLineNumber, SEPARATOR);
    }

    @Override
    public void close() {}

    private String readUntilNonEmptyLine() {
        try {
            String currentLine;

            do {
                currentLine = scanner.nextLine();
                currentLineNumber++;
            }
            while (Utilities.isLineFictive(currentLine, COMMENT_PREFIX));

            return currentLine;
        } catch (NoSuchElementException | IllegalStateException exception) {
            throw new LinesScannerException("Error occurred during reading input stream");
        }
    }
}
