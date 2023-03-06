package ru.nsu.kondrenko.model.parsing;

public abstract class AbstractParser implements LinesScanner, AutoCloseable {
    protected static final String SEPARATOR = " ";

    protected static final String COMMENT_PREFIX = "#";
}
