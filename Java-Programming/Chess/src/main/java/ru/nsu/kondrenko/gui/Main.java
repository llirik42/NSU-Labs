package ru.nsu.kondrenko.gui;

import ru.nsu.kondrenko.gui.controller.Controller;
import ru.nsu.kondrenko.gui.view.GUIViewer;
import ru.nsu.kondrenko.model.Model;
import ru.nsu.kondrenko.model.statistics.StatisticsDiskStorage;
import ru.nsu.kondrenko.model.statistics.StatisticsStorage;

public class Main {
    public static void main(String[] args) {
        try {
            final StatisticsStorage statisticsStorage = new StatisticsDiskStorage();
            final Model model = new Model(statisticsStorage);
            final Controller controller = new Controller(model);
            final GUIViewer viewer = new GUIViewer(controller);
            model.setListener(viewer);
        } catch (Exception exception) {
            System.err.println(exception.getLocalizedMessage());
        }
    }
}
