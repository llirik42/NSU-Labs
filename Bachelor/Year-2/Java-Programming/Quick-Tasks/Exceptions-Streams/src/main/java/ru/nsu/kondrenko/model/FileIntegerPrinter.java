package ru.nsu.kondrenko.model;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;

public class FileIntegerPrinter extends IntegerPrinter {
    private final BufferedWriter writer;

    private final String absoluteFilePath;

    public FileIntegerPrinter(File file, Converter converter) throws IOException {
        super(converter);

        absoluteFilePath = file.getAbsolutePath();

        try {
            writer = new BufferedWriter(new FileWriter(file));
        } catch (IOException exception) {
            throw new IOException("Something went wrong while opening " + absoluteFilePath);
        }
    }

    public void print(int x) throws IOException {
        try {
            writer.write(converter.convert(x) + '\n');
        } catch (IOException exception){
            throw new IOException("Something went wrong while writing to " + absoluteFilePath);
        }
    }

    public void close() throws IOException {
        try {
            writer.close();
        } catch (IOException exception){
            throw new IOException("Something went wrong while closing " + absoluteFilePath);
        }
    }
}
