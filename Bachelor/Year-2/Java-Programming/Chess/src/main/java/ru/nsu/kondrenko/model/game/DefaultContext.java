package ru.nsu.kondrenko.model.game;

class DefaultContext implements Context {
    private boolean isGameOver = false;

    private Side currentSide = Side.WHITE;

    @Override
    public boolean isGameOver() {
        return isGameOver;
    }

    @Override
    public void setGameOver() {
        isGameOver = true;
    }

    @Override
    public Side getCurrentSide() {
        return currentSide;
    }

    @Override
    public void switchCurrentSide() {
        currentSide = currentSide == Side.WHITE ? Side.BLACK : Side.WHITE;
    }
}
