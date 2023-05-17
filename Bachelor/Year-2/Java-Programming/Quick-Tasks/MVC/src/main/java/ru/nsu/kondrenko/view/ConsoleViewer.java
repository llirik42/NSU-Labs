package ru.nsu.kondrenko.view;

import ru.nsu.kondrenko.model.Listener;

public class ConsoleViewer implements Listener {
    @Override
    public void notifyInputMessage(int number) {
        System.out.print("You have to write [sqrt(" + number + ")] ");
    }

    @Override
    public void notifyGameOver() {
        System.out.println("Game over");
    }

    @Override
    public void notifyCheck(boolean match) {
        if (match) {
            System.out.println("You calculated correctly");
        } else {
            System.out.println("You calculated incorrectly");
        }
    }

    @Override
    public void notifyIncorrectFormat() {
        System.out.println("You can input only numbers");
    }

    @Override
    public void notifyYouAreLate() {
        System.out.println("\nYou are late");
    }
}
