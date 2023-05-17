package ru.nsu.kondrenko.model;

import java.util.Random;
import java.util.Timer;
import java.util.TimerTask;

public class Game {
    public final static int EXIT_VALUE = -1;

    private Listener gameObserver = null;

    private final static int MIN_VALUE = 1;
    private final static int MAX_VALUE = 1000;
    private final Timer timer = new Timer();

    private final Random random = new Random();

    private boolean gameOver = false;
    private int currentNumber;

    public void registerObserver(Listener gameObserver) {
        this.gameObserver = gameObserver;
    }

    public void start() {
        int DELAY = 5000;
        timer.schedule(new TimerTask() {
            @Override
            public void run() {
                gameObserver.notifyYouAreLate();
                startIteration();
            }
        }, DELAY, DELAY);

        startIteration();
    }

    public void putValue(int value) {
        if (value == EXIT_VALUE) {
            gameOver = true;
            gameObserver.notifyGameOver();
            timer.cancel();
            return;
        }

        gameObserver.notifyCheck(value == (int) Math.sqrt(currentNumber));
        currentNumber = generateRandomNumber();
        gameObserver.notifyInputMessage(currentNumber);
    }

    public boolean gameOver() {
        return gameOver;
    }

    public void sendError() {
        gameObserver.notifyIncorrectFormat();
    }

    private int generateRandomNumber() {
        return random.nextInt(MIN_VALUE, MAX_VALUE + 1);
    }

    private void startIteration() {
        currentNumber = generateRandomNumber();
        gameObserver.notifyInputMessage(currentNumber);
    }
}
