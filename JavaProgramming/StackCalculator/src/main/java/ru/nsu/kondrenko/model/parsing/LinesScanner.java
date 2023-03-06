package ru.nsu.kondrenko.model.parsing;

public interface LinesScanner {
    boolean hasNext();

    ParsedLine next();
}
