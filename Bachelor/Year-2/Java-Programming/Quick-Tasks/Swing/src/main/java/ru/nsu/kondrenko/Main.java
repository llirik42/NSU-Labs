package ru.nsu.kondrenko;

import ru.nsu.kondrenko.model.Game;
import ru.nsu.kondrenko.view.SwingViewer;

import javax.swing.*;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;

public class Main {
    public static void main(String[] args){
        SwingUtilities.invokeLater(() -> {
            Game game = new Game();
            SwingViewer swingViewer = new SwingViewer(game);
            game.registerObserver(swingViewer);

            swingViewer.addWindowListener(new WindowAdapter() {
                @Override
                public void windowClosing(WindowEvent event) {
                    game.putValue(Game.EXIT_VALUE);
                }
            });

            game.start();
            swingViewer.setVisible(true);
        });
    }
}
