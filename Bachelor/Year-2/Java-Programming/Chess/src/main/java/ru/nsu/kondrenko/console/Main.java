package ru.nsu.kondrenko.console;

import ru.nsu.kondrenko.console.controller.Controller;
import ru.nsu.kondrenko.console.view.View;
import ru.nsu.kondrenko.model.Model;
import ru.nsu.kondrenko.model.statistics.StatisticsDiskStorage;
import ru.nsu.kondrenko.model.statistics.StatisticsStorage;

public class Main {
    public static void main(String[] args) {
        try {
            StatisticsStorage statisticsStorage = new StatisticsDiskStorage();
            final Model model = new Model(statisticsStorage);
            View viewer = new View();
            model.setListener(viewer);
            Controller controller = new Controller(model);
            controller.start();
        } catch (Exception exception) {
            System.err.println(exception.getLocalizedMessage());
        }
    }
}
