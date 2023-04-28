package ru.nsu.kondrenko.gui.controller;

import ru.nsu.kondrenko.model.BotEngine;
import ru.nsu.kondrenko.model.game.Move;

import java.util.List;

class BotPlayer extends Player {
    @Override
    public boolean isBot() {
        return true;
    }

    @Override
    public Move generateMove(final List<Move> legalMoves) {
        return BotEngine.generateRandomMove(legalMoves);
    }
}
