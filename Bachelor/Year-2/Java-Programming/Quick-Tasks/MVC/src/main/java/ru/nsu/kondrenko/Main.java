package ru.nsu.kondrenko;

import ru.nsu.kondrenko.controller.ConsoleController;
import ru.nsu.kondrenko.model.Game;
import ru.nsu.kondrenko.view.ConsoleViewer;

public class Main {
    public static void main(String[] args) {
        ConsoleViewer viewer = new ConsoleViewer();
        Game game = new Game();
        game.registerObserver(viewer);
        game.start();
        ConsoleController gameController = new ConsoleController(game);
        gameController.start();
    }
}
