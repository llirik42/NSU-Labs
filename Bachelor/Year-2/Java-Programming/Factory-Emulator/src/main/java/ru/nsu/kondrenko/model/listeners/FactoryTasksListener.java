package ru.nsu.kondrenko.model.listeners;

public interface FactoryTasksListener {
    void notifyAboutNewTasks(int newTasksCount);

    void notifyAboutClosingTask();
}
