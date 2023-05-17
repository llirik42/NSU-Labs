package ru.nsu.kondrenko.view;

import ru.nsu.kondrenko.model.Listener;

public class View implements Listener {
    @Override
    public void notifyThreadEnter(String threadName, int time) {
        System.out.println("Поток \"" + threadName + "\" зашёл со временем " + time);
    }

    @Override
    public void notifyThreadExit(String threadName) {
        System.out.println("Поток \"" + threadName + "\" вышел");
    }
}
