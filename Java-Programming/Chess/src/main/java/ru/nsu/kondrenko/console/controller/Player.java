package ru.nsu.kondrenko.console.controller;

import ru.nsu.kondrenko.model.AbstractPlayer;
import ru.nsu.kondrenko.model.exceptions.NotImplementedException;
import ru.nsu.kondrenko.model.game.Move;

import java.util.List;

abstract class Player extends AbstractPlayer {
    public Player() {
        super();
    }

    public Player(final String name) {
        super(name);
    }

    public Move generateMove(final String stringMove) {
        throw new NotImplementedException();
    }

    public Move generateMove(final List<Move> legalMoves) {
        throw new NotImplementedException();
    }
}
