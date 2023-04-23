package ru.nsu.kondrenko.console.controller;

import ru.nsu.kondrenko.model.game.ChessPosition;
import ru.nsu.kondrenko.model.game.Move;

import java.util.HashMap;
import java.util.Map;

class HumanPlayer extends Player {
    private static final Map<Character, Integer> LETTER_TO_RANK = new HashMap<>() {{
        put('a', 1);
        put('b', 2);
        put('c', 3);
        put('d', 4);
        put('e', 5);
        put('f', 6);
        put('g', 7);
        put('h', 8);
    }};

    public HumanPlayer(final String name) {
        super(name);
    }

    @Override
    public boolean isBot() {
        return false;
    }

    @Override
    public Move generateMove(final String stringMove) {
        final ChessPosition from = extractChessPosition(stringMove.substring(0, 2));
        final ChessPosition to = extractChessPosition(stringMove.substring(2, 4));
        return new Move(from, to);
    }

    private ChessPosition extractChessPosition(final String stringBoardPosition) {
        final int file = LETTER_TO_RANK.get(stringBoardPosition.charAt(0));
        final int rank = Integer.parseInt(String.valueOf(stringBoardPosition.charAt(1)));
        return new ChessPosition(file, rank);
    }
}
