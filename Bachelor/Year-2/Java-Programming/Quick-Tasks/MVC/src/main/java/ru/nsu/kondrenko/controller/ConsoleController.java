package ru.nsu.kondrenko.controller;

import ru.nsu.kondrenko.model.Game;

import java.util.InputMismatchException;
import java.util.Scanner;

public class ConsoleController {
    private final Game game;

    private final Scanner scanner = new Scanner(System.in);

    public ConsoleController(Game game) {
        this.game = game;
    }

    public void start() {
        while (!game.gameOver()) {
            try {
                final int value = scanner.nextInt();
                game.putValue(value);
            } catch (InputMismatchException exception) {
                game.sendError();
                scanner.next();
            }
        }
    }
}
