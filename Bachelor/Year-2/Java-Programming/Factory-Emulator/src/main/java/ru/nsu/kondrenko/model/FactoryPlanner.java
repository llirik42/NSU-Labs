package ru.nsu.kondrenko.model;

import ru.nsu.kondrenko.model.work.threadpools.Factory;

public class FactoryPlanner extends Thread {
    private static final int CARS_TO_SALE_PLAN = 100;

    private final Factory factory;

    private int tasksAssigned = 0;

    private boolean wasNotified = false;

    public FactoryPlanner(Factory factory) {
        this.factory = factory;
    }

    public void run() {
        while (true) {
            final int newTasksCount = getNewTasksCount();
            tasksAssigned += newTasksCount;
            factory.addTasks(newTasksCount);
            wasNotified = false;

            while (!wasNotified) {
                synchronized (this) {
                    try {
                        wait();
                    } catch (InterruptedException exception) {
                        interrupt();
                        return;
                    }
                }
            }
        }
    }

    public void notifyAboutCarSale() {
        wasNotified = true;
    }

    private int getNewTasksCount() {
        return tasksAssigned < CARS_TO_SALE_PLAN ? 2 : 0;
    }
}
