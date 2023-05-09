package ru.nsu.kondrenko.model.work.factory;

public class MaxFactoryPlanner extends FactoryPlanner {
    private static final int CARS_TO_SALE_PLAN = 1000;

    private boolean assignedAllTasks = false;

    public MaxFactoryPlanner(Factory factory) {
        super(factory);
    }

    @Override
    protected int getNewTasksCount() {
        int ret = assignedAllTasks ? 0 : CARS_TO_SALE_PLAN;
        assignedAllTasks = true;
        return ret;
    }
}
