package ru.nsu.kondrenko.ui;

import ru.nsu.kondrenko.model.IOFactory;
import ru.nsu.kondrenko.model.Reader;
import ru.nsu.kondrenko.model.Writer;

public class Main {
    public static void main(String[] args) {
        var factory = new IOFactory();

        Reader reader = factory.createReader();
        Writer writer = factory.createWriter();

        var someInt = reader.readInt();
        System.out.println("Wow! " + someInt);

        writer.write(2);
    }
}
