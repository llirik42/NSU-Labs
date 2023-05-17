package ru.nsu.kondrenko.model;

import java.io.File;
import java.io.FileNotFoundException;
import java.math.BigDecimal;
import java.util.NoSuchElementException;
import java.util.Scanner;

public class NumberFile<T extends Number> {
    private final File file;

    public NumberFile(File file) {
        this.file = file;
    }

    public void accept(Visitor<T> visitor){
        try (Scanner scanner = new Scanner(file)) {
            while (scanner.hasNext()) {
                String line = scanner.nextLine();
                BigDecimal tmp = new BigDecimal(line);
                T number = (T) tmp;
                visitor.visit(number);
            }
        } catch (FileNotFoundException exception) {
            throw new RuntimeException("Number file is not found\n", exception);
        } catch (IllegalStateException | NoSuchElementException exception) {
            throw new RuntimeException("IO error occurred", exception);
        }
    }
}
