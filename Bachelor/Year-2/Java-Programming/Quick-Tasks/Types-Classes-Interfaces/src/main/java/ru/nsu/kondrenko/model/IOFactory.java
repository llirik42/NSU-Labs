package ru.nsu.kondrenko.model;

public class IOFactory {
    private final ConsoleIO consoleIO;

    public IOFactory(){
        consoleIO = new ConsoleIO();
    }

    public Reader createReader(){
        return consoleIO;
    }

    public Writer createWriter(){
        return consoleIO;
    }
}
