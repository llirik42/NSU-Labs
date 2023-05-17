package ru.nsu.kondrenko.model;

public interface Listener {
    void notifyInputMessage(int number);

    void notifyCheck(boolean match);

    void notifyIncorrectFormat();

    void notifyYouAreLate();

    void notifyGameOver();
}
