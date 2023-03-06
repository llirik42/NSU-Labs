package ru.nsu.kondrenko.model.parsing;

import ru.nsu.kondrenko.model.Utilities;

import java.util.Arrays;

public final class ParsedLine {
    private final String instructionName;

    private final String[] instructionOperands;

    private final int lineNumber;

    public ParsedLine(String line, int lineNumber, String separator) {
        var separatedWords = Utilities.reduceSpaces(line).split(separator);

        this.instructionName = separatedWords[0];
        this.instructionOperands = Arrays.copyOfRange(separatedWords, 1, separatedWords.length);
        this.lineNumber = lineNumber;
    }

    public String getInstructionName() {
        return instructionName;
    }

    public String[] getInstructionOperands() {
        return instructionOperands;
    }

    public int getLineNumber() {
        return lineNumber;
    }
}
