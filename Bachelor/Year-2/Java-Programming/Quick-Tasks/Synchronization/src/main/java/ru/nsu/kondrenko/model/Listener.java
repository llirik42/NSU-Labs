package ru.nsu.kondrenko.model;

public interface Listener {
    void notifyThreadEnter(String threadName, int time);

    void notifyThreadExit(String threadName);
}
