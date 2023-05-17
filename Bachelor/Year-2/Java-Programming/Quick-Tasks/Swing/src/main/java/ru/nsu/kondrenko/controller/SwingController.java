package ru.nsu.kondrenko.controller;

import ru.nsu.kondrenko.model.Game;

import javax.swing.text.BadLocationException;
import javax.swing.text.Document;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

public class SwingController implements ActionListener {
    private final Game game;
    private final Document document;

    public SwingController(Game game, Document document) {
        this.game = game;
        this.document = document;
    }

    @Override
    public void actionPerformed(ActionEvent actionEvent) {
        try {
            game.putValue(Integer.parseInt(document.getText(0, document.getLength())));
        } catch (NumberFormatException exception) {
            game.sendError();
        } catch (BadLocationException exception) {
            throw new RuntimeException(exception);
        }
    }
}
