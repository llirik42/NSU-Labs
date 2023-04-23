package ru.nsu.kondrenko.gui.controller;

import ru.nsu.kondrenko.model.AbstractPlayer;
import ru.nsu.kondrenko.model.exceptions.NotImplementedException;
import ru.nsu.kondrenko.model.game.Move;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.List;

abstract class Player extends AbstractPlayer implements ActionListener {
    public Player() {
        super();
    }

    public Player(final String name) {
        super(name);
    }

    @Override
    public void actionPerformed(ActionEvent e) {
        throw new NotImplementedException();
    }

    public Move generateMove(List<Move> legalMoves) {
        throw new NotImplementedException();
    }
}
