package ru.nsu.kondrenko.model;

import ru.nsu.kondrenko.model.game.Move;

import java.util.List;
import java.util.Random;

public final class BotEngine {
    private static final Random random = new Random();

    private BotEngine() {}

    public static Move generateRandomMove(final List<Move> legalMoves) {
        return legalMoves.get(random.nextInt(0, legalMoves.size()));
    }
}
