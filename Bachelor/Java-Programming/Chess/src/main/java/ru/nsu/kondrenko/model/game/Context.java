package ru.nsu.kondrenko.model.game;

public interface Context {
    boolean isGameOver();

    void setGameOver();

    Side getCurrentSide();

    void switchCurrentSide();
}
