package ru.nsu.kondrenko.model.parsing;

import ru.nsu.kondrenko.model.Utilities;

public final class ParsedLine {
    private final String[] words;

    private final int lineNumber;

    public ParsedLine(String line, int lineNumber, String separator) {
        words = Utilities.reduceSpaces(line).split(separator);
        this.lineNumber = lineNumber;
    }

    public String[] getWords() {
        return words;
    }

    public int getLineNumber() {
        return lineNumber;
    }
}
