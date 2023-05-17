package ru.nsu.kondrenko.model.work.factory;

public abstract class FactoryPlanner extends Thread {
    private final Factory factory;
    private final Object synchronizationObject = new Object();
    private boolean wasNotified = false;

    protected FactoryPlanner(Factory factory) {
        this.factory = factory;
    }

    @Override
    public void run() {
        while (true) {
            final int newTasksCount = getNewTasksCount();
            factory.addTasks(newTasksCount);
            wasNotified = false;

            while (!wasNotified) {
                synchronized (synchronizationObject) {
                    try {
                        synchronizationObject.wait();
                    } catch (InterruptedException exception) {
                        interrupt();
                        return;
                    }
                }
            }
        }
    }

    public Object getSynchronizationObject() {
        return synchronizationObject;
    }

    public void notifyAboutCarSale() {
        wasNotified = true;
    }

    public abstract int getNewTasksCount();
}
