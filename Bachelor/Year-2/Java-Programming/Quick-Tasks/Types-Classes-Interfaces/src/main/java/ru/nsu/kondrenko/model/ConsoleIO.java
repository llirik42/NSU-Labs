package ru.nsu.kondrenko.model;

import java.util.Random;
import java.util.Scanner;

class ConsoleIO implements Reader, Writer{
    private final static String[] WORDS = new String[] {"word1", "word2", "word3", "word4"};

    ConsoleIO(){}

    @Override
    public int readInt() {
        var scanner = new Scanner(System.in);
        return scanner.nextInt();
    }

    @Override
    public void write(int n) {
        Random random = new Random();

        int randomIndex = random.nextInt(0, WORDS.length);

        for (int i = 0; i < n; i++){
            System.out.print(WORDS[randomIndex] + ' ');
        }
    }
}
