package ru.nsu.kondrenko.ui;

import ru.nsu.kondrenko.model.*;

import java.io.File;
import java.io.IOException;

public class Main {
    public static void main(String[] args) {
        File arabicOutput = new File("./arabic.txt");
        File romanOutput = new File("./roman.txt");

        Converter arabicConverter = new ArabicConverter();
        Converter romanConverter = new RomanConverter();

        try (IntegerPrinter arabicPrinter = new FileIntegerPrinter(arabicOutput, arabicConverter);
             IntegerPrinter romanPrinter = new FileIntegerPrinter(romanOutput, romanConverter)) {

            for (int i = 1; i <= 1000; i++) {
                arabicPrinter.print(i);
                romanPrinter.print(i);
            }
        } catch (IOException exception){
            System.err.println(exception.getLocalizedMessage());
        }
    }
}
