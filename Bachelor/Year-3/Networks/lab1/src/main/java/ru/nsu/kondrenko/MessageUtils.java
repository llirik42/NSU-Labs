package ru.nsu.kondrenko;

import java.util.List;

public final class MessageUtils {
    private MessageUtils() {
    }

    public static void printAppCopies(final List<AppCopy> appCopies) {
        System.out.printf("Total count of copies=%d\n", appCopies.size());
        for (final var it : appCopies) {
            System.out.println(it);
        }
        System.out.println();
    }
}
