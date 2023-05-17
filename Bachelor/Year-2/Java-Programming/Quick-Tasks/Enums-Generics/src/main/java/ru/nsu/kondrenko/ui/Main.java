package ru.nsu.kondrenko.ui;

import ru.nsu.kondrenko.model.Mode;
import ru.nsu.kondrenko.model.NumberFile;
import ru.nsu.kondrenko.model.NumberVisitorsFactory;

import java.io.File;

public class Main {
    public static void main(String[] args) {
        Mode mode = Mode.SUMMATION;

        NumberVisitorsFactory<Integer> factory = new NumberVisitorsFactory<>();
        var visitor = factory.create_number_visitor(mode);

        NumberFile<Integer> numberFile = new NumberFile<>(new File("in.txt"));
        numberFile.accept(visitor);

        System.out.println(visitor.getResult());
    }
}
