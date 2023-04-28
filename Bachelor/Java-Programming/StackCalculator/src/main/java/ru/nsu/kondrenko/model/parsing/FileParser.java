package ru.nsu.kondrenko.model.parsing;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.FileNotFoundException;
import java.io.IOException;

import ru.nsu.kondrenko.model.exceptions.FileException;
import ru.nsu.kondrenko.model.exceptions.LinesScannerException;
import ru.nsu.kondrenko.model.Utilities;

public class FileParser extends AbstractParser {
    private final BufferedReader reader;

    private int nextLineNumber = 0;

    private String nextLine;

    public FileParser(String filePath) {
        try {
            this.reader = new BufferedReader(new FileReader(filePath));
            this.nextLine = readUntilNonEmptyLine();
        } catch (FileNotFoundException exception) {
            throw new LinesScannerException("Input file not found", exception);
        }
    }

    @Override
    public boolean hasNext() {
        return nextLine != null;
    }

    @Override
    public ParsedLine next() {
        String currentLine = nextLine;
        int previousLineNumber = nextLineNumber;
        nextLine = readUntilNonEmptyLine();
        return new ParsedLine(currentLine, previousLineNumber, SEPARATOR);
    }

    @Override
    public void close() {
        try {
            reader.close();
        } catch (IOException exception) {
            throw new FileException("Error occurred during closing the input file", exception);
        }
    }

    private String readUntilNonEmptyLine() {
        try {
            String currentLine;

            do {
                currentLine = reader.readLine();
                nextLineNumber++;
            }
            while (currentLine != null && Utilities.isLineFictive(currentLine, COMMENT_PREFIX));

            return currentLine;
        } catch (IOException exception) {
            throw new LinesScannerException("Error occurred during reading the input file", exception);
        }
    }
}
