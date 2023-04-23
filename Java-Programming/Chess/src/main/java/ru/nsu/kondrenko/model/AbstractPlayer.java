package ru.nsu.kondrenko.model;

public abstract class AbstractPlayer implements PlayerInfo {
    private final String name;

    public AbstractPlayer() {
        name = null;
    }

    public AbstractPlayer(final String name) {
        this.name = name;
    }

    public String getName() {
        return name;
    }
}
