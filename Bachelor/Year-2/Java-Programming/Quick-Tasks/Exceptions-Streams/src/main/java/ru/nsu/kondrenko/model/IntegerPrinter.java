package ru.nsu.kondrenko.model;

import java.io.Closeable;
import java.io.IOException;

public abstract class IntegerPrinter implements Closeable {
    protected final Converter converter;

    protected IntegerPrinter(Converter converter){
        this.converter = converter;
    }

    public abstract void print(int x) throws IOException;

    public abstract void close() throws IOException;
}
