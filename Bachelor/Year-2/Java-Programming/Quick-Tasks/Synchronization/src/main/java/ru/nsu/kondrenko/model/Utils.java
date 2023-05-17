package ru.nsu.kondrenko.model;

import java.util.Random;

public final class Utils {
    private Utils() {}

    private static final Random random = new Random();

    public static int generateRandomNumber(int minValue, int maxValue) {
        return random.nextInt(minValue, maxValue + 1);
    }
}
